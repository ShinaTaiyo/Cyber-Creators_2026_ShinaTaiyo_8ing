//============================================================================
// 
// ２０２５年４月２８日：コリジョン系クラス [bounding_Cylinder.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _BOUNDING_Cylinder_H_
#define _BOUNDING_Cylinder_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "collision.h" //Collision基底クラスがあるので

//****************************************************
// シリンダー境界当たり判定クラス
//****************************************************
class CCollisionInfo_BoundingCylinder : public CCollisionInfo
{
public:
	CCollisionInfo_BoundingCylinder();                       // コンストラクタ
	~CCollisionInfo_BoundingCylinder() override;             // デストラクタ

	//< Setter>
	void SetUpdateInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld,D3DXVECTOR3 Size, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin,
		bool bIsLanding, bool bIsLandingOld);// 情報更新用（判定前に全ての情報を手動で更新）
	//< Setter/Getter>
	void SetSize(const D3DXVECTOR3 Size);                   // サイズ設定
	const D3DXVECTOR3& GetSize() const;                     // サイズ取得

	//<setter/getter>
	void SetVtxMax(D3DXVECTOR3 VtxMax);                     // 最大頂点設定
	const D3DXVECTOR3& GetVtxMax() const;                   // 最大頂点取得

	//<setter/getter>
	void SetVtxMin(D3DXVECTOR3 VtxMin);                     // 最小頂点設定
	const D3DXVECTOR3& GetVtxMin() const;                   // 最小頂点取得
private:
	D3DXVECTOR3 
		m_Size,   // サイズ
	    m_VtxMax, // 最大頂点
	    m_VtxMin; // 最小頂点
};
#endif // _COLLISION_H_