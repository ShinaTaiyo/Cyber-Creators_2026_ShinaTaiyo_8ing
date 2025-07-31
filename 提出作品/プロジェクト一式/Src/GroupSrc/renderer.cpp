//============================================================================
// 
// レンダラー [renderer.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "renderer.h"
#include "manager.h"
#include "scene.h"

// カメラ取得用
#include "camera_manager.h"

// オブジェクト取得
#include "object.h"
#include "object_TextMesh.h"
#include "object_Texture.h"

//****************************************************
// プリプロセッサディレクティブ
//****************************************************
#define ENABLE_STENCIL_BUFFER 1	// ステンシルバッファの有効化
#define ENABLE_ANTI_ALIASING 1	// アンチエイリアスの有効化

//****************************************************
// 無名名前空間を定義
//****************************************************
namespace
{
	// これはテキストメッシュの描画テスト用
	float fSizeCoef = 1.0f;

	// フォグの設定用
	bool 
		DEBUG_UseFog = false;	// デバッグフォグの使用
	float 
		DEBUG_FogStart = 0.0f,	// デバッグフォグの始点
		DEBUG_FogEnd = 0.0f;	// デバッグフォグの終点
	D3DXCOLOR
		DEBUG_COL = { 0.0f, 0.0f, 0.0f, 0.0f };	// デバッグカラー
}

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
CRenderer* CRenderer::m_pRenderer = nullptr;	// レンダラーの本体

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CRenderer::Update()
{
	// 文字列クリア
	m_DebugStr = {};

	// 全オブジェクト更新処理
	CObject::UpdateAll();

	// UIテキスト更新処理
	m_UiTextManager.Update();
#ifdef _DEBUG
#if 1 // テキストサイズの調整
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin(u8"テキストサイズ")) {
		ImGui::DragFloat(u8"サイズ", &fSizeCoef, 0.001f, 0.0f, 100.0f);
		ImGui::End();
	}
#endif

#if 0 // フォグの調整
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin(u8"フォグ"))
	{
		ImGui::Checkbox(u8"操作", &DEBUG_UseFog);

		if (DEBUG_UseFog)
		{
			ImGui::DragFloat(u8"始点", &DEBUG_FogStart, 1.0f, 0.0f);
			m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&DEBUG_FogStart));
			ImGui::DragFloat(u8"終点", &DEBUG_FogEnd, 1.0f, 0.0f);
			m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&DEBUG_FogEnd));
			ImGui::ColorEdit4(u8"カラー", &DEBUG_COL.r);
			m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, DEBUG_COL);
		}
		/* 仮 */
		else
		{
			m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fFogStart));
			m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fFogEnd));
		}
	}
	ImGui::End();
#endif
#endif // _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CRenderer::Draw()
{
	// 画面バッファクリア
	m_pD3DDevice->Clear(0,											// クリアしたい四角形の数を設定 (ビューポート全体の場合は0)
		nullptr,													// 四角形構造体のポインタを設定 (nullptrを渡すことでビューポート全体の範囲)
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),	// クリアするサーフェスを指定する
		D3DCOLOR_RGBA(0, 0, 0, 0),									// このカラーでターゲットをクリア
		1.0f,														// この値に大してデプスバッファをクリア
		10);													    // この値でステンシルバッファをクリア

	// 疑似スクリーンのテクスチャ内へ描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		if (CManager::GetManager()->GetKeyboard()->GetPress(DIK_0))
		{
			CCamera::RefFreeCam()->SetCamera({ 0.0f, 0.0f, 0.0f });
		}
		else if (CManager::GetManager()->GetKeyboard()->GetPress(DIK_9))
		{
			CCamera_Manager::GetInstance()->RefSelectCamera().SetCamera({ 0.0f, 0.0f, 0.0f });
		}
		else
		{
			// カメラをセット
			if (CCamera::GetStateUseFreeCam())
			{
				CCamera::RefFreeCam()->SetCamera({ 0.0f, 0.0f, 0.0f });
			}
			else
			{
				CCamera_Manager::GetInstance()->RefSelectCamera().SetCamera({ 0.0f, 0.0f, 0.0f });
			}
		}

		// 全オブジェクトの描画
		CObject::DrawAll();

		// 全てのUIテキストを描画する
		m_UiTextManager.Draw();

		// シーンの専用描画
		CScene_Manager::GetInstance()->GetScene()->Draw();

