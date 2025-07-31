//============================================================================
// 
// ２０２５年５月１５日：召喚グループを [summon_mmanager_state_start.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "summon_manager_state_start.h" // 元ヘッダ
#include "pch.h"                        // 様々なインクルード情報を使う
#include "obstacle.h"                   // 障害物
#include "input_keyboard.h"             // キー入力
#include "PlayerState_Thrusting.h"      // プレイヤー状態＿突き
#include "PlayerState_Returning.h"      // プレイヤー状態＿戻る
#include "PlayerState_BeforeThrust.h"   // プレイヤー状態＿突きをする前
#include "SummonGroupSelector.h"        // 召喚グループ選択
#include "zone.h"                       // ゾーン
#include "game.h"                       // ゲーム
#include "GamePlayer.h"                 // ゲームプレイヤー
#include "manager.h"                    // マネージャー
#include "VisionBlockSpawner.h"         // 視野妨害オブジェクトスポナー
#include "VisionBlocker.h"              // 視野妨害オブジェクト
#include "particle.h"                   // パーティクル
#include "renderer.h"                   // レンダラー（デバッグ表示など）
#include "SummonCommonData.h"           // 召喚グループ共通データ
#include "StencilMask.h"                // ステンシルマスク
#include "HoleObstacle.h"               // 穴あき障害物

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

//====================================================
// デフォルトコンストラクタ
//====================================================
CSummon_Manager_State_Start::CSummon_Manager_State_Start() :
	m_nSummonCoolTimeCnt{ s_nMaxSummonCoolTime },
	m_bSummonCoolTime{ true },
	m_bStartPhase{ false },
	m_nSummonCoolTimeFrame{ 0 },
	m_bSuccessPhase{ false },
	m_nSummonNum{ 0 },
	m_nPhaseComboCnt{ 0 },
	m_nNumPhase{0},
	m_nMaxPhase{ s_nNormalMaxPhase },
	m_bAllPhaseComplete{ false },
	m_bPhaseJudgmentFrame{ false },
	m_pSummonGroupSelector{ DBG_NEW CSummonGroupSelector() }
{
	// 何もなし
}

//====================================================
// 最大フェーズ決定用コンストラクタ
//====================================================
CSummon_Manager_State_Start::CSummon_Manager_State_Start(int nMaxPhase) :
	m_nSummonCoolTimeCnt{ s_nMaxSummonCoolTime},
	m_bSummonCoolTime{ true},
	m_bStartPhase{false},
	m_nSummonCoolTimeFrame{0},
	m_bSuccessPhase{false},
	m_nSummonNum{0},
    m_nPhaseComboCnt{0},
	m_nNumPhase{ 0 },
	m_nMaxPhase{ nMaxPhase },
	m_bAllPhaseComplete{false},
	m_bPhaseJudgmentFrame{false},
	m_pSummonGroupSelector{ DBG_NEW CSummonGroupSelector() }
{
	// 何もなし
}

//====================================================
// デストラクタ
//====================================================
CSummon_Manager_State_Start::~CSummon_Manager_State_Start()
{
	// 召喚グループ選択オブジェクト終了
	if (m_pSummonGroupSelector)
	{
		delete m_pSummonGroupSelector;
		m_pSummonGroupSelector = nullptr;
	}
}

//====================================================
// 更新処理
//====================================================
void CSummon_Manager_State_Start::Update(CSummon_Manager* pManager)
{
	// === 処理を開始する ===

	// 全てのフェーズが終了していたら処理をしない
	if (m_bAllPhaseComplete)
	{
		return;
	}

	if (m_pSummonGroupSelector)
	{
		m_pSummonGroupSelector->Update();
	}
	
	// ランダム召喚（インゲーム）
	RandumSummon();

	// 基底クラス更新
	CSummon_Manager_State::Update(pManager);

	// デバッグ表示
	CRenderer::SetDebugString("ステート変更（V)：召喚開始モード");
	CRenderer::SetDebugString("召喚クールタイム：" + to_string(m_nSummonCoolTimeCnt));
	CRenderer::SetDebugString("召喚マネージャーオブジェクト数：" + to_string(CSummon_Manager::GetNumObj()));
	CRenderer::SetDebugString("現在のフェーズ：" + to_string(m_nNumPhase + 1) + "/" + to_string(m_nMaxPhase));
	CRenderer::SetDebugString("フェーズ連続成功回数：" + to_string(m_nPhaseComboCnt));
}

//====================================================
// フェーズに勝利したかどうか取得
//====================================================
const bool& CSummon_Manager_State_Start::GetSuccessPhase() const
{
	return m_bSuccessPhase;
}

//====================================================
// フェーズに勝利したかどうか設定
//====================================================
void CSummon_Manager_State_Start::SetSuccessPhase(bool bSuccess)
{
	m_bSuccessPhase = bSuccess;
}

