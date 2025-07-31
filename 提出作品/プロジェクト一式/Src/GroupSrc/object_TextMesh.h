//============================================================================
// 
// テキストメッシュオブジェクト、ヘッダファイル [object_TextMesh.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_TEXTMESH_H_
#define _OBJECT_TEXTMESH_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

//****************************************************
// テキストメッシュクラス
//****************************************************
class CObject_TextMesh : public CObject
{
public:

	// <special function>
	CObject_TextMesh(LAYER Priority = LAYER::FRONT);	// コンストラクタ
	~CObject_TextMesh() override;						// デストラクタ

	// <function>
	HRESULT	Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <getter>
	LPDIRECT3DSURFACE9 GetSurface();	// サーフェイスを取得

	// <getter/setter>
	const std::string& GetText() const;	// テキストを取得
	void SetText(std::string Text);		// テキストを設定

	// <getter/setter>
	const std::string& GetTextTarget() const;	// 目標テキストを取得
	void SetTextTarget(std::string Text);		// 目標テキストを設定

	// <timer -> setter>
	void SetTimer(int nTimer);	// タイマーを設定
 
	// <getter/setter>
	const D3DXVECTOR2& GetTextSize() const;	// テキストサイズ取得
	void SetTextSize(D3DXVECTOR2 Size);		// テキストサイズ設定

	// <getter/setter>
	const D3DXVECTOR3& GetMeshSize() const;	// メッシュサイズ取得
	void SetMeshSize(D3DXVECTOR3 Size);		// メッシュサイズ設定

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

	// <static function>
	static CObject_TextMesh* Create(JSON Json);	// 生成

private:

	/// <summary> 頂点数 </summary>
	static constexpr WORD NUM_VTX = 4;

	/// <summary> プリミティブ数 </summary>
	static constexpr WORD NUM_PRIM = 2;

	// <function>
	HRESULT CreateVtxBuff();	// 頂点バッファの生成
	HRESULT CreateTex();		// テクスチャの生成
	void	DisapperByTimer();	// 時間経過による消滅
	void	TextAnimation();	// テキスト送り
	void	SetMtxWorld();		// ワールド行列設定

	// <data>
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファのポインタ
	LPDIRECT3DTEXTURE9		m_pTex;			// テクスチャのポインタ
	LPDIRECT3DSURFACE9		m_pSurface;		// サーフェイスのポインタ
	std::string				m_Text;			// テキスト
	std::string				m_TextTarget;	// 目標テキスト
	int						m_nCntDisp;		// テキスト送りカウント
	int						m_nTextSpeed;	// テキストスピード
	int						m_nTextDelay;	// テキストディレイ
	int						m_nTimer;		// タイマー
	D3DXVECTOR2				m_TextSize;		// テキストサイズ
	D3DXVECTOR3				m_MeshSize;		// メッシュサイズ
	D3DXVECTOR3				m_Rot;			// 向き
	D3DXVECTOR3				m_Pos;			// 座標
	D3DXCOLOR				m_Col;			// 色
	float					m_fLength;		// 展開用対角線
	float					m_fAngle;		// 対角線用角度
	D3DXMATRIX				m_MtxWorld;		// ワールド行列
};

#endif // _OBJECT_TEXTMESH_H_