#ifdef _DEBUG
		// デバッグ表示
		PrintDebug();

		// ImGuiの描画
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
#endif // _DEBUG

		// 描画終了
		m_pD3DDevice->EndScene();
	}

#if 1 // テキスト描画準備中

	// レンダリングターゲット保持用
	LPDIRECT3DSURFACE9 oldRenderTarget = nullptr;

	// 通常のバックバッファを保持
	m_pD3DDevice->GetRenderTarget(0, &oldRenderTarget);

	// 前面優先度のオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::FRONT);

	while (pObj != nullptr)
	{
		// タイプを取得
		CObject::TYPE Type = pObj->GetType();

		if (Type == CObject::TYPE::TEXTMESH || Type == CObject::TYPE::ONLYTEXT)
		{ // テキストタイプのオブジェクトなら
			
			// テキストメッシュ用の描画
			DrawTextMesh(*pObj);
		}

		pObj = pObj->GetNext();
	}

	if (m_bDrawSpace)
	{
		// 前面優先度のオブジェクトを取得
		CObject* pObj = CObject::GetTopObject(CObject::LAYER::FRONT);

		while (pObj != nullptr)
		{
			// タイプを取得
			CObject::TYPE Type = pObj->GetType();

			if (Type == CObject::TYPE::TEXTURE)
			{
				// テクスチャオブジェクト用の描画
				DrawTexture(*pObj);
			}

			pObj = pObj->GetNext();
		}
		m_bDrawSpace = false;
	}

	// 通常のレンダリングターゲットに戻す
	m_pD3DDevice->SetRenderTarget(0, oldRenderTarget);

	// 保持していたレンダリングターゲットの破棄
	if (oldRenderTarget != nullptr)
	{
		oldRenderTarget->Release();
		oldRenderTarget = nullptr;
	}

#endif

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//============================================================================
// デバッグ表示
//============================================================================
void CRenderer::PrintDebug()
{
	// 表示範囲調整用
	static WORD wAdjustHeight = 0;

	// 表示範囲変動
	if (CManager::GetKeyboard()->GetPress(DIK_NUMPAD2))
	{
		wAdjustHeight > 0 ? wAdjustHeight -= 5 : wAdjustHeight = 0;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_NUMPAD8))
	{
		wAdjustHeight += 5;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_NUMPAD0))
	{
		wAdjustHeight = 0;
	}

	// 表示位置
	RECT Rect = { 0, 0 - wAdjustHeight, WSCREEN_WIDTH, WSCREEN_HEIGHT - wAdjustHeight };

	for (WORD i = 0; i < m_TimeStr.size(); ++i)
	{
		if (m_TimeStr[i].second > 0)
		{
			// 表示時間をカウントダウン
			m_TimeStr[i].second--;

			// 文章をつなげる
			m_DebugStr += m_TimeStr[i].first + "\n";
		}
		else
		{
			// 文章を消去
			m_TimeStr.erase(m_TimeStr.begin() + i);
		}
	}

	// テキストの描画
	m_pDebugFont->DrawText(NULL, m_DebugStr.c_str(), -1, &Rect, DT_TOP | DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//============================================================================
// UIテキストマネージャー取得
//============================================================================
CUITextManager& CRenderer::GetUiTextManager()
{
	return m_UiTextManager;
}

//============================================================================
// 生成
//============================================================================
HRESULT CRenderer::Create(HWND hWnd, BOOL bWindow)
{
	if (m_pRenderer != nullptr)
	{
		assert(false && "レンダラーは既に作成されているか、ダングリングしています");
	}

	// レンダラーを生成
	m_pRenderer = DBG_NEW CRenderer();

	if (m_pRenderer == nullptr)
	{
		assert(false && "レンダラーの生成に失敗");
	}

	// レンダラーの初期設定
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 解放
//============================================================================
void CRenderer::Release()
{
	// レンダラーの破棄
	if (m_pRenderer != nullptr)
	{
		// レンダラー終了処理
		m_pRenderer->Uninit();

		// メモリを解放
		delete m_pRenderer;

		// ポインタを初期化
		m_pRenderer = nullptr;
	}
}

//============================================================================
// フォグの始点を補間
//============================================================================
void CRenderer::CorrectFogStart(float fStart)
{
	m_pRenderer->m_fFogStart += (fStart - m_pRenderer->m_fFogStart) * 0.025f;

	// フォグの始点を設定
	m_pRenderer->m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_pRenderer->m_fFogStart));
}

