//============================================================================
// 
// 3Dオブジェクト、ヘッダファイル [object_3D.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_3D_H_
#define _OBJECT_3D_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

//****************************************************
// 3Dオブジェクトクラス
//****************************************************
class CObject_3D : public CObject
{
	//****************************************************
	// 型エイリアスの定義
	//****************************************************
	using BUFF_VTX = LPDIRECT3DVERTEXBUFFER9;
	using BUFF_TEX = LPDIRECT3DTEXTURE9;

public:

	// <special function>
	CObject_3D(LAYER Priority = LAYER::BG);	// コンストラクタ
	~CObject_3D() override;					// デストラクタ

	// <function>
	HRESULT	Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <setter>
	void BindTex(LPDIRECT3DTEXTURE9 pTex);	// テクスチャ割当

	// <getter/setter>
	const D3DXVECTOR3& GetSize() const;	// サイズ取得
	void SetSize(D3DXVECTOR3 Size);		// サイズ設定

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// 向き取得
	void SetRot(D3DXVECTOR3 Rot);		// 向き設定

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// 座標取得
	void SetPos(D3DXVECTOR3 Pos);		// 座標設定

	// <getter/setter>
	const D3DXVECTOR3& GetScale() const; // 拡大率取得
	void SetScale(D3DXVECTOR3 Scale);    // 拡大率設定

	//<getter/setter>
	const D3DXVECTOR3& GetInitSize() const; // 初期サイズ取得
	void SetInitSize(D3DXVECTOR3 Size);     // 初期サイズ設定

	// <getter/setter>
	const D3DXCOLOR& GetCol() const;	// 色取得
	void SetCol(D3DXCOLOR Col);			// 色設定

	// <getter/setter>
	const float& GetAlpha() const;	// アルファ値取得
	void SetAlpha(float fAlpha);	// アルファ値設定

	// <getter>
	const float& GetLength() const;	// 展開用対角線取得

	// <static function>
	static CObject_3D* Create();	// 生成

	// テクスチャアニメーターポインタの参照
	inline useful::up<CTextureAnimator>& RefTextureAnimator() { return m_upTextureAnimator; }

private:

	/// <summary> 頂点数 </summary>
	static constexpr WORD NUM_VTX = 4;

	/// <summary> プリミティブ数 </summary>
	static constexpr WORD NUM_PRIM = 2;

	// <function>
	HRESULT CreateVtxBuff();	// 頂点バッファの生成
	void	SetMtxWorld();		// ワールド行列設定
	void    UpdateSize();       // サイズ更新

	// <data>
	BUFF_VTX	m_pVtxBuff;	// 頂点バッファのポインタ
	BUFF_TEX	m_pTex;		// テクスチャのポインタ
	D3DXVECTOR3	m_Size;		// サイズ
	D3DXVECTOR3 m_InitSize;	// 初期サイズ
	D3DXVECTOR3	m_Rot;		// 向き
	D3DXVECTOR3	m_Pos;		// 座標
	D3DXVECTOR3 m_Scale;	// 拡大率
	D3DXCOLOR	m_Col;		// 色
	float		m_fLength;	// 展開用対角線
	float		m_fAngle;	// 対角線用角度
	D3DXMATRIX	m_MtxWorld;	// ワールド行列

	// テクスチャアニメーター合成先
	std::unique_ptr<CTextureAnimator> m_upTextureAnimator;
};

#endif // _OBJECT_3D_H_