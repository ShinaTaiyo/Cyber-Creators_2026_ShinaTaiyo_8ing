//============================================================================
// 
// Xモデルマネージャー、ヘッダファイル [X_manager.h]
// Author : 福田歩希
// 
//============================================================================

// インクルードガード
#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "singleton.hpp"

// コンポジション
#include "registry.hpp"

//****************************************************
// Xモデルマネージャークラスの定義
//****************************************************
class CX_Manager final : public CSingleton<CX_Manager>
{
	//****************************************************
	// フレンド宣言
	//****************************************************
	friend struct std::default_delete<CX_Manager>;
	friend CX_Manager* CSingleton<CX_Manager>::GetInstance();

	//****************************************************
	// 静的メンバ変数の定義
	//****************************************************
	static constexpr const char* INITIALIZE_PATH = "Data\\JSON\\INITIALIZE\\X_Manager.json";

public:

	//****************************************************
	// モデル情報の構造体の定義
	//****************************************************
	struct MODEL
	{
		LPD3DXMESH			pMesh;		// メッシュ情報
		D3DXVECTOR3			Size;		// サイズ
		D3DXVECTOR3         VtxMax;     // 最大頂点
		D3DXVECTOR3         VtxMin;     // 最小頂点
		LPD3DXBUFFER		pBuffMat;	// マテリアル情報
		DWORD				dwNumMat;	// マテリアル数
		D3DXCOLOR*			apColMat;	// マテリアル色	
		LPDIRECT3DTEXTURE9* apTex;		// テクスチャ
	};

	//****************************************************
	// function
	//****************************************************

	// レジストリの参照の取得
	inline CRegistry<MODEL*>& RefRegistry() const { return *m_upRegistry; }

private:

	//****************************************************
	// special function
	//****************************************************
	CX_Manager();			// デフォルトコンストラクタ
	~CX_Manager() override;	// デストラクタ

	//****************************************************
	// function
	//****************************************************
	bool Initialize();	// 初期化処理
	void Finalize();	// 終了処理
	
	D3DXVECTOR3 LoadSize(const std::string& path, D3DXVECTOR3& VtxMax, D3DXVECTOR3& VtxMin);	// サイズ読み込み

	//****************************************************
	// data
	//****************************************************
	std::unique_ptr<CRegistry<MODEL*>> m_upRegistry;	// Xモデル用レジストリ

	/* マテリアルテクスチャのキャッシュ */
	std::map<std::string, LPDIRECT3DTEXTURE9> m_mCache;
};