//============================================================================
// 
// テクスチャオブジェクト、ヘッダファイル [object_Texture.h]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

//****************************************************
// テキストメッシュクラス
//****************************************************
class CObject_Texture : public CObject
{
	//****************************************************
	// 静的メンバ定数の定義
	//****************************************************
	static constexpr WORD NUM_VTX = 4;
	static constexpr WORD NUM_PRIM = 2;

public:

	//****************************************************
	// special funtion
	//****************************************************
	CObject_Texture(LAYER Priority = LAYER::FRONT);	// コンストラクタ
	~CObject_Texture() override;					// デストラクタ

	//****************************************************
	// function
	//****************************************************
	HRESULT	Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	/**
	 * テクスチャオブジェクトを生成し、インスタンスのポインタを返します
	 * 
	 * json -> 「OBJECT」フォルダ内の生成データを渡してください
	 */
	static CObject_Texture* Create(const JSON& json);

	// テクスチャの取得
	inline LPDIRECT3DTEXTURE9& GetTexture () { return m_pTexture; }

	// サーフェイスを取得
	inline LPDIRECT3DSURFACE9& GetSurface() { return m_pSurface; }

	// テクスチャムーブの取得 / 設定
	inline const D3DXVECTOR2& GetTextureMove() const { return m_TextureMove; }
	inline void SetTextureMove(const D3DXVECTOR2& textureMove) { m_TextureMove = textureMove; }

	// メッシュサイズの取得 / 設定
	inline const D3DXVECTOR2& GetMeshSize() const { return m_MeshSize; }
	inline void SetMeshSize(const D3DXVECTOR2& size) { m_MeshSize = size; }

	// 目標メッシュサイズの取得 / 設定
	inline const D3DXVECTOR2& GetMeshSizeTarget() const { return m_MeshSizeTarget; }
	inline void SetMeshSizeTarget(const D3DXVECTOR2& sizeTarget) { m_MeshSizeTarget = sizeTarget; }

	// 向きの取得 / 設定
	inline const D3DXVECTOR3& GetRot() const { return m_Rot; }
	inline void SetRot(const D3DXVECTOR3& rot) { m_Rot = rot; }

	// 目標向きの取得 / 設定
	inline const D3DXVECTOR3& GetRotTarget() const { return m_RotTarget; }
	inline void SetRotTarget(const D3DXVECTOR3& rotTarget) { m_RotTarget = rotTarget; }

	// 座標の取得 / 設定
	inline const D3DXVECTOR3& GetPos() const { return m_Pos; }
	inline void SetPos(const D3DXVECTOR3& pos) { m_Pos = pos; }

	// 目標向きの取得 / 設定
	inline const D3DXVECTOR3& GetPosTarget() const { return m_PosTarget; }
	inline void SetPosTarget(const D3DXVECTOR3& posTarget) { m_PosTarget = posTarget; }

	// 色の取得 / 設定
	inline const D3DXCOLOR& GetCol() const { return m_Col; }
	inline void SetCol(const D3DXCOLOR& col) { m_Col = col; }

	// 目標色の取得 / 設定
	inline const D3DXCOLOR& GetColTarget() const { return m_ColTarget; }
	inline void SetColTarget(const D3DXCOLOR& colTarget) { m_ColTarget = colTarget; }

private:

	//****************************************************
	// funtion
	//****************************************************
	HRESULT CreateVtxBuff();	// 頂点バッファの生成
	HRESULT CreateTex();		// テクスチャの生成
	void	CorrectToTarget();	// 目標値への補間

	//****************************************************
	// data
	//****************************************************
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャのポインタ
	LPDIRECT3DSURFACE9		m_pSurface;			// サーフェイスのポインタ
	D3DXVECTOR2				m_TextureSize;		// テクスチャサイズ
	D3DXVECTOR2				m_TextureMove;		// テクスチャムーブ
	D3DXVECTOR2				m_MeshSize;			// メッシュサイズ
	D3DXVECTOR2				m_MeshSizeTarget;	// 目標メッシュサイズ
	float					m_fDecayRate;		// 減衰割合
	D3DXVECTOR3				m_Rot;				// 向き
	D3DXVECTOR3				m_RotTarget;		// 目標向き
	D3DXVECTOR3				m_Pos;				// 座標
	D3DXVECTOR3				m_PosTarget;		// 目標座標
	D3DXCOLOR				m_Col;				// 色
	D3DXCOLOR				m_ColTarget;		// 目標色
	float					m_fLength;			// 展開用対角線
	float					m_fAngle;			// 対角線用角度
	D3DXMATRIX				m_MtxWorld;			// ワールド行列
};