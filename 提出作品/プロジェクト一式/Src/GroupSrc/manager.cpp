//============================================================================
// 
// マネージャー [manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "manager.h"

// 各種シングルトンクラス用
#include "camera_manager.h"
//#include "font_manager.h"
#include "texture_manager.h"
#include "logger.h"

// === シェーダー ===
#include "EffectAccumulation.h"
#include "ToneAdjust.h"
#include "SmokeEffect.h"

/* 旧シングルトン */
#include "renderer.h"
#include "scene.h"
#include "sound.h"
#include "X_manager.h"

//****************************************************
// プリプロセッサディレクティブ
//****************************************************
#define ENABLE_STENCIL_BUFFER 1	// ステンシルバッファの有効化
#define IMGUI_USE_JAPANESE 1	// ImGuiの日本語有効化

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
CManager* CManager::m_pManager = nullptr;	// マネージャーの本体

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CManager::Update()
{
	// Guiの更新を開始
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// レンダラーの更新
	CRenderer::GetRenderer()->Update();

	// ライトの更新
	m_pLight->Update();

	// カメラの更新
	CCamera_Manager::GetInstance()->Update();

#ifdef _DEBUG
	// フリーカメラの更新
	CCamera::UpdateFreeCam();
#endif // _DEBUG

	// シーンの更新
	CScene_Manager::GetInstance()->GetScene()->Update();

	// キーボードの更新
	m_pKeyboard->Update();

	// マウスの更新
	m_pMouse->Update();

	// パッドの更新
	m_pPad->Update();

	// ログ出力
	CLogger::GetInstance()->DisplayLog();

	// Guiの更新を終了
	ImGui::EndFrame();

	// 全オブジェクト後更新処理
	CObject::LateUpdateAll();
}

//============================================================================
// 描画処理
//============================================================================
void CManager::Draw()
{
	// レンダラーの描画
	CRenderer::GetRenderer()->Draw();
}

//============================================================================
// ライトを取得
//============================================================================
CLight* CManager::GetLight() const
{
	return m_pLight;
}

//============================================================================
// 生成
//============================================================================
HRESULT CManager::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pManager != nullptr)
	{
		assert(false && "マネージャーは既に作成されているか、ダングリングしています");
	}

	// マネージャーを生成
	m_pManager = DBG_NEW CManager();

	if (m_pManager == nullptr)
	{
		assert(false && "マネージャーの生成に失敗");
	}

	// マネージャーの初期設定
	if (FAILED(m_pManager->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 解放
//============================================================================
void CManager::Release()
{
	// マネージャーの破棄
	if (m_pManager != nullptr)
	{
		// マネージャーの終了処理
		m_pManager->Uninit();

		// メモリを解放
		delete m_pManager;

		// ポインタを初期化
		m_pManager = nullptr;
	}
}

//============================================================================
// マネージャーを取得
//============================================================================
CManager* CManager::GetManager()
{
	return m_pManager;
}

//============================================================================
// キーボードを取得
//============================================================================
CInputKeyboard* CManager::GetKeyboard()
{
	return m_pManager->m_pKeyboard;
}

//============================================================================
// マウスを取得
//============================================================================
CInputMouse* CManager::GetMouse()
{
	return m_pManager->m_pMouse;
}

//============================================================================
// パッドを取得
//============================================================================
CInputPad* CManager::GetPad()
{
	return m_pManager->m_pPad;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CManager::CManager() :
	m_pKeyboard{ nullptr },
	m_pMouse{ nullptr },
	m_pPad{ nullptr },
	m_pLight{ nullptr }
{

}

//============================================================================
// デストラクタ
//============================================================================
CManager::~CManager()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd)
{
	// レンダラーの生成
	if (FAILED(CRenderer::Create(hWnd, TRUE)))
	{
		return E_FAIL;
	}

	// サウンド初期設定
	if (FAILED(CSound::GetInstance()->Init(hWnd)))
	{
		return E_FAIL;
	}

	// カメラマネージャーの明示的な生成と、初期カメラの選択
	CCamera_Manager::GetInstance()->SetSelectKey("Title");

	// シーンマネージャーの生成 (カメラ・サウンド生成後)
	if (FAILED(CScene_Manager::Create()))
	{
		return E_FAIL;
	}

	// フォントマネージャーの明示的な生成
	//CFont_Manager::GetInstance();

	// テクスチャマネージャーの明示的な生成
	CTexture_Manager::GetInstance();

	// Xモデルマネージャーの明示的な生成
	CX_Manager::GetInstance();

	// ロガーの明示的な生成
	CLogger::GetInstance();

	// ライトの生成
	m_pLight = DBG_NEW CLight();

	if (m_pLight == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// ライトの初期化
	m_pLight->Init();

	// キーボードの生成
	m_pKeyboard = DBG_NEW CInputKeyboard();

	if (m_pKeyboard == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// キーボードの初期化
	m_pKeyboard->Init(hInstance, hWnd);

	// マウスの生成
	m_pMouse = DBG_NEW CInputMouse();

	if (m_pMouse == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// マウスの初期化
	m_pMouse->Init(hInstance, hWnd);

	// パッドの生成
	m_pPad = DBG_NEW CInputPad();

	if (m_pPad == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// パッドの初期化
	m_pPad->Init(hInstance, hWnd);

	// ImGuiの初期設定
	InitImGui(hWnd);

	return S_OK;
}

//============================================================================
// ImGuiの初期設定
//============================================================================
void CManager::InitImGui(HWND hWnd)
{
	// ImGuiのコンテキストを作成
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// ImGuiの入出力設定
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// ImGuiの表示スタイルを設定
	ImGui::StyleColorsDark();

	// バックエンドの初期設定
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(CRenderer::GetDeviece());

	// ImGuiのフォント設定
#if IMGUI_USE_JAPANESE	// ImGuiの日本語有効化
	io.Fonts->AddFontFromFileTTF("Data\\FONT\\meiryo.ttc", 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//ImGui_ImplDX9_InvalidateDeviceObjects();
	//ImGui_ImplDX9_CreateDeviceObjects();
#else
	//io.Fonts->AddFontFromFileTTF("Data\\FONT\\meiryo.ttc", 20.0f, nullptr);
#endif // IMGUI_USE_JAPANESE
}

//============================================================================
// 終了処理
//============================================================================
void CManager::Uninit()
{
	// ImGuiの終了処理
	UninitImGui();

	// パッドの破棄
	if (m_pPad != nullptr)
	{
		m_pPad->Uninit();	// 終了処理
		delete m_pPad;		// メモリを解放
		m_pPad = nullptr;	// ポインタを初期化
	}

	// マウスの破棄
	if (m_pMouse != nullptr)
	{
		m_pMouse->Uninit();	// 終了処理
		delete m_pMouse;	// メモリを解放
		m_pMouse = nullptr;	// ポインタを初期化
	}

	// キーボードの破棄
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();	// 終了処理
		delete m_pKeyboard;		// メモリを解放
		m_pKeyboard = nullptr;	// ポインタを初期化
	}

	// ライトの破棄
	if (m_pLight != nullptr)
	{
		delete m_pLight;		// メモリを解放
		m_pLight = nullptr;		// ポインタを初期化
	}

	// シーンマネージャーの破棄
	CScene_Manager::Release();

	// サウンドの破棄
	CSound::GetInstance()->Release();

	// レンダラーの破棄
	CRenderer::Release();
}

//============================================================================
// ImGuiの終了処理
//============================================================================
void CManager::UninitImGui()
{
	// ImGUiの終了処理
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}