//============================================================================
// フォグの終点を補間
//============================================================================
void CRenderer::CorrectFogEnd(float fEnd)
{
	m_pRenderer->m_fFogEnd += (fEnd - m_pRenderer->m_fFogEnd) * 0.025f;

	// フォグの終点を設定
	m_pRenderer->m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_pRenderer->m_fFogEnd));
}

//============================================================================
// レンダラーの取得
//============================================================================
CRenderer* CRenderer::GetRenderer()
{
	return m_pRenderer;
}

//============================================================================
// デバイスの取得
//============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDeviece()
{
	return m_pRenderer->m_pD3DDevice;
}

//============================================================================
// デバッグ用文字列に追加
//============================================================================
void CRenderer::SetDebugString(std::string Str)
{
	m_pRenderer->m_DebugStr += Str + "\n";
}

//============================================================================
// 時限式デバッグ文字列に追加
//============================================================================
void CRenderer::SetTimeString(std::string Str, int nCnt)
{
	m_pRenderer->m_TimeStr.push_back({ Str, nCnt });
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CRenderer::CRenderer() :
	m_pD3D{ nullptr },
	m_pD3DDevice{ nullptr },
	m_fFogStart{ FOG_SRART },
	m_fFogEnd{ FOG_END },
	m_pFont{ nullptr },
	m_pDebugFont{ nullptr },
	m_DebugStr{},
	m_TimeStr{},
	m_bDrawSpace(false),
	m_UiTextManager{}
{

}

//============================================================================
// デストラクタ
//============================================================================
CRenderer::~CRenderer()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindiw)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{ // 生成に失敗した場合
		return E_FAIL;
	}

	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{ // 現在のディスプレイモードを取得
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// パラメータのゼロクリア
	d3dpp.BackBufferWidth = WSCREEN_WIDTH;		// ゲームサイズ(幅)
	d3dpp.BackBufferHeight = WSCREEN_HEIGHT;	// ゲームサイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;		// バックバッファの形式
	d3dpp.BackBufferCount = 1;					// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;		// デプスバッファとステンシルバッファを作成
	d3dpp.Windowed = bWindiw;									// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// ステンシルバッファの有効化
#if ENABLE_STENCIL_BUFFER
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// デプスバッファとして24bit、ステンシルバッファとして8bitを使用する
#else
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// デプスバッファとして16bitを使う
#endif

	// アンチエイリアスの有効化
#if ENABLE_ANTI_ALIASING
	d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES; // アンチエイリアスを有効化
#endif

	// Direct3Dデバイスの生成行程①
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		// Direct3Dデバイスの生成行程②
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			// Direct3Dデバイスの生成行程③
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

#if 0
	// 今回のウィンドウ生成に使用したハンドルを保持
	m_D3dpp.hDeviceWindow = hWnd;

	// 今回のプレゼンテーションパラメータを保持
	m_D3dpp = d3dpp;
#endif

	// レンダーステートの初期設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);							// カリングモードの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);							// アルファブレンドの有効化
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);					// ソースブレンドの設定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);				// ターゲットブレンドの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);							// アルファテストの有効化
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x24);									// アルファ参照値
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);						// アルファテスト合格基準
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);							// アルファテストの無効化
	m_pD3DDevice->SetRenderState(D3DRS_STENCILMASK, 0x000000ff);						// ステンシルマスクを設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);								// フォグの有効化
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);					// フォグモードの設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// フォグカラーの設定

	// フォグの範囲設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fFogStart));	// 始点を設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fFogEnd));		// 終点を設定

	// テクスチャステージステートの初期設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#if 0
	// フォントをプロセスに追加
	if (AddFontResourceEx("Data\\FONT\\Hangyaku-xRRwq.ttf", FR_PRIVATE, 0) == 0)
	{
		throw std::runtime_error("Add Font Failed");
	}
