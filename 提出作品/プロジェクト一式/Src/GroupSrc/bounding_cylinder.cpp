//============================================================================
// 
// ２０２５年４月２８日：コリジョン系クラス [bounding_Cylinder.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "bounding_Cylinder.h"//元ヘッダ

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//============================================================================
// コンストラクタ
//============================================================================
CCollisionInfo_BoundingCylinder::CCollisionInfo_BoundingCylinder() :
	m_Size(VEC3_INIT),
	m_VtxMax(VEC3_INIT),
	m_VtxMin(VEC3_INIT)
{
	SetType(CCollisionInfo::TYPE::CYLINDER); //判定タイプを「シリンダー」に設定
}

//============================================================================
// デストラクタ
//============================================================================
CCollisionInfo_BoundingCylinder::~CCollisionInfo_BoundingCylinder()
{

}

//============================================================================
// 情報更新用（判定前に全ての情報を手動で更新）
//============================================================================
void CCollisionInfo_BoundingCylinder::SetUpdateInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld, D3DXVECTOR3 Size, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin,
	bool bIsLanding, bool bIsLandingOld)
{
	m_Size = Size;     // サイズ
	m_VtxMax = VtxMax; // 最大頂点
	m_VtxMin = VtxMin; // 最小頂点
	CCollisionInfo::SetUpdateInfo(Pos, PosOld, bIsLanding, bIsLandingOld); // 基底クラスの情報更新
}

//============================================================================
// サイズ設定
//============================================================================
void CCollisionInfo_BoundingCylinder::SetSize(const D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// サイズ取得
//============================================================================
const D3DXVECTOR3& CCollisionInfo_BoundingCylinder::GetSize() const
{
	return m_Size;
}

//============================================================================
// 最大頂点設定
//============================================================================
void CCollisionInfo_BoundingCylinder::SetVtxMax(D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//============================================================================
// 最大頂点取得
//============================================================================
const D3DXVECTOR3& CCollisionInfo_BoundingCylinder::GetVtxMax() const
{
	return m_VtxMax;
}

//============================================================================
// 最小頂点設定
//============================================================================
void CCollisionInfo_BoundingCylinder::SetVtxMin(D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//============================================================================
// 最小頂点取得
//============================================================================
const D3DXVECTOR3& CCollisionInfo_BoundingCylinder::GetVtxMin() const
{
	return m_VtxMin;
}