//============================================================================
// 
// ２０２５年６月５日：召喚グループ選択クラスを作成する [SummonGroupSelector.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************************************
// インクルードファイル
//****************************************************
#include "SummonGroupSelector.h"  // 元ヘッダ
#include "summon_manager_state.h" // 召喚マネージャーステート
#include "game.h" // ゲームシーン
#include "manager.h"     // マネージャー
#include "object.h"      // オブジェクト
#include "renderer.h"    // デバッグ用
#include "calculation.h" // 計算用
#include "VisionBlockSpawner.h" // 視野妨害オブジェクトスポナー

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;
using namespace std;

//============================================================================
//
// publicメンバ
//
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CSummonGroupSelector::CSummonGroupSelector() :
	m_nSuccessPhaseNum{ 0 },
	m_nNumUseHandSword{ 0 },
	m_nDifficultyScore{ 0 },
	m_DifficultyRank{CSummon_Manager::DIFFICULTY::EASY}
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CSummonGroupSelector::~CSummonGroupSelector()
{
	// 何もなし
}

//============================================================================
// 更新処理
//============================================================================
void CSummonGroupSelector::Update()
{
	UpdateDifficultyRank(); // 難易度ランク更新

	// 難易度スコアを範囲制限
	m_nDifficultyScore = Calculation::Value::LimitRange(m_nDifficultyScore, 0, s_nMaxDifficultyScore);

	// デバッグ表示
	CRenderer::SetDebugString("難易度スコア：" + to_string(m_nDifficultyScore));
	CRenderer::SetDebugString("難易度ランク：" + to_string(static_cast<int>(m_DifficultyRank)));}

//============================================================================
// フェーズ成功回数を取得
//============================================================================
const int& CSummonGroupSelector::GetSuccessPhaseNum() const
{
	return m_nSuccessPhaseNum;
}

//============================================================================
// フェーズ成功回数を設定
//============================================================================
void CSummonGroupSelector::SetSuccessPhaseNum(int nNum)
{
	m_nSuccessPhaseNum = nNum;
}

//============================================================================
// 手刀使用回数を取得
//============================================================================
const int& CSummonGroupSelector::GetNumUseHandSword() const
{
	return m_nNumUseHandSword;
}

//============================================================================
// 手刀使用回数を設定
//============================================================================
void CSummonGroupSelector::SetNumUseHandSword(int nNum)
{
	m_nNumUseHandSword = nNum;
}

//============================================================================
// 難易度スコアを取得
//============================================================================
const int& CSummonGroupSelector::GetDifficultyScore() const
{
	return m_nDifficultyScore;
}

//============================================================================
// 難易度スコアを設定
//============================================================================
void CSummonGroupSelector::SetDifficultyScore(int nScore)
{
	m_nDifficultyScore = nScore;
}

//============================================================================
// 召喚グループ番号を取得する
//============================================================================
const int& CSummonGroupSelector::GetSummonGroupNum() const
{
	return m_nSummonGroupNum;
}

//============================================================================
// 召喚グループ番号を設定する
//============================================================================
void CSummonGroupSelector::SetSummonGroupNum(int nNum)
{
	m_nSummonGroupNum = nNum;
}

//============================================================================
// 難易度ランクを更新する
//============================================================================
void CSummonGroupSelector::UpdateDifficultyRank()
{
	int nDifficultyRank = static_cast<int>(m_DifficultyRank); // 難易度ランク

	// 難易度スコアがボーダーを超えたので難易度ランクを上げる。EXPERTは最大ランクなので上げません
	if (m_DifficultyRank != CSummon_Manager::DIFFICULTY::EXPERT)
	{
		if (m_nDifficultyScore >= s_nDifficultyBorderScore[nDifficultyRank + 1])
		{
			m_DifficultyRank = static_cast<CSummon_Manager::DIFFICULTY>(nDifficultyRank + 1);
		}
	}

	// 難易度スコアが前のランクのボーダーを下回ったので難易度ランクを下げる。EASYは最低ランクなので下げません。
	if (m_DifficultyRank != CSummon_Manager::DIFFICULTY::EASY)
	{
		if (m_nDifficultyScore < s_nDifficultyBorderScore[nDifficultyRank])
		{
			m_DifficultyRank = static_cast<CSummon_Manager::DIFFICULTY>(nDifficultyRank - 1);
		}
	}
}

//============================================================================
// 召喚グループを決定する
//============================================================================
void CSummonGroupSelector::DecideSummonGroup()
{
	// 召喚マネージャーへのポインタ取得
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));

	// 召喚マネージャーが存在しているなら
	if (pSummonManager)
	{
		CSummon_Manager_State* pSummonManagerState = pSummonManager->GetState(); // 召喚マネージャー状態へのポインタ
		const JSON& SummonPatternList = pSummonManagerState->GetJsonSummonPatternList();    // 召喚パターンリスト
		vector<int> PossibleSummonGroupNum; // 召喚出来るグループ番号動的配列
		CSummon_Manager::DIFFICULTY Difficulty = CSummon_Manager::DIFFICULTY::MAX; // 難易度（どれにも当てはまらないMAXに初期化)
		int nCnt = 0; // 配列カウント用

		// 範囲ベースfor文で全ての召喚パターンを検索
		for (auto& Obj : SummonPatternList)
		{
			// レベル名のキーが存在しないなら処理を中断
			if (!Obj.contains("LevelName"))
			{
				assert(false && "召喚パターンリストにレベル名のキーがありません");
			}

			// どれにも当てはまらないMAXに初期化（難易度とは別の"Test"というレベルがあるのでそれを読み込まないため)
			Difficulty = CSummon_Manager::DIFFICULTY::MAX;

			// 難易度決定
			if (Obj["LevelName"] == "Easy")
			{
				Difficulty = CSummon_Manager::DIFFICULTY::EASY;
			}
			else if (Obj["LevelName"] == "Normal")
			{
				Difficulty = CSummon_Manager::DIFFICULTY::NORMAL;
			}
			else if (Obj["LevelName"] == "Hard")
			{
				Difficulty = CSummon_Manager::DIFFICULTY::HARD;
			}
			else if (Obj["LevelName"] == "Expert")
			{
				Difficulty = CSummon_Manager::DIFFICULTY::EXPERT;
			}

			if (Difficulty == m_DifficultyRank)
			{
				PossibleSummonGroupNum.push_back(nCnt); // 現在の召喚グループ番号を格納
			}
			
			nCnt++;
		}

		// 現在の難易度に該当する召喚パターンがない場合、召喚グループ番号を決定しない
		if (PossibleSummonGroupNum.size() > 0)
		{
			int nPossibleSummonRand = rand() % PossibleSummonGroupNum.size();

			// 現在の難易度ランクのランダムなグループ番号を設定する
			m_nSummonGroupNum = PossibleSummonGroupNum[nPossibleSummonRand];

			// デバッグ表示
			CRenderer::SetDebugString("難易度の召喚グループ数：" + to_string(PossibleSummonGroupNum.size()));
		}
	}
}

//============================================================================
// 難易度ランク取得
//============================================================================
const CSummon_Manager::DIFFICULTY& CSummonGroupSelector::GetDifficultyRank() const
{
	return m_DifficultyRank;
}

//============================================================================
// 召喚グループを決定する
//============================================================================
const int& CSummonGroupSelector::GetMaxDifficultyScore()
{
	return s_nMaxDifficultyScore;
}