//====================================================
// フェーズ可否判定フレームかどうか
//====================================================
const bool& CSummon_Manager_State_Start::GetPhaseJudgmentFrame() const
{
	return m_bPhaseJudgmentFrame;
}

//====================================================
// フェーズを開始したかどうか
//====================================================
const bool& CSummon_Manager_State_Start::GetStartPhase() const
{
	return m_bStartPhase;
}

//====================================================
// 終了かどうか
//====================================================
const bool& CSummon_Manager_State_Start::GetAllPhaseComplete() const
{
	return m_bAllPhaseComplete;
}

//====================================================
// 召喚クールタイムかどうか取得
//====================================================
const bool& CSummon_Manager_State_Start::GetSummonCoolTime() const
{
	return m_bSummonCoolTime;
}

//====================================================
// 召喚グループ選択オブジェクトを取得する
//====================================================
CSummonGroupSelector* CSummon_Manager_State_Start::GetSummonGroupSelector() const
{
	return m_pSummonGroupSelector;
}

//====================================================
// 召喚クールタイム最大フレームを取得
//====================================================
const int& CSummon_Manager_State_Start::GetMaxSummonCoolTime()
{
	return s_nMaxSummonCoolTime;
}

//====================================================
// ランダム召喚処理
//====================================================
bool CSummon_Manager_State_Start::RandumSummon()
{
	// === 処理に必要な情報を宣言、初期化 ===

	bool bSummon = false; // 召喚したかどうか
	int nNumSummonObj = CSummon_Manager::GetNumObj(); // 召喚オブジェクト数
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER));   // プレイヤーへのポインタ取得
	CPlayerState_Returning * pReturning = dynamic_cast<CPlayerState_Returning*>(pPlayer->GetState()); // 戻る状態へのポインタ取得
	m_bPhaseJudgmentFrame = false; // 既に可否判定を可能にしたいわけではないので

	// === ランダム召喚を開始 ===

	// 召喚クールタイムじゃなければ召喚を開始する
	if (!m_bSummonCoolTime)
	{
		// 召喚オブジェクトがなくなり、
		if (nNumSummonObj <= 0)
		{
			// プレイヤーが戻る状態以外なら処理を終了
			if (pReturning == nullptr)
			{
				return false;
			}

			m_nNumPhase++;

			// 全てのフェーズが終了していたらそのフラグをオンにする
			if (m_nNumPhase == m_nMaxPhase)
			{
				m_bAllPhaseComplete = true;
			}

			m_bSummonCoolTime = true; // 召喚クールタイムにする
			m_nSummonCoolTimeCnt = s_nMaxSummonCoolTime; // 召喚クールタイムフレーム数を設定

			// １回でも召喚されていない場合、可否判定を行わない
			if (m_nSummonNum > 0)
			{
				m_bPhaseJudgmentFrame = true; // 今回のフェーズの可否判定を可能にする
			}
			// 今回のフェーズの可否判定をするので、成功したなら難易度スコアを増やす
			if (m_bSuccessPhase)
			{
				int nDifficultyScore = m_pSummonGroupSelector->GetDifficultyScore();
				m_nPhaseComboCnt++; // フェーズ連続成功回数をインクリメント
				m_pSummonGroupSelector->SetDifficultyScore(nDifficultyScore + s_nAddDifficultyScore);
			}
			// 今回のフェーズが失敗したならフェーズ連続成功回数をリセット
			else
			{
				m_nPhaseComboCnt = 0;
			}
		}
	}
	// 召喚クールタイムなら
	else
	{
		if (m_pSummonGroupSelector)
		{
			bSummon = CoolTimeAfterChooseSummon(); // クールタイム終了後に選択グループを召喚
		}
	}

	return bSummon;
}

//====================================================
// 召喚クールタイム後に召喚する
//====================================================
bool CSummon_Manager_State_Start::CoolTimeAfterChooseSummon()
{
	bool bSummon = false; // 召喚に成功したかどうか
	m_bStartPhase = false; // フェーズ開始フラグリセット

	// 召喚クールタイムなら
	if (m_bSummonCoolTime)
	{
		m_nSummonCoolTimeFrame++; // 召喚クールタイムフレーム数をカウントする

		// 召喚クールタイムが０以上なら減らし続ける
		if (m_nSummonCoolTimeCnt > 0)
		{
			m_nSummonCoolTimeCnt--;
		}
		// 召喚クールタイムが０ならクールタイムを解除し召喚
		else
		{
			// 召喚グループ選択がなければ
			if (m_pSummonGroupSelector)
			{
				//pGamePlayer->SetControl(true);
				m_pSummonGroupSelector->DecideSummonGroup(); // 召喚グループ決定
				int nSummonGroup = m_pSummonGroupSelector->GetSummonGroupNum(); // 現在の召喚グループを取得
				bSummon = SetSummonGroup(nSummonGroup); // 召喚開始
			}

			return bSummon;
		}
	}
	
	return false;
}

