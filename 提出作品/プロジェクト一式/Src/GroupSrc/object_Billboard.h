//============================================================================
// 
// ビルボードオブジェクト、ヘッダファイル [object_billboard.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_BILLBOARD_H_
#define _OBJECT_BILLBOARD_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

//****************************************************
// ビルボードオブジェクトクラス
//****************************************************
class CObject_Billboard : public CObject
{
public:

	// <special function>
	CObject_Billboard(LAYER Priority = LAYER::DEFAULT);	// コンストラクタ
	~CObject_Billboard() override;						// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
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
	const D3DXCOLOR& GetCol() const;	// 色取得
	void SetCol(D3DXCOLOR Col);			// 色設定

	// <getter/setter>
	const float& GetAlpha() const;	// アルファ値取得
	void SetAlpha(float fAlpha);	// アルファ値設定

	// <getter>
	const float& GetLength() const;	// 展開用対角線取得

	// <getter/setter>
	void SetTexWidth(float fWidth);		// 横テクスチャ分割幅設定
	void SetTexHeight(float fHeight);	// 縦テクスチャ分割幅設定

	// <getter/setter>
	const int& GetNowPatternU() const;		// 現在のテクスチャ横分割幅取得
	void SetNowPatternU(int nNowPatternU);	// 現在のテクスチャ横分割幅設定

	// <getter/setter>
	const int& GetNowPatternV() const;		// 現在のテクスチャ縦分割幅取得
	void SetNowPatternV(int nNowPatternV);	// 現在のテクスチャ縦分割幅設定

	// <static function>
	static CObject_Billboard* Create();	// 生成

private:

	/// <summary> 頂点数 </summary>
	static constexpr WORD NUM_VTX = 4;

	/// <summary> プリミティブ数 </summary>
	static constexpr WORD NUM_PRIM = 2;

	// <function>
	HRESULT CreateVtxBuff();	// 頂点バッファの生成
	void	SetMtxWorld();		// ワールド行列設定

	// <data>
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファのポインタ
	LPDIRECT3DTEXTURE9		m_pTex;			// テクスチャのポインタ
	D3DXVECTOR3				m_Size;			// サイズ
	D3DXVECTOR3				m_Rot;			// 向き
	D3DXVECTOR3				m_Pos;			// 座標
	D3DXCOLOR				m_Col;			// 色
	float					m_fLength;		// 展開用対角線
	float					m_fAngle;		// 対角線用角度
	float					m_fTexWidth;	// 横テクスチャ分割幅
	float					m_fTexHeight;	// 縦テクスチャ分縦幅
	int						m_nNowPatternU;	// 現在の横テクスチャ種類
	int						m_nNowPatternV;	// 現在の縦テクスチャ種類
	D3DXMATRIX				m_MtxWorld;		// ワールド行列
};

#endif // _OBJECT_BILLBOARD_H_