#endif

	// フォントを生成
	D3DXCreateFont(m_pD3DDevice,
		0x096,
		0,
		FW_HEAVY,
		4,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"叛逆明朝",
		&m_pFont);

	// デバッグフォントを生成
	D3DXCreateFont(m_pD3DDevice,
		0x16,
		0,
		FW_HEAVY,
		0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&m_pDebugFont);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CRenderer::Uninit()
{
	// 全オブジェクト解放処理
	CObject::ReleaseAll();

	// デバッグフォントの破棄
	if (m_pDebugFont != nullptr)
	{
		m_pDebugFont->Release();
		m_pDebugFont = nullptr;
	}

	// フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}

#if 0
	// フォントをプロセスから除外
	if (RemoveFontResourceEx("Data\\FONT\\Hangyaku-xRRwq.ttf", FR_PRIVATE, 0) == 0)
	{
		throw std::runtime_error("Add Font Failed");
	}
#endif

	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	// 全てのUIテキストをクリアする
	m_UiTextManager.Clear();
}

//============================================================================
// テキストメッシュ用の描画
//============================================================================
void CRenderer::DrawTextMesh(CObject& ref)
{
	// テキストメッシュクラスにダウンキャスト
	CObject_TextMesh& rTextMesh = *hws::DownCast<CObject_TextMesh, CObject>(&ref);

	// レンダリングターゲットにテキストメッシュのサーフェイスを指定
	m_pD3DDevice->SetRenderTarget(0, rTextMesh.GetSurface());

	// 画面バッファクリア
	m_pD3DDevice->Clear(
		0,
		nullptr,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	// テキストサイズをコピー
	D3DXVECTOR2 TextSize = rTextMesh.GetTextSize();

#ifdef _DEBUG
	/* サイズを調整 */
	TextSize *= fSizeCoef;
#endif // _DEBUG

	// テキスト表示範囲
	RECT Rect = { 0, 0, static_cast<LONG>(TextSize.x), static_cast<LONG>(TextSize.y) };

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// テキストメッシュ座標をコピー
		D3DXVECTOR3 Pos = rTextMesh.GetPos();

		// 若干フィールドの外側に
		Pos.x *= 1.35f;
		Pos.z *= 1.35f;

		// カメラをセット
		CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game")->SetCamera({ 0.0f, 0.0f, 0.0f });	// 数値を持った座標 -> ポップアップカメラ

		// メッシュにテキストを描画
		m_pFont->DrawText(NULL, rTextMesh.GetText().c_str(), -1, &Rect, DT_CENTER | DT_VCENTER, D3DCOLOR_RGBA(255, 255, 255, 255));

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// カメラをセット (最後に通常カメラをセットしないとビルボードのマトリックスが変わる)
	CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game")->SetCamera({ 0.0f, 0.0f, 0.0f });	// ゼロ座標 -> 通常カメラ
}

//============================================================================
// テクスチャ用の描画
//============================================================================
void CRenderer::DrawTexture(CObject& ref)
{
	// テクスチャオブジェクトクラスにダウンキャスト
	CObject_Texture& rTexture = *hws::DownCast<CObject_Texture, CObject>(&ref);

	// レンダリングターゲットにテクスチャのサーフェイスを指定
	m_pD3DDevice->SetRenderTarget(0, rTexture.GetSurface());

	// 画面バッファクリア
	m_pD3DDevice->Clear(
		0,
		nullptr,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// カメラをセット
		CCamera_Manager::GetInstance()->RefSelectCamera().SetCamera({ 0.0f, 0.0f, 0.0f });

		// 全オブジェクトの描画
		CObject::DrawAll();

#ifdef _DEBUG
		// デバッグ表示
		//PrintDebug();

		// ImGuiの描画
		//ImGui::Render();
		//ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
#endif // _DEBUG

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// カメラをセット (最後に通常カメラをセットしないとビルボードのマトリックスが変わる)
	CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game")->SetCamera({ 0.0f, 0.0f, 0.0f });	// ゼロ座標 -> 通常カメラ
}