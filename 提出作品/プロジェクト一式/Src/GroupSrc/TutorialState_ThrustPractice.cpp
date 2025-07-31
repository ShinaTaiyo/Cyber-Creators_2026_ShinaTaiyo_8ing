//============================================================================
// 
// ２０２５年７月２２日：チュートリアルを実装する [TutorialState_ThrustPractice.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルード
//****************************************************
#include "TutorialState_ThrustPractice.h"
#include "PlayerState_GuideBeforeThrust.h"
#include "summon_manager_state_start.h"
#include "PlayerState_Returning.h"
#include "TutorialState_HandSwordPractice.h"
#include "summon_manager_state_choose.h"
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
CTutorialState_ThrustPractice::CTutorialState_ThrustPractice(CTutorial* pTutorial) :
	m_nSuccessNum{ 0 },
	m_bStartPhaseOld{false},
	m_bStartPhase{false},
	m_nNextSceneCntDown{s_nInitNextSceneCntDown},
	CTutorialState{ pTutorial }
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CTutorialState_ThrustPractice::~CTutorialState_ThrustPractice()
{
	// 何もなし
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CTutorialState_ThrustPractice::Init(CTutorial* pTutorial)
{
	std::fstream ReadingFile; // ファイル読み込み用

    // === 突き練習ジェイソンファイル読み込み ===

	ReadingFile.open("Data\\JSON\\Tutorial\\ThrustPractice.Json", std::ios::in);

	// ファイルが正常に開けていたら召喚パターンリストを格納
	if (ReadingFile.good())
	{
		ReadingFile >> m_Json; // ジェイソン格納

		// 配列じゃなかったら処理を終了
		if (!m_Json.is_object())
		{
			assert(false && "突き練習状態のジェイソンがオブジェクト型じゃありません");
		}
	}

	ReadingFile.close(); // ファイルを閉じる

	// 召喚マネージャーへのポインタを取得し、召喚開始状態に変える
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));
	pSummonManager->SetChengeState(DBG_NEW CSummon_Manager_State_Start(999));

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER));

	// プレイヤーへのポインタが存在するならプレイヤーを戻る状態にする
	if (pPlayer)
	{
		pPlayer->SetState(DBG_NEW CPlayerState_Returning(pPlayer));
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTutorialState_ThrustPractice::Uninit(CTutorial* pTutorial)
{
	// 何もなし
}

//============================================================================
// 更新処理
//============================================================================
void CTutorialState_ThrustPractice::Update(CTutorial* pTutorial)
{
	// 現在の目的を表示する
	CRenderer::GetRenderer()->GetUiTextManager().Add(
		DBG_NEW CUIText("突きの練習をしてみよう！", WSCREEN_WIDTH / 2, 100,
			50,1, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));

	// 召喚開始状態へのポインタを取得する
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));
	CSummon_Manager_State_Start* pStart = dynamic_cast<CSummon_Manager_State_Start*>(pSummonManager->GetState());
	CRenderer* pRenderer = CRenderer::GetRenderer(); // 描画情報へのポインタを取得
	CUITextManager & pUiTextManager = pRenderer->GetUiTextManager(); // UIテキストマネージャーを取得

	if (pStart)
	{
		if (pStart->GetPhaseJudgmentFrame())
		{
			if (pStart->GetSuccessPhase())
			{
				pUiTextManager.Add(
					DBG_NEW CUIText("すごいです！", WSCREEN_WIDTH / 2, WSCREEN_HEIGHT / 2 - 150,
						100, 100, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));
				m_nSuccessNum++; // 成功回数インクリメント
			}
			else
			{
				pUiTextManager.Add(
					DBG_NEW CUIText("ちがいます・・・", WSCREEN_WIDTH / 2, WSCREEN_HEIGHT / 2 - 150,
						100, 100, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));
			}
		}
	}

	// 突きを繰り出すための文字を表示
	pUiTextManager.Add(
		DBG_NEW CUIText("＜突きを繰り出す＞\nキーボード：J\nコントローラー：Aボタン", WSCREEN_WIDTH / 2, WSCREEN_HEIGHT / 2 + 250,
			50, 1, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));

	// 成功回数UIテキスト表示
	pUiTextManager.Add(
		DBG_NEW CUIText(
			Calculation::Convert::Utf8ToSjis(u8"突き成功回数：" + to_string(m_nSuccessNum) + "/" + to_string(s_nGoalSuccessNum)), 
			WSCREEN_WIDTH / 2 + 400, 150,
			50, 1, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));

	// 目標成功回数に達したらカウントダウンし、ゲームシーンへ移行
	if (m_nSuccessNum == s_nGoalSuccessNum)
	{
		m_nNextSceneCntDown--;

		// 突きのチュートリアル状態に移行します
		if (m_nNextSceneCntDown <= 0)
		{
			pTutorial->SetState(DBG_NEW CTutorialState_HandSwordPractice(pTutorial));
		}
	}
}

//============================================================================
// 
// privateメンバ
//
//============================================================================
