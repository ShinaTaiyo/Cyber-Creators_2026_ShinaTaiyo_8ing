//============================================================================
// 
// フォントマネージャー [font_manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "font_manager.h"

// テンプレート定義ファイル
#include "register.tpp"

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
CFont_Manager::CFont_Manager() :
	m_upRegister(std::make_unique<CRegister<LPD3DXFONT>>())
{}

//============================================================================
// デストラクタ
//============================================================================
CFont_Manager::~CFont_Manager()
{
	// 終了処理
	Finalize();
}

//============================================================================
// 初期化処理
//============================================================================
bool CFont_Manager::Initialize()
{
	// 生成処理用のラムダオブジェクト作成
	std::function<LPD3DXFONT(const std::string&)> fpCreate =
		[](const std::string& path) -> LPD3DXFONT
	{
		// フォントのポインタ
		LPD3DXFONT pElement = nullptr;

		// デバイスを取得
		auto pDev = CRenderer::GetDeviece();

		// 必要な追加情報を取得
		const JSON& Json = hws::SafeOpenJsonFile(path);

		// 各パラメータをキャスト
		int
			nHeight = static_cast<int>(Json["Height"]),					// 文字の太さ
			nWidth = static_cast<int>(Json["Width"]),					// 文字の幅
			nMipLevels = static_cast<int>(Json["MipLevels"]);			// ミップマップのレベル
		const std::string& Name = hws::JsonConvertToSJIS(Json["Name"]);	// フォント名

		// フォントを生成
		HRESULT hr = D3DXCreateFont(
			pDev,				// デバイスのポインタ
			nHeight,			// 文字の太さ
			nWidth,				// 文字の幅
			FW_HEAVY,			// 文字の太さ
			nMipLevels,			// ミップマップのレベル
			FALSE,				// イタリックの指定
			SHIFTJIS_CHARSET,	// 文字セットの指定
			OUT_DEFAULT_PRECIS,	// 出力制度
			DEFAULT_QUALITY,	// 出力品質
			DEFAULT_PITCH,		// フォントのピッチ
			Name.c_str(),		// フォント名
			&pElement);			// フォントのポインタ

		// 生成失敗
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to Create Texture");
		}

		// 作成したリソースのポインタを返す
		return pElement;
	};

	// レジスタの初期化処理
	if (!m_upRegister->Initialize(INITIALIZE_PATH, std::move(fpCreate)))
	{
		return false;
	}

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CFont_Manager::Finalize()
{
	// 解放処理用のラムダオブジェクト作成
	std::function<void(LPD3DXFONT&)> fpRelease = 
		[](LPD3DXFONT& element) -> void
	{
		// リソースが存在したら
		if (element)
		{
			// 解放し、ポインタを初期化
			element->Release();
			element = nullptr;
		}
	};

	// レジスタの終了処理
	m_upRegister->Finalize(fpRelease);
}