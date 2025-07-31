//============================================================================
// 
// ２０２５年６月１０日：回転円柱とAABBの当たり判定を作成 [RotatingCylinder.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _ROTATINGCYLINDER_H_
#define _ROTATINGCYLINDER_H_ 

//****************************************************
// インクルードファイル
//****************************************************
#include "bounding_cylinder.h" // boudingCylinder

//****************************************************
// 回転円柱当たり判定クラス
//****************************************************
class CRotatingCylinder : public CCollisionInfo_BoundingCylinder
{
public:
	// === 特殊関数 ===

	CRotatingCylinder(); // コンストラクタ
	~CRotatingCylinder() override; // デストラクタ

	// === メンバ関数 ===

	// 向き
	void SetRot(D3DXVECTOR3 Rot);      // 設定
	const D3DXVECTOR3& GetRot() const; // 取得
private:
	D3DXVECTOR3 m_Rot; // 向き
};
#endif // _COLLISION_H_