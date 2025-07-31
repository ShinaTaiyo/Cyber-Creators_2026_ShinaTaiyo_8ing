//============================================================================
// 
// ２０２５年６月９日：OBB判定を実装する [OBB.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _OBB_H_
#define _OBB_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "collision.h" //CollisionStrategy基底クラスがあるので

//****************************************************
// OBB判定クラス
//****************************************************
class COBB : public CCollisionInfo
{
public:
	// === 特殊関数 ===

	COBB(); // コンストラクタ
	~COBB() override; // デストラクタ

	// === メンバ関数 ===

	// 最大頂点
	void SetVtxMax(D3DXVECTOR3 VtxMax);   // 設定
	const D3DXVECTOR3& GetVtxMax() const; // 取得
										  
	// 最小頂点				  
	void SetVtxMin(D3DXVECTOR3 VtxMin);   // 設定
	const D3DXVECTOR3& GetVtxMin() const; // 取得

	// 向き 
	void SetRot(D3DXVECTOR3 Rot);      // 設定
	const D3DXVECTOR3& GetRot() const; // 取得
private:
	// === メンバ変数 ===

	D3DXVECTOR3
		m_VtxMax, // 最大頂点
		m_VtxMin, // 最小頂点
		m_Rot;    // 向き

};
#endif // _COLLISION_H_