//============================================================================
// 
// テクスチャマネージャー [texture_manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "texture_manager.h"

// デバイス取得用
#include "renderer.h"

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CTexture_Manager::CTexture_Manager() :
	m_upRegistry(std::make_unique<CRegistry<LPDIRECT3DTEXTURE9>>())
{}

//============================================================================
// デストラクタ
//============================================================================
CTexture_Manager::~CTexture_Manager()
{
	// 終了処理
	Finalize();
}

//============================================================================
// 初期化処理
//============================================================================
bool CTexture_Manager::Initialize()
{
	// 生成処理用のラムダオブジェクト作成
	std::function<LPDIRECT3DTEXTURE9(const std::string&)> fpCreate = 
		[](const std::string& path) -> LPDIRECT3DTEXTURE9
	{
		// テクスチャのポインタ
		LPDIRECT3DTEXTURE9 pElement = nullptr;

		// デバイスを取得
		auto pDev = CRenderer::GetDeviece();

		// テクスチャを生成
		HRESULT hr = D3DXCreateTextureFromFileA(
			pDev,
			path.c_str(),
			&pElement);

		// 生成失敗
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to Create Texture");
		}

		// テクスチャのポインタを返す
		return pElement;
	};

	// レジストリの初期化処理
	if (!m_upRegistry->Initialize(INITIALIZE_PATH, std::move(fpCreate)))
	{
		return false;
	}

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CTexture_Manager::Finalize()
{
	// 解放処理用のラムダオブジェクト作成
	std::function<void(LPDIRECT3DTEXTURE9&)> fpRelease =
		[](LPDIRECT3DTEXTURE9& element) -> void
	{
		// テクスチャを解放
		if (element)
		{
			element->Release();
			element = nullptr;
		}
	};

	// レジストリの終了処理
	m_upRegistry->Finalize(fpRelease);
}