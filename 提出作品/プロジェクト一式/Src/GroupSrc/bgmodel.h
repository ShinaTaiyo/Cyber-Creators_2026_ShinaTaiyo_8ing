//============================================================================
// 
// ２０２５年６月１０日：回転円柱とAABBの当たり判定を実装する [bgmodel.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _BGMODEL_H_
#define _BGMODEL_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"

//****************************************************
// 前方宣言
//****************************************************
class CRotatingCylinder; // 回転円柱情報
class CCollisionInfo; // 判定情報
class CMove_Info; // 移動情報

//****************************************************
// プレイヤークラス
//****************************************************
class CBgModel : public CObject_X
{
public:

	// === 特殊関数 ===

	CBgModel();				// コンストラクタ
	~CBgModel() override;	// デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// === メンバ関数 ===

	// 向き加算量
	void SetAddRot(D3DXVECTOR3 AddRot);   // 設定
	const D3DXVECTOR3& GetAddRot() const; // 取得

	CCollisionInfo* GetCollisionInfo() const; // 判定情報取得
	CMove_Info* GetMoveInfo() const; // 移動情報取得

	// === 静的メンバ関数 ===

	static CBgModel* Create(D3DXVECTOR3 InitPos); // 生成

private:
	// === メンバ変数 ===

	D3DXVECTOR3 m_AddRot; // 加算する向き
	CRotatingCylinder * m_pRotating_Cylinder; // 回転円柱情報 
	CMove_Info* m_pMoveInfo; // 移動情報
};

#endif // _PALYER_H_