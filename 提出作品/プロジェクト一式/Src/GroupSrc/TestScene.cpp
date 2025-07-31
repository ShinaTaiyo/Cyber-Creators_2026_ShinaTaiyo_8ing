//============================================================================
// 
// ゲーム [TestScene.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "TestScene.h"
#include "sound.h"

// シーン遷移用
#include "manager.h"
#include "object.h"
#include "title.h"
#include "game.h"
#include "TestScene.h" // チュートリアルプレイヤー

// カメラ取得用
#include "camera_manager.h"

// デバッグ用
#include "renderer.h"

// オブジェクト生成用

#include "player.h"
#include "motion_set.h"		// モーションセット用
#include "space.h"			// 空間制御処理用
#include "swaying_player.h"	// 揺れ再生処理用
#include "calculation.h"
#include "StencilMask.h"

// その他ステート
#include "summon_manager_state.h" // 召喚状態
#include "summon_manager_state_choose.h" // 召喚選択状態
#include "summon_manager_state_start.h" // 召喚開始状態

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
bool CTestScene::s_bTest = false; // テスト配置を利用するかどうか

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CTestScene::Update()
{
	// シーンクラスの更新処理
	CScene::Update();

	/* 仮 */
	if (CManager::GetKeyboard()->GetTrigger(DIK_X))
	{
		// このフレームでの空間描画を許可
		CRenderer::GetRenderer()->SetDrawSpace();
	}

#ifdef _DEBUG	// パラメータ確認用
	// 音を出す
	if (CManager::GetKeyboard()->GetTrigger(DIK_SPACE))
	{
		CSound::GetInstance()->Play(CSound::LABEL::TEXT);
	}

#endif // _DEBUG
	// リザルト画面へ
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
	{
		To_Next();
	}
}

//============================================================================
// 描画処理
//============================================================================
void CTestScene::Draw()
{
	// 基底クラスの描画処理
	CScene::Draw();
}

//============================================================================
// 次のシーンへ
//============================================================================
void CTestScene::To_Next()
{
	// 全オブジェクトの死亡フラグを発動する
	CObject::AllSetRelease();

	// タイトルシーンへ変更
	CScene_Manager::ChangeScene(CTitle::Create());
}

//============================================================================
// ゲームを生成
//============================================================================
CTestScene* CTestScene::Create()
{
	// ゲームを生成
	CTestScene* pNew = DBG_NEW CTestScene();

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
// テスト配置をするかどうか
//============================================================================
bool& CTestScene::GetTest()
{
	return s_bTest;
}

//============================================================================
// オブジェクトの動きを止めるかどうか取得
//============================================================================
//const bool& CTestScene::GetStopObject()
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
CTestScene::CTestScene() :
	CScene{},
	m_bEnd{ false },
	m_pToneAdjust{ nullptr },
	m_pEffectAccumulation{ nullptr },
	m_pSmokeEffect{ nullptr }
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CTestScene::~CTestScene()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CTestScene::Init()
{
	CSummon_Manager* pSummonManager = CSummon_Manager::Create(); // 召喚マネージャー生成
	pSummonManager->SetChengeState(DBG_NEW CSummon_Manager_State_Choose(false)); // 召喚マネージャーのステートを「召喚選択状態」にする
	
	// 今回のカメラを選択（ゲームシーンとほとんど同じなのでゲームのカメラを使用する）
	CCamera_Manager::GetInstance()->SetTransition("Game");

	// ゲームのカメラへのポインタを取得
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game");
	pCamera->SetUseDecayState(false);
	pCamera->SetUpAdjust(0.0f); // 俯瞰度合いをなくす
	m_pToneAdjust = CToneAdjust::Create();
	m_pSmokeEffect = CSmokeEffect::Create();

	CPlayer::Create(D3DXVECTOR3(30.0f, -30.0f, -330.0f)); // チュートリアルプレイヤー生成

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
void CTestScene::Uninit()
{
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game"); // カメラへのポインタ
	pCamera->OverwriteView(nullptr); // デフォルト挙動に戻す
}