//====================================================
// 視野妨害イベントを行う
//====================================================
void CSummon_Manager_State_Start::VisionBlockerEvent()
{
	fstream ReadingFile; // ファイル読み込み用
	ReadingFile.open("Data\\JSON\\VisionBlockerPattern_List.json", ios::in);
	CSummon_Manager::DIFFICULTY DifficultyRank = m_pSummonGroupSelector->GetDifficultyRank(); // 難易度ランクを召喚グループ選択から取得

	// ファイルが正常に読み込めていなかったら例外処理
	if (!ReadingFile.good())
	{
		assert(false && "視野妨害パターンリストが正常に読み込めていません");
	}

	JSON PatternList; // 視野妨害パターンリスト
	ReadingFile >> PatternList; // 視野妨害パターンを読み込む

	ReadingFile.close(); // ファイルを閉じる

	// 配列型で読み込むのでちがうなら例外処理
	if (!PatternList.is_array())
	{
		assert(false && "視野妨害パターンリストが配列型じゃありません");
	}

	string PatternFilePass = PatternList[static_cast<int>(DifficultyRank)]; // 今回のパターン

	ReadingFile.open(PatternFilePass, ios::in); // 視野妨害パターンリストの指定したパスのファイルを読み込む

	if (!ReadingFile.good())
	{
		assert(false && "視野妨害パターンが正常に読み込めていません");
	}

	JSON JsonPattern;
	ReadingFile >> JsonPattern; // 視野妨害パターンを読み込む

	ReadingFile.close(); // ファイルを閉じる

	// 読み込んだパターンの全ての要素を検索
	for (auto& Obj : JsonPattern)
	{
		vector<float> vSpawnPos = Obj["SpawnPoint"]; // 召喚位置
		vector<float> vBaseMove = Obj["BaseMove"];   // ベース移動量
		int nSpawnRate = Obj["SpawnRate"]; // 召喚レート
		vector<float> vMaxSpawnRange = Obj["SpawnRange"]["Max"]; // 最大召喚範囲
		vector<float> vMinSpawnRange = Obj["SpawnRange"]["Min"]; // 最小召喚範囲
		float fMaxLeafSpeed = Obj["LeafSpeed"]["Max"]; // 最大葉っぱ速度
		float fMinLeafSpeed = Obj["LeafSpeed"]["Min"]; // 最低葉っぱ速度
		float fMaxLeafSwayAddAngle = Obj["LeafSwayAddAngle"]["Max"]; // 最大葉っぱが揺れる向き加算量
		float fMinLeafSwayAddAngle = Obj["LeafSwayAddAngle"]["Min"]; // 最低葉っぱが揺れる向き加算量
		float fLeafAddRotRange = Obj["LeafAddRotRange"]; // 葉っぱを回転させる向き加算量範囲
		int nLife = Obj["Life"]; // 体力
		string LeafName = Obj["LeafType"]; // 葉っぱの種類名前
		CVisionBlocker::TYPE LeafType = CVisionBlocker::TYPE::LEAF00; // 葉っぱ種類

		// 葉っぱの名前によって種類を決定する
		if (LeafName == "Green")
		{
			LeafType = CVisionBlocker::TYPE::LEAF00;
		}
		else if (LeafName == "Yellow")
		{
			LeafType = CVisionBlocker::TYPE::LEAF01;
		}
		else if (LeafName == "Red")
		{
			LeafType = CVisionBlocker::TYPE::LEAF02;
		}

		// 視野妨害オブジェクトスポナーを生成
		CVisionBlockSpawner::Create(
			D3DXVECTOR3(vSpawnPos[0], vSpawnPos[1], vSpawnPos[2]),
			D3DXVECTOR3(vBaseMove[0], vBaseMove[1], vBaseMove[2]),
			nSpawnRate,
			D3DXVECTOR3(vMaxSpawnRange[0], vMaxSpawnRange[1], vMaxSpawnRange[2]),
			D3DXVECTOR3(vMinSpawnRange[0], vMinSpawnRange[1], vMinSpawnRange[2]),
			fMaxLeafSpeed,
			fMinLeafSpeed,
			fMaxLeafSwayAddAngle,
			fMinLeafSwayAddAngle,
			fLeafAddRotRange,
			nLife,
			LeafType);
	}
}

//====================================================
// グループ召喚処理
//====================================================
bool CSummon_Manager_State_Start::SetSummonGroup(int nSummonGroup)
{
	m_bSuccessPhase = true; // 新しい召喚グループ = フェーズが始まるので成功判定を初期化
	m_nSummonNum++; // 召喚回数をインクリメント
	m_nSummonCoolTimeFrame = 0; // 召喚クールタイムのフレーム数を初期化
	m_bSummonCoolTime = false; // 召喚クールタイムをオフにする
	m_bStartPhase = true; // フェーズ開始
	return CSummon_Manager_State::SetSummonGroup(nSummonGroup);
}
