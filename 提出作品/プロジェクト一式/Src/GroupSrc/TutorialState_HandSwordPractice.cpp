//============================================================================
// 
// ２０２５年７月２２日：チュートリアルを実装する [TutorialState_HandSwordPractice.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルード
//****************************************************
#include "TutorialState_HandSwordPractice.h"
#include "summon_manager_state_start.h"
#include "PlayerState_Returning.h"
#include "summon_manager_state_choose.h"
#include "PlayerState_HandSwordPractice.h"
#include "tutorial.h"
#include "Pch.h"
#include "calculation.h"
#include "UiText.h"
#include "manager.h"
#include "player.h"
#include "renderer.h"
#include "input.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;
using namespace std;

//****************************************************
// 静的メンバ変数
//****************************************************

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CTutorialState_HandSwordPractice::CTutorialState_HandSwordPractice(CTutorial* pTutorial) :
	m_nCommentNum{0},
	m_nCommentDispFrame{0},
	m_bExecuteHandSword{false},
	CTutorialState{ pTutorial }
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CTutorialState_HandSwordPractice::~CTutorialState_HandSwordPractice()
{
	// 何もなし
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CTutorialState_HandSwordPractice::Init(CTutorial* pTutorial)
{
	std::fstream ReadingFile; // ファイル読み込み用

	// === 突き練習ジェイソンファイル読み込み ===

	ReadingFile.open("Data\\JSON\\Tutorial\\HandSwordPractice.Json", std::ios::in);

	// ファイルが正常に開けていたら召喚パターンリストを格納
	if (ReadingFile.good())
	{
		ReadingFile >> m_Json; // ジェイソン格納

		// 配列じゃなかったら処理を終了
		if (!m_Json.is_array())
		{
			assert(false && "突き練習状態のジェイソンがオブジェクト型じゃありません");
		}
	}

	ReadingFile.close(); // ファイルを閉じる

	// 召喚マネージャーへのポインタを取得し,選択召喚状態に変える
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));
	if (pSummonManager)
	{
		pSummonManager->SetChengeState(DBG_NEW CSummon_Manager_State_Choose(true));
	}

	// プレイヤーへのポインタが存在するならプレイヤーを戻る状態にする
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER));
	if (pPlayer)
	{
		pPlayer->SetState(DBG_NEW CPlayerState_Returning(pPlayer));
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTutorialState_HandSwordPractice::Uninit(CTutorial* pTutorial)
{
	// 何もなし
}

//============================================================================
// 更新処理
//============================================================================
void CTutorialState_HandSwordPractice::Update(CTutorial* pTutorial)
{
	// テキストがないなら処理を終了
	if (!m_Json[m_nCommentNum].contains("Text"))
	{
		return;
	}

	// 説明文字を表示
	string StrComment = m_Json[m_nCommentNum]["Text"];
	CRenderer::GetRenderer()->GetUiTextManager().Add(
		DBG_NEW CUIText(Calculation::Convert::Utf8ToSjis(StrComment), WSCREEN_WIDTH / 2, 100, 50,1,
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));

	// 説明を変える
	ChangeStrExplanation(pTutorial);
}

//============================================================================
// 説明を変える
//============================================================================
void CTutorialState_HandSwordPractice::ChangeStrExplanation(CTutorial* pTutorial)
{
	if (!m_Json[m_nCommentNum].contains("DispFrame") ||
		!m_Json[m_nCommentNum].contains("Stop"))
	{
		return;
	}

	int nDispFrame = m_Json[m_nCommentNum]["DispFrame"]; // 表示フレーム数
	m_bExecuteHandSword = m_Json[m_nCommentNum]["Stop"]; // 止めるかどうか

	// プレイヤーへのポインタが存在するならプレイヤーを戻る状態にする
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER));

	CRenderer* pRenderer = CRenderer::GetRenderer(); // 描画情報へのポインタを取得
	CUITextManager& pUiTextManager = pRenderer->GetUiTextManager(); // UIテキストマネージャーを取得

	// 手刀を発動しないなら
	if (!m_bExecuteHandSword)
	{
		// 表示フレーム数を超えたら次のコメントに進む
		if (m_nCommentDispFrame >= nDispFrame)
		{
			m_nCommentNum++; // 説明文字列番号をインクリメントする

		    // 召喚マネージャーへのポインタを取得し,選択召喚状態に変える
			CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));
			if (!pSummonManager)
			{
				assert(false && "手刀練習状態で召喚マネージャーへのポインタがありません");
				return;
			}

			CSummon_Manager_State_Choose* pChoose =
				dynamic_cast<CSummon_Manager_State_Choose*>(pSummonManager->GetState());

			switch (m_nCommentNum)
			{
				// このコメントになった瞬間に召喚グループを呼ぶ
			case static_cast<int>(COMMENT::KONOMAMAZYA):
				pChoose->SetSummonGroup(static_cast<int>(CSummon_Manager::SUMMONGROUP::PATTERN_2));
				// プレイヤーの状態を「手刀練習」にする
				if (pPlayer)
				{
					pPlayer->SetState(DBG_NEW CPlayerState_HandSwordPractice(pPlayer, 150));
				}
				break;
				case static_cast<int>(COMMENT::EXECUTE):

					// 召喚されたオブジェクトをポーズ状態にする
					CSummon_Manager::StopSummonObj();
				break;
			default:
				break;
			}
			m_nCommentDispFrame = 0; // コメントフレーム数を初期化
		}
		// コメント表示フレーム数をカウントアップする
		m_nCommentDispFrame++;
	}
	// 手刀を発動するなら
	else
	{
		// 突きを繰り出すための文字を表示
		pUiTextManager.Add(
			DBG_NEW CUIText("＜手刀を繰り出す＞\nキーボード：X\nコントローラー：Xボタン", WSCREEN_WIDTH / 2, WSCREEN_HEIGHT / 2 + 250,
				50, 1, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));

		// プレイヤーの手刀練習が終わったら次のコメントへ
		if (CPlayerState_HandSwordPractice::GetEndPractice())
		{
			m_nCommentNum++;
			if (m_nCommentNum >= static_cast<int>(COMMENT::MAX))
			{
				m_nCommentNum = 0; // 説明選択番号を０に戻す
			}
		}
	}

	// 全てのコメントが流れたらシーン遷移する
	if (m_nCommentNum >= static_cast<int>(COMMENT::MAX))
	{
		m_nCommentNum = 0;
		pTutorial->To_Next();
		return; // シーンが変わるので処理を終了
	}
}
