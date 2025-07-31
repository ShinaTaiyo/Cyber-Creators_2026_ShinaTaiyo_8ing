//============================================================================
// 
// ２０２５年６月２６日：ステンシルバッファ実装のためマスク用ポリゴンを描画 [StencilMask.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _STENCILMASK_H_
#define _STENCILMASK_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_2D.h" // オブジェクト2D

//****************************************************
// ステンシルマスククラス
//****************************************************
class CStencilMask : public CObject_2D
{
public:
	// === 特殊関数 ===

	CStencilMask();	// デフォルトコンストラクタ
	~CStencilMask() override;	// デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// === 静的メンバ関数 ===

	static CStencilMask* Create(D3DXVECTOR3 InitPos);	// 生成
	static void SetOtherStencilState(); // 他のオブジェクトがステンシルバッファの設定を行う
	static void SetOtherStencilAvoid(); // ステンシルマスクをしたくないオブジェクトの描画で呼びます
	static void SetOtherResetStencilState(); // 他のオブジェクトがステンシルバッファの設定を戻す
	static void SetUseStencil(bool bUse); // ステンシルバッファを使用するか設定
private:
	// === 静的メンバ変数 ===

	static bool s_bStencil; // ステンシルバッファを使用するかどうか

	// === メンバ関数 ===

	void SetStencilState();   // ステンシルバッファの設定を行う
	void ResetStencilState(); // ステンシルバッファの設定を元に戻す
};

#endif // _PALYER_H_