//============================================================================
// 
// ゲーム [Tutorial.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "tutorial.h"
#include "sound.h"

// シーン遷移用
#include "manager.h"
#include "object.h"
#include "game.h"

// カメラ取得用
#include "camera_manager.h"

// デバッグ用
#include "renderer.h"

// オブジェクト生成用
#include "TutorialPlayer.h" // チュートリアルプレイヤー
#include "obstacle.h"     // 障害物
#include "HoleObstacle.h" // 穴あき障害物
#include "hole.h"     // 穴
#include "zone.h"
#include "number.h"   // 数字
#include "block.h"    // ブロック
#include "target.h"   // ターゲット
#include "logger.h"   // ログ
#include "particle.h" // パーティクル
#include "bgmodel.h"  // 背景モデル
#include "StencilMask.h" // ステンシルマスク
#include "UiText.h" // UIテキスト

#include "motion_set.h"		// モーションセット用
#include "space.h"			// 空間制御処理用
#include "swaying_player.h"	// 揺れ再生処理用
#include "calculation.h"

// その他ステート
#include "summon_manager_state.h" // 召喚状態
#include "summon_manager_state_choose.h" // 召喚選択状態
#include "summon_manager_state_start.h" // 召喚開始状態

// === チュートリアル状態 ===
#include "TutorialState.h"
#include "TutorialState_GamePurpose.h" // ゲーム目的

// カメラ状態
#include "camera_state_targetzoom.h" // カメラズーム状態

// 情報
#include "move_info.h" // 移動情報にアクセスして動きを止めたい
#include "transparent_info.h"

// マネージャー関連
#include "camera.h"  // カメラ
#include "manager.h" // マネージャー
#include "UiTextManager.h" // UIテキストマネージャー

// スポナー
#include "VisionBlockSpawner.h" // 視野妨害物を召喚するオブジェクト

#include <fstream>
//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 静的メンバ変数
//****************************************************

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CTutorial::Update()
{
	// 状態の更新
	if (m_pState)
	{
		m_pState->Update(this);
	}

	// シーンクラスの更新処理
	CScene::Update();

	/* 仮 */
	if (CManager::GetKeyboard()->GetTrigger(DIK_X))
	{
		// このフレームでの空間描画を許可
		CRenderer::GetRenderer()->SetDrawSpace();
	}

	// 強制的にゲームシーンへ
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) ||
		CManager::GetPad()->GetTrigger(CInputPad::JOYKEY::START))
	{
		To_Next();
	}

	m_nCntFrame++; // フレーム数をカウントする
}

//============================================================================
// 描画処理
//============================================================================
void CTutorial::Draw()
{
	// 基底クラスの描画処理
	CScene::Draw();
}

//============================================================================
// 次のシーンへ
//============================================================================
void CTutorial::To_Next()
{
	// 全オブジェクトの死亡フラグを発動する
	CObject::AllSetRelease();

	// リザルトシーンへ変更
	CScene_Manager::ChangeScene(CGame::Create());
}

//============================================================================
// 状態を変更する
//============================================================================
void CTutorial::SetState(CTutorialState* pState)
{
	// 状態へのポインタが存在していたら破棄して新しい状態を設定
	if (m_pState)
	{
		m_pState->Uninit(this); // 終了処理
		delete m_pState;
		m_pState = pState;
	}

	// 状態へのポインタが存在しないので設定する
	m_pState = pState;
	m_pState->Init(this); // 初期化処理
}

//============================================================================
// ゲームを生成
//============================================================================
CTutorial* CTutorial::Create()
{
	// ゲームを生成
	CTutorial* pNew = DBG_NEW CTutorial();

	if (pNew == nullptr)
	{
		assert(false && "ゲームの生成に失敗");
	}

	// ゲームの初期設定
	if (FAILED(pNew->Init()))
	{
		assert(false && "ゲームの初期設定に失敗");
	}

	return pNew;
}

//============================================================================
// オブジェクトの動きを止めるかどうか取得
//============================================================================
//const bool& CTutorial::GetStopObject()
//{
//	return s_bSTOPOBJECT;
//}
//
//============================================================================
// 
// protectedメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CTutorial::CTutorial() :
	CScene{},
	m_bEnd{ false },
	m_nCntFrame{ 0 },
	m_bEditSummon{ true },
	m_pToneAdjust{ nullptr },
	m_pEffectAccumulation{ nullptr },
	m_pSmokeEffect{ nullptr },
	m_pState{nullptr}
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CTutorial::~CTutorial()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CTutorial::Init()
{
	CSummon_Manager* pSummonManager = CSummon_Manager::Create(); // 召喚マネージャー生成
	// テスト配置をしないならチュートリアル状態を生成
	m_pState = DBG_NEW CTutorialState_GamePurpose(this);
	m_pState->Init(this); // 初期化処理

	pSummonManager->SetChengeState(DBG_NEW CSummon_Manager_State_Choose(true)); // 召喚マネージャーのステートを「召喚選択状態」にする

	// 今回のカメラを選択（ゲームシーンとほとんど同じなのでゲームのカメラを使用する）
	CCamera_Manager::GetInstance()->SetTransition("Game");

	// ゲームのカメラへのポインタを取得
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game");
	pCamera->SetUseDecayState(false);
	pCamera->OverwriteView(nullptr);
	pCamera->SetUpAdjust(0.0f); // 俯瞰度合いをなくす
	m_pToneAdjust = CToneAdjust::Create();
	m_pSmokeEffect = CSmokeEffect::Create();

	CTutorialPlayer::Create(D3DXVECTOR3(30.0f, -30.0f, -330.0f)); // チュートリアルプレイヤー生成

	// ステンシルバッファ用のマスクを生成
	CStencilMask::Create(D3DXVECTOR3(FSCREEN_WIDTH / 2, FSCREEN_HEIGHT / 2, 0.0f));

	// === 変数 ===

	m_pEffectAccumulation = CEffectAccumulation::Create();

	CStencilMask::SetUseStencil(false); // ステンシルバッファは一旦使用しません

	// 天球を生成
	const JSON& ParamSkyDome = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Title_SkyDome.json");
	CObject_X::Create(ParamSkyDome);

	// レンダラーのポインタ
	CRenderer* pRenderer = CRenderer::GetRenderer();

	// フォグの始点をセット
	pRenderer->SetFogStart(30000.0f);

	// フォグの終点をセット
	pRenderer->SetFogEnd(60000.0f);

	// フォグのカラーをセット
	pRenderer->SetFogCol({ 0.0f, 0.0f, 0.0f ,0.0f });

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTutorial::Uninit()
{	
	// 状態の終了
	if (m_pState)
	{
		m_pState->Uninit(this); // 終了処理
		delete m_pState;
		m_pState = nullptr;
	}

	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game"); // カメラへのポインタ
	pCamera->OverwriteView(nullptr); // デフォルト挙動に戻す
}