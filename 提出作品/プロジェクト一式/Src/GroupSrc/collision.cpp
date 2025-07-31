//============================================================================
// 
// コリジョン系関数 [collision.cpp]
// Author : 福田歩希
// 
////============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "collision.h"    // 元ヘッダ
#include "calculation.h"  // 計算用情報
#include "OBB.h"          // OBB情報
#include "bounding_box.h" // 箱判定情報
#include "bounding_cylinder.h" // 円柱情報
#include "RotatingCylinder.h"  // 回転円柱情報
#include "pch.h"
#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

////============================================================================
//// 球同士
////============================================================================
//bool collision::HitSphereToSphere(const CBounding_Sphere* const Self, const CBounding_Sphere* const Other)
//{
//	// お互いの距離を計算
//	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();
//
//	// 距離が、お互いの半径の和以下ならば衝突
//	if (sqrtf(Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z) <= Self->GetRadius() + Other->GetRadius())
//	{
//		return 1;
//	}
//
//	return 0;
//}
//
////============================================================================
//// 球とAABB
////============================================================================
//bool collision::HitSphereToAABB(const CBounding_Sphere* const Self, const CBounding_Box* const Other)
//{
//	// パラメータをコピー
//	const Vec3& SelfPos = Self->GetCenterPos(), OtherPos = Other->GetCenterPos(), OtherSize = Other->GetSize();
//	const float& fSelfRadius = Self->GetRadius();
//
//	// 全ての軸方向への射影同士が重なっていれば衝突
//	if (SelfPos.x + fSelfRadius > OtherPos.x - OtherSize.x &&
//		SelfPos.x - fSelfRadius < OtherPos.x + OtherSize.x &&
//		SelfPos.y + fSelfRadius > OtherPos.y - OtherSize.y &&
//		SelfPos.y - fSelfRadius < OtherPos.y + OtherSize.y &&
//		SelfPos.z + fSelfRadius > OtherPos.z - OtherSize.z &&
//		SelfPos.z - fSelfRadius < OtherPos.z + OtherSize.z)
//	{
//		return 1;
//	}
//
//	return 0;
//}
//
////============================================================================
//// 円柱同士
////============================================================================
//bool collision::HitCylinderToCylinder(const CBounding_Cylinder* const Self, const CBounding_Cylinder* const Other)
//{
//	// お互いの距離を計算
//	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();
//
//	// 距離が、お互いの半径の和より大きければ衝突していない
// 	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius() + Other->GetRadius())
//	{
//		return 0;
//	}
//
//	// Y軸への射影同士が重なっていたら衝突
//	if (Self->GetCenterPos().y + Self->GetHeight() > Other->GetCenterPos().y - Other->GetHeight() &&
//		Self->GetCenterPos().y - Self->GetHeight() < Other->GetCenterPos().y + Other->GetHeight())
//	{
//		return 1;
//	}
//
//	return 0;
//}
//
////============================================================================
//// 円柱と点
////============================================================================
//bool collision::HitCylinderToPoint(const CBounding_Cylinder* const Self, const Vec3& Other)
//{
//	// お互いの距離を計算
//	const Vec3& Distance = Other - Self->GetCenterPos();
//
//	// 距離が、円柱の半径より大きければ衝突していない
//	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius())
//	{
//		return 0;
//	}
//
//	// Y軸への射影に点が存在すれば衝突
//	if (Self->GetCenterPos().y + Self->GetHeight() > Other.y &&
//		Self->GetCenterPos().y - Self->GetHeight() < Other.y)
//	{
//		return 1;
//	}
//
//	return 0;
//}
//
////============================================================================
//// 円柱と球
////============================================================================
//bool collision::HitCylinderToSphere(const CBounding_Cylinder* const Self, const CBounding_Sphere* const Other)
//{
//	// お互いの距離を計算
//	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();
//
//	// 距離が、お互いの半径の和より大きければ衝突していない
//	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius() + Other->GetRadius())
//	{
//		return 0;
//	}
//
//	// Y軸への射影同士が重なっていれば衝突
//	if (Self->GetCenterPos().y + Self->GetHeight() > Other->GetCenterPos().y - Other->GetRadius() &&
//		Self->GetCenterPos().y - Self->GetHeight() < Other->GetCenterPos().y + Other->GetRadius())
//	{
//		return 1;
//	}
//
//	return 0;
//}
//
////============================================================================
//// 円柱と球
////============================================================================
//bool collision::HitCylinderToSphere(const Vec3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const float& fOtherRadius)
//{
//	// お互いの距離を計算
//	const Vec3& Distance = OtherPos - SelfPos;
//
//	// 距離が、お互いの半径の和より大きければ衝突していない
//	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > fSelfRadius + fOtherRadius)
//	{
//		return 0;
//	}
//
//	// Y軸への射影同士が重なっていれば衝突
//	if (SelfPos.y + fSelfHeight > OtherPos.y - fSelfRadius &&
//		SelfPos.y - fSelfHeight < OtherPos.y + fSelfRadius)
//	{
//		return 1;
//	}
//
//	return 0;
//}
//
////============================================================================
//// 円柱とAABB
////============================================================================
//bool collision::HitCylinderToAABB(const CBounding_Cylinder* const Self, const CBounding_Box* const Other)
//{
//	// パラメータをコピー
//	const Vec3& SelfPos = Self->GetCenterPos(), OtherPos = Other->GetCenterPos(), OtherSize = Other->GetSize();
//	const float& fSelfRadius = Self->GetRadius(), fSelfHeight = Self->GetHeight();
//
//	// 全ての軸方向への射影同士が重なっていれば衝突
//	if (SelfPos.x + fSelfRadius > OtherPos.x - OtherSize.x &&
//		SelfPos.x - fSelfRadius < OtherPos.x + OtherSize.x &&
//		SelfPos.y + fSelfHeight > OtherPos.y - OtherSize.y &&
//		SelfPos.y < OtherPos.y + OtherSize.y &&
//		SelfPos.z + fSelfRadius > OtherPos.z - OtherSize.z &&
//		SelfPos.z - fSelfRadius < OtherPos.z + OtherSize.z)
//	{
//		return 1;
//	}
//
//	return 0;
//}
//
////============================================================================
//// 円柱とAABB
////============================================================================
//bool collision::HitCylinderToAABB(const Vec3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const Vec3& OtherSize)
//{
//	// 全ての軸方向への射影同士が重なっていれば衝突
//	if (SelfPos.x + fSelfRadius > OtherPos.x - OtherSize.x &&
//		SelfPos.x - fSelfRadius < OtherPos.x + OtherSize.x &&
//		SelfPos.y + fSelfHeight > OtherPos.y - OtherSize.y &&
//		SelfPos.y - fSelfHeight < OtherPos.y + OtherSize.y &&
//		SelfPos.z + fSelfRadius > OtherPos.z - OtherSize.z &&
//		SelfPos.z - fSelfRadius < OtherPos.z + OtherSize.z)
//	{
//		return 1;
//	}
//
//	return 0;
//}
//
////============================================================================
//// 円柱とAABBの衝突面を取得
////============================================================================
//int collision::GetCylinderToAABB(const Vec3& SelfOldPos, const Vec3& SelfNowPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const Vec3& OtherSize)
//{
//#ifdef _DEBUG
//	// ウィンドウを表示
//	ImGui::SetNextWindowSize({ -1, -1 });
//	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
//	if (ImGui::Begin("[GetCylinderToAABB] UpdateInfo"))
//	{
//		ImGui::Text("OldPos:X %.2f:Y %.2f:Z %.2f", SelfOldPos.x, SelfOldPos.y, SelfOldPos.z);
//		ImGui::Text("NowPos:X %.2f:Y %.2f:Z %.2f", SelfNowPos.x, SelfNowPos.y, SelfNowPos.z);
//		ImGui::Text("SWidth:L %.2f:R %.2f", SelfNowPos.x + fSelfRadius, SelfNowPos.x - fSelfRadius);
//		ImGui::End();
//	}
//#endif // _DEBUG
//
//	// 衝突自体が無ければ
//	if (!HitCylinderToAABB(SelfNowPos, fSelfRadius, fSelfHeight, OtherPos, OtherSize))
//	{
//		return 0;
//	}
//
//	// もともとX軸への射影同士が重なっていたか (計算上Z方向に衝突はしない)
//	if (SelfOldPos.x + fSelfRadius > OtherPos.x - OtherSize.x &&
//		SelfOldPos.x - fSelfRadius < OtherPos.x + OtherSize.x)
//	{ // 重なっていた -> 上下の判定
//
//		// 上にいた
//		if (SelfOldPos.y > OtherPos.y)
//		{
//			return 1;
//		}
//
//		// 下にいた
//		if (SelfOldPos.y < OtherPos.y)
//		{
//			return 2;
//		}
//	}
//	else
//	{ // 重なっていなかった -> 左右の判定
//
//		// 左にいた
//		if (SelfOldPos.x < OtherPos.x)
//		{
//#ifdef _DEBUG
//			CRenderer::SetTimeString("[りれき]衝突面 : 左", 60);
//#endif	// _DEBUG
//			return 3;
//		}
//
//		// 右にいた
//		if (SelfOldPos.x > OtherPos.x)
//		{
//#ifdef _DEBUG
//			CRenderer::SetTimeString("[りれき]衝突面 : 右", 60);
//#endif	// _DEBUG
//			return 4;
//		}
//	}
//
//	return 0;
//}

//******************************************************************************************
// コリジョンストラテジークラス（ストラテジーにすることにより、形に応じた判定が可能）
//******************************************************************************************

//=====================================================
// コンストラクタ
//=====================================================
CCollisionInfo::CCollisionInfo() : 
	m_Pos(VEC3_INIT),
	m_PosOld(VEC3_INIT),
	m_CollisionType(TYPE::NONE),
	m_bIsLanding(false),
	m_bIsLandingOld(false)
{

}

//=====================================================
// デストラクタ
//=====================================================
CCollisionInfo::~CCollisionInfo()
{

}

//=====================================================
// 位置設定
//=====================================================
void CCollisionInfo::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//=====================================================
// 位置取得
//=====================================================
const D3DXVECTOR3& CCollisionInfo::GetPos() const
{
	return m_Pos;
}

//=====================================================
// 1f前の位置設定
//=====================================================
void CCollisionInfo::SetPosOld(D3DXVECTOR3 PosOld)
{
	m_PosOld = PosOld;
}

//=====================================================
// 1f前の位置取得
//=====================================================
const D3DXVECTOR3& CCollisionInfo::GetPosOld() const
{
	return m_PosOld;
}

//=====================================================
// 地面に乗っているかどうか設定
//=====================================================
void CCollisionInfo::SetLanding(bool bIs)
{
	m_bIsLanding = bIs;
}

//=====================================================
// 地面に乗っているかどうか取得
//=====================================================
const bool& CCollisionInfo::GetLanding() const
{
	return m_bIsLanding;
}

//=====================================================
// 1f前に地面に乗っているかどうか設定
//=====================================================
void CCollisionInfo::SetLandingOld(bool bIsOld)
{
	m_bIsLandingOld = bIsOld;
}

//=====================================================
// 1f前に地面に乗っているかどうか取得
//=====================================================
const bool& CCollisionInfo::GetLandingOld() const
{
	return m_bIsLandingOld;
}

//=====================================================
// 判定タイプ取得
//=====================================================
const CCollisionInfo::TYPE& CCollisionInfo::GetType() const
{
	return m_CollisionType;
}

//=====================================================
// 判定タイプ設定
//=====================================================
void CCollisionInfo::SetType(TYPE Type)
{
	m_CollisionType = Type;
}

//=====================================================
// 情報更新用
//=====================================================
void CCollisionInfo::SetUpdateInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld, bool bIsLanding, bool bIsLandingOld)
{
	m_Pos = Pos;               // 位置
	m_PosOld = PosOld;         // 1f前の位置
	m_bIsLanding = bIsLanding; // 地面に乗っているかどうか
	m_bIsLandingOld = bIsLandingOld;// 1f前に地面に乗っているかどうか
}

//******************************************************************************************
// コリジョン関数(namespace)
//******************************************************************************************

//=====================================================
// 箱対箱衝突判定
//=====================================================
bool collision::HitBoxToBox(const CCollisionInfo_BoundingBox* const Self, const CCollisionInfo_BoundingBox* const Other)
{
	const D3DXVECTOR3& 
		SelfPos = Self->GetPos(),            // 自分の位置
	    SelfVtxMax = Self->GetVtxMax(),      // 自分の最大頂点
	    SelfVtxMin = Self->GetVtxMin(),      // 自分の最小頂点
	    OtherPos = Other->GetPos(),          // 相手の位置
	    OtherVtxMax = Other->GetVtxMax(),    // 相手の最大頂点
	    OtherVtxMin = Other->GetVtxMin();    // 相手の最小頂点

	// 判定開始
	if (SelfPos.x + SelfVtxMax.x >= OtherPos.x + OtherVtxMin.x &&
		SelfPos.x + SelfVtxMin.x <= OtherPos.x + OtherVtxMax.x &&
		SelfPos.y + SelfVtxMax.y >= OtherPos.y + OtherVtxMin.y &&
		SelfPos.y + SelfVtxMin.y <= OtherPos.y + OtherVtxMax.y &&
		SelfPos.z + SelfVtxMax.z >= OtherPos.z + OtherVtxMin.z &&
		SelfPos.z + SelfVtxMin.z <= OtherPos.z + OtherVtxMax.z)
	{
		return true;
	}

	return false;
}

//=====================================================
// 箱対箱押し出し判定
//=====================================================
bool collision::PushOutBoxToBox(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other)
{
	// 自分
	const bool& bIsLandingOld = Self->GetLandingOld();      // 1f前に自分自身が地面にいたかどうか
	const bool& bPushXOld = Self->GetPushXOld();            // 1f前にX軸に押し出されていたかどうか
	bool bSuccess = false;                                  // 判定が成功した

	if (bPushXOld)
	{
		CRenderer::SetDebugString("X軸優先");

		if (bIsLandingOld)
		{
			PushOutBoxToBoxFunction::PushY(Self, Other, bSuccess);         // Y軸押し出し
			PushOutBoxToBoxFunction::PushX(Self, Other, bSuccess);         // X軸押し出し
			PushOutBoxToBoxFunction::PushZ(Self, Other, bSuccess);         // Z軸押し出し
		}
		else
		{
			PushOutBoxToBoxFunction::PushX(Self, Other, bSuccess);         // X軸押し出し
			PushOutBoxToBoxFunction::PushZ(Self, Other, bSuccess);         // Z軸押し出し
			PushOutBoxToBoxFunction::PushY(Self, Other, bSuccess);         // Y軸押し出し

		}
	}
	else
	{
		CRenderer::SetDebugString("Z軸優先");
		if (bIsLandingOld)
		{
			PushOutBoxToBoxFunction::PushY(Self, Other, bSuccess);         // Y軸押し出し
			PushOutBoxToBoxFunction::PushZ(Self, Other, bSuccess);         // Z軸押し出し
			PushOutBoxToBoxFunction::PushX(Self, Other, bSuccess);         // X軸押し出し
		}
		else
		{
			PushOutBoxToBoxFunction::PushZ(Self, Other, bSuccess);         // Z軸押し出し
			PushOutBoxToBoxFunction::PushX(Self, Other, bSuccess);         // X軸押し出し
			PushOutBoxToBoxFunction::PushY(Self, Other, bSuccess);         // Y軸押し出し
		}
	}
	return bSuccess;
}

//=====================================================
// 箱対円柱衝突判定
//=====================================================
bool collision::HitBoxToCylinder(const CCollisionInfo_BoundingBox * const Self, const CCollisionInfo_BoundingCylinder* const Other)
{
	// 自分
	const D3DXVECTOR3& SelfPos = Self->GetPos();           // 自分の位置
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax();     // 自分の最大頂点
	const D3DXVECTOR3 SelfVtxMaxPos = SelfPos + SelfVtxMax;// 自分の最大頂点の位置
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin();     // 自分の最小頂点
	const D3DXVECTOR3 SelfVtxMinPos = SelfPos + SelfVtxMin;// 自分の最小頂点の位置

	// 相手
	const D3DXVECTOR3& OtherPos = Other->GetPos();       // 相手の位置
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // 相手の最大頂点
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // 相手の最小頂点
	const D3DXVECTOR3& OtherSize = Other->GetSize();     // 相手のサイズ

	if (SelfPos.y + SelfVtxMax.y < OtherPos.y + OtherVtxMin.y ||
		SelfPos.y + SelfVtxMin.y > OtherPos.y + OtherVtxMax.y)
	{//Y軸で重なっていない
		return false;
	}

	float fOtherRadius = OtherSize.x; //相手の半径を求める

	float fNearX = 0.0f;// 最近点X
	float fNearZ = 0.0f;// 最近点Z

	if (SelfVtxMinPos.x < OtherPos.x && OtherPos.x < SelfVtxMaxPos.x)
	{//相手の中心点が自分のAABBの内側に入っていたら
		fNearX = OtherPos.x;// 最近点は内側に入っているので同じ位置
		CRenderer::SetDebugString("中心点XがAABBの中に");
	}
	else
	{//最近点は自分のMaxかMin
		float fVtxMinLengthX = fabsf(SelfVtxMinPos.x - OtherPos.x);// 自分の最小頂点との距離
		float fVtxMaxLengthX = fabsf(SelfVtxMaxPos.x - OtherPos.x);// 自分の最大頂点との距離

		if (fVtxMinLengthX < fVtxMaxLengthX)
		{//最近点は自分の最小頂点の位置
			fNearX = SelfVtxMinPos.x;
			CRenderer::SetDebugString("最近点XはAABBの最小");
		}
		else
		{//最近点は自分の最大頂点の位置
			fNearX = SelfVtxMaxPos.x;
			CRenderer::SetDebugString("最近点XはAABBの最大");
		}
	}

	if (SelfVtxMinPos.z < OtherPos.z && OtherPos.z < SelfVtxMaxPos.z)
	{//相手の中心点が自分のAABBの内側に入っていたら
		fNearZ = OtherPos.z;// 最近点は内側に入っているので同じ位置
		CRenderer::SetDebugString("中心点ZがAABBの中に");
	}
	else
	{//最近点は自分のMaxかMin
		float fVtxMinLengthZ = fabsf(SelfVtxMinPos.z - OtherPos.z);// 自分の最小頂点との距離
		float fVtxMaxLengthZ = fabsf(SelfVtxMaxPos.z - OtherPos.z);// 自分の最大頂点との距離

		if (fVtxMinLengthZ < fVtxMaxLengthZ)
		{//最近点は自分の最小頂点の位置
			fNearZ = SelfVtxMinPos.z;
			CRenderer::SetDebugString("最近点ZはAABBの最小");
		}
		else
		{//最近点は自分の最大頂点の位置
			fNearZ = SelfVtxMaxPos.z;
			CRenderer::SetDebugString("最近点ZはAABBの最大");
		}
	}

	float fLength = sqrtf(powf(fNearX - OtherPos.x, 2) + powf(fNearZ - OtherPos.z,2)); // 最近点との距離を取る

	if (fLength < fOtherRadius)
	{//最短点との距離が相手の半径よりも短ければ
		return true;
	}

	return false;
}

//=====================================================
// 箱対円柱押し出し判定
//=====================================================
bool collision::PushOutBoxToCylynder(CCollisionInfo_BoundingBox* const Self, const CCollisionInfo_BoundingCylinder* const Other)
{
	// 自分
	const D3DXVECTOR3& SelfPos = Self->GetPos();           // 自分の位置
	const D3DXVECTOR3& SelfPosOld = Self->GetPosOld();     // 自分の1f前の位置
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax();     // 自分の最大頂点
	const D3DXVECTOR3 SelfVtxMaxPos = SelfPos + SelfVtxMax;// 自分の最大頂点の位置
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin();     // 自分の最小頂点
	const D3DXVECTOR3 SelfVtxMinPos = SelfPos + SelfVtxMin;// 自分の最小頂点の位置

	// 相手
	const D3DXVECTOR3& OtherPos = Other->GetPos();       // 相手の位置
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // 相手の最大頂点
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // 相手の最小頂点
	const D3DXVECTOR3& OtherSize = Other->GetSize();     // 相手のサイズ

	float fOtherRadius = OtherSize.x; //相手の半径を求める

	float fNearX = 0.0f;// 最近点X
	float fNearZ = 0.0f;// 最近点Z

	if (SelfVtxMinPos.x < OtherPos.x && OtherPos.x < SelfVtxMaxPos.x)
	{//相手の中心点が自分のAABBの内側に入っていたら
		fNearX = OtherPos.x;// 最近点は内側に入っているので同じ位置
		CRenderer::SetDebugString("中心点XがAABBの中に");
	}
	else
	{//最近点は自分のMaxかMin
		float fVtxMinLengthX = fabsf(SelfVtxMinPos.x - OtherPos.x);// 自分の最小頂点との距離
		float fVtxMaxLengthX = fabsf(SelfVtxMaxPos.x - OtherPos.x);// 自分の最大頂点との距離

		if (fVtxMinLengthX < fVtxMaxLengthX)
		{//最近点は自分の最小頂点の位置
			fNearX = SelfVtxMinPos.x;
			CRenderer::SetDebugString("最近点XはAABBの最小");
		}
		else
		{//最近点は自分の最大頂点の位置
			fNearX = SelfVtxMaxPos.x;
			CRenderer::SetDebugString("最近点XはAABBの最大");
		}
	}

	if (SelfVtxMinPos.z < OtherPos.z && OtherPos.z < SelfVtxMaxPos.z)
	{//相手の中心点が自分のAABBの内側に入っていたら
		fNearZ = OtherPos.z;// 最近点は内側に入っているので同じ位置
		CRenderer::SetDebugString("中心点ZがAABBの中に");
	}
	else
	{//最近点は自分のMaxかMin
		float fVtxMinLengthZ = fabsf(SelfVtxMinPos.z - OtherPos.z);// 自分の最小頂点との距離
		float fVtxMaxLengthZ = fabsf(SelfVtxMaxPos.z - OtherPos.z);// 自分の最大頂点との距離

		if (fVtxMinLengthZ < fVtxMaxLengthZ)
		{//最近点は自分の最小頂点の位置
			fNearZ = SelfVtxMinPos.z;
			CRenderer::SetDebugString("最近点ZはAABBの最小");
		}
		else
		{//最近点は自分の最大頂点の位置
			fNearZ = SelfVtxMaxPos.z;
			CRenderer::SetDebugString("最近点ZはAABBの最大");
		}
	}

	D3DXVECTOR3 ClosestVec = { fNearX - OtherPos.x, 0.0f, fNearZ - OtherPos.z }; // 最近点と円柱の中心との距離
	float fLength = D3DXVec3Length(&ClosestVec);                                 // ベクトルの大きさ
	CRenderer::SetDebugString("距離" + to_string(fLength));                      // 距離デバッグ表示
	bool bSuccess = false;                                                       // 判定が成功したかどうか

	// １：箱と円柱が重なっている→先にY軸押し出し
	// ２：そのうえでY軸で重なっている場合→XZ押し出し

	//１
	if (fLength < fOtherRadius)
	{// XZ平面で当たっていたら

		if (SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y &&
			SelfPosOld.y + SelfVtxMin.y >= OtherPos.y + OtherVtxMax.y)
		{// 上端への押し出し
			Self->SetPos(D3DXVECTOR3(SelfPos.x, OtherPos.y + OtherVtxMax.y - SelfVtxMin.y, SelfPos.z)); // 上方向に押し出す
			Self->SetLanding(true); // 地面に乗っている
			bSuccess = true;
		}
		else if (SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y &&
			SelfPosOld.y + SelfVtxMax.y <= OtherPos.y + OtherVtxMin.y)
		{// 下端への押し出し
			Self->SetPos(D3DXVECTOR3(SelfPos.x, OtherPos.y + OtherVtxMin.y - SelfVtxMax.y, SelfPos.z)); // 下方向に押し出す
			bSuccess = true;
		}
	}

	//２
	if (SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y &&
		SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y)
	{//Y軸で重なっている

		if (fLength < fOtherRadius && fLength > 0.0001f)
		{// 距離が0以上じゃないと押し出さない

			D3DXVECTOR3 PushVec;
			D3DXVec3Normalize(&PushVec, &ClosestVec); // 正規化押し出しベクトル

			float fDiffLength = fOtherRadius - fLength; // 半径と最近点との距離の差分

			D3DXVECTOR3 NewPos = Self->GetPos() + PushVec * fDiffLength; // 上記の処理で求めた差分だけ押し出す
			Self->SetPos(NewPos);                                        // 位置を設定

			bSuccess = true;
		}
	}
	return bSuccess;

}

//=====================================================
// 円柱同士衝突判定
//=====================================================
bool collision::HitCylinderToCylinder(const CCollisionInfo_BoundingCylinder* const Self, const CCollisionInfo_BoundingCylinder* const Other)
{
	const D3DXVECTOR3& SelfPos = Self->GetPos();         // 自分の位置
	const D3DXVECTOR3& SelfSize = Self->GetSize();       // 自分のサイズ
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax();   // 自分の最大頂点
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin();   // 自分の最小頂点

	const D3DXVECTOR3& OtherPos = Other->GetPos();       // 相手の位置
	const D3DXVECTOR3& OtherSize = Other->GetSize();     // 相手のサイズ
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // 相手の最大頂点
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // 相手の最小頂点


	float fLength = sqrtf(powf(OtherPos.x - SelfPos.x, 2) + powf(OtherPos.z - SelfPos.z, 2));// 円柱同士の距離


	if (SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y &&
		SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y)
	{//Y軸が重なっている
		if (fLength < SelfSize.x + OtherSize.x)
		{//二つの円柱の半径の合計より中心点間の距離が短ければ当たっている
			return true;
		}
	}
	return false;
}

//=====================================================
// 円柱同士押し出し判定
//=====================================================
bool collision::PushOutCylinderToCylinder(CCollisionInfo_BoundingCylinder* const Self, CCollisionInfo_BoundingCylinder* const Other)
{
	const D3DXVECTOR3& SelfPos = Self->GetPos();         // 自分の位置
	const D3DXVECTOR3& SelfPosOld = Self->GetPosOld();   // 自分の1f前の位置
	const D3DXVECTOR3& SelfSize = Self->GetSize();       // 自分のサイズ
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax();   // 自分の最大頂点
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin();   // 自分の最小頂点

	const D3DXVECTOR3& OtherPos = Other->GetPos();       // 相手の位置
	const D3DXVECTOR3& OtherSize = Other->GetSize();     // 相手のサイズ
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // 相手の最大頂点
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // 相手の最小頂点


	float fLength = sqrtf(powf(OtherPos.x - SelfPos.x, 2) + powf(OtherPos.z - SelfPos.z, 2));// 円柱同士の距離

	bool bSuccess = false; // 判定が成功したかどうか

	if (fLength < SelfSize.x + OtherSize.x)
	{//二つの円柱の半径の合計より中心点間の距離が短ければ当たっている

		if (SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y &&
			SelfPosOld.y + SelfVtxMin.y >= OtherPos.y + OtherVtxMax.y)
		{// 上端への押し出し
			Self->SetPos(D3DXVECTOR3(SelfPos.x, OtherPos.y + OtherVtxMax.y - SelfVtxMin.y, SelfPos.z)); // 上方向に押し出す
			Self->SetLanding(true); // 地面に乗っている
			bSuccess = true; // 成功
		}
		else if (SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y &&
			SelfPosOld.y + SelfVtxMax.y <= OtherPos.y + OtherVtxMin.y)
		{// 下端への押し出し
			Self->SetPos(D3DXVECTOR3(SelfPos.x, OtherPos.y + OtherVtxMin.y - SelfVtxMax.y, SelfPos.z)); // 下方向に押し出す
			bSuccess = true; // 成功
		}

		// XZ押し出しのみ行う（Y軸が重なっているとき）
		if (SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y &&
			SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y)
		{
			D3DXVECTOR3 PushVecXZ = SelfPos - OtherPos; // 押し出し方向へのベクトル
			PushVecXZ.y = 0.0f; // Y軸は無視

			if (fLength != 0.0f) 
			{
				D3DXVec3Normalize(&PushVecXZ, &PushVecXZ); // 押し出しベクトルを正規化する
				float fPushDist = SelfSize.x + OtherSize.x - fLength; // 半径の和と距離の差分だけ押し出す
				Self->SetPos(SelfPos + PushVecXZ * fPushDist); // 設定
				bSuccess = true;                               // 成功
			}
		}
	}

	return bSuccess;
}

//=====================================================
// OBB対OBB衝突判定
//=====================================================
bool collision::HitOBBToOBB(COBB* const Self, COBB* const Other)
{
	// === 列挙型 ===

	// 軸
	enum class AXIS
	{
		X = 0,
		Y,
		Z,
		MAX
	};

	// === 処理に使用する情報を宣言、初期化 ===

	const D3DXVECTOR3&
		SelfPos = Self->GetPos(),        // 自分自身の位置
		SelfRot = Self->GetRot(),        // 自分自身の向き
		SelfVtxMax = Self->GetVtxMax(),  // 自分自身の最大頂点
		SelfVtxMin = Self->GetVtxMin();  // 自分自身の最小頂点
	D3DXVECTOR3
		SelfBoxCornars[8] = {},   // 自分自身の正方形の「角」を表す
		SelfWorldCornars[8] = {}; // 自分自身の回転後の「角」を表す

	const D3DXVECTOR3&
		OtherPos = Other->GetPos(),        // 相手の位置
		OtherRot = Other->GetRot(),        // 相手の向き       
		OtherVtxMax = Other->GetVtxMax(),  // 相手の最大頂点
		OtherVtxMin = Other->GetVtxMin();  // 相手の最小頂点
	D3DXVECTOR3
		OtherBoxCornars[8] = {},   // 相手の正方形の「角」を表す
		OtherWorldCornars[8] = {}; // 相手の回転後の「角」を表す

	// === 正方形８頂点の位置を求める ===

	// 自身
	SelfBoxCornars[0] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMax.z); // 左上前
	SelfBoxCornars[1] = SelfVtxMax; // 右上前
	SelfBoxCornars[2] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMin.z); // 左上後
	SelfBoxCornars[3] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMax.y, SelfVtxMin.z); // 右上後
	SelfBoxCornars[4] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMin.y, SelfVtxMax.z); // 左下前
	SelfBoxCornars[5] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMax.z); // 右下前
	SelfBoxCornars[6] = SelfVtxMin; // 左下後
	SelfBoxCornars[7] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMin.z); // 右下後

	// 相手
	OtherBoxCornars[0] = D3DXVECTOR3(OtherVtxMin.x, OtherVtxMax.y, OtherVtxMax.z); // 左上前
	OtherBoxCornars[1] = OtherVtxMax; // 右上前
	OtherBoxCornars[2] = D3DXVECTOR3(OtherVtxMin.x, OtherVtxMax.y, OtherVtxMin.z); // 左上後
	OtherBoxCornars[3] = D3DXVECTOR3(OtherVtxMax.x, OtherVtxMax.y, OtherVtxMin.z); // 右上後
	OtherBoxCornars[4] = D3DXVECTOR3(OtherVtxMin.x, OtherVtxMin.y, OtherVtxMax.z); // 左下前
	OtherBoxCornars[5] = D3DXVECTOR3(OtherVtxMax.x, OtherVtxMin.y, OtherVtxMax.z); // 右下前
	OtherBoxCornars[6] = OtherVtxMin; // 左下後
	OtherBoxCornars[7] = D3DXVECTOR3(OtherVtxMax.x, OtherVtxMin.y, OtherVtxMin.z); // 右下後

	// === 自身の回転後の８頂点の位置を求める ===

	D3DXMATRIX SelfRotMatrix; // 自分自身の回転行列
	D3DXMatrixIdentity(&SelfRotMatrix); // 初期化

	// 自身の回転行列を求める
	D3DXMatrixRotationYawPitchRoll(&SelfRotMatrix, SelfRot.y, SelfRot.x, SelfRot.z);

	// 自身の回転行列を用いて回転後の８頂点を求める
	for (int i = 0; i < 8; ++i) {
		D3DXVec3TransformCoord(&SelfWorldCornars[i], &SelfBoxCornars[i], &SelfRotMatrix);
		SelfWorldCornars[i] += SelfPos;
	}

	// === 相手の回転後の８頂点の位置を求める ===

	D3DXMATRIX OtherRotMatrix; // 相手の回転行列
	D3DXMatrixIdentity(&OtherRotMatrix); // 初期化

	// 相手の回転行列を求める
	D3DXMatrixRotationYawPitchRoll(&OtherRotMatrix, OtherRot.y, OtherRot.x, OtherRot.z);

	// 相手の回転行列を用いて回転後の８頂点を求める
	for (int i = 0; i < 8; ++i) {
		D3DXVec3TransformCoord(&OtherWorldCornars[i], &OtherBoxCornars[i], &OtherRotMatrix);
		OtherWorldCornars[i] += OtherPos;
	}


	// ８頂点が表せているか確認用
	{
		//// 回転後の８頂点を表せているかチェック
		//for (int nCnt = 0; nCnt < 8; nCnt++)
		//{
		//	CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 2, 30.0f, 30.0f, SelfWorldCornars[nCnt], D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
		//}
	}

	// === ローカル分離軸を求める ===

	// 基準となる軸
	D3DXVECTOR3 BaseAxis[static_cast<int>(AXIS::MAX)] =
	{ {1.0f,0.0f,0.0f},
	 {0.0f,1.0f,0.0f},
	 {0.0f,0.0f,1.0f}
	};

	// 自身の分離軸
	D3DXVECTOR3 SelfSepAxis[static_cast<int>(AXIS::MAX)];

	// それぞれの分離軸を求める
	for (int nCnt = 0; nCnt < static_cast<int>(AXIS::MAX); nCnt++)
	{
		D3DXVec3TransformCoord(&SelfSepAxis[nCnt], &BaseAxis[nCnt], &SelfRotMatrix);
	}

	// 相手の分離軸
	D3DXVECTOR3 OtherSepAxis[static_cast<int>(AXIS::MAX)];

	// それぞれの分離軸を求める
	for (int nCnt = 0; nCnt < static_cast<int>(AXIS::MAX); nCnt++)
	{
		D3DXVec3TransformCoord(&OtherSepAxis[nCnt], &BaseAxis[nCnt], &OtherRotMatrix);
	}

	// === ローカル分離軸に回転後の頂点を投影し、分離しているかどうかを求める ===

	// 自分のローカル軸に互いの回転後頂点を投影
	for (int nCntAxis = 0; nCntAxis < 3; nCntAxis++)
	{
		// お互いのオブジェクトの投影値の最大最小を初期化
		float
			fSelfMin = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &SelfWorldCornars[0]),
			fSelfMax = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &SelfWorldCornars[0]),

			fOtherMin = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &OtherWorldCornars[0]),
			fOtherMax = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &OtherWorldCornars[0]);

		// 相手の回転後頂点を投影
		for (int nCntVtx = 0; nCntVtx < 8; nCntVtx++)
		{
			// 互いの頂点の投影線分を求める
			float fSelfLength = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &SelfWorldCornars[nCntVtx]);
			float fOtherLength = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &OtherWorldCornars[nCntVtx]);

			// 自身の投影値の最低値を更新
			if (fSelfLength < fSelfMin)
			{
				fSelfMin = fSelfLength;
			}

			// 自身の投影値の最大値を更新
			if (fSelfLength > fSelfMax)
			{
				fSelfMax = fSelfLength;
			}

			// 相手の投影値の最低値を更新
			if (fOtherLength < fOtherMin)
			{
				fOtherMin = fOtherLength;
			}

			// 相手の投影値の最大値を更新
			if (fOtherLength > fOtherMax)
			{
				fOtherMax = fOtherLength;
			}
		}

		// 分離軸が存在しているので処理を終了
		if (fSelfMax < fOtherMin || fOtherMax < fSelfMin)
		{
			return false;
		}
	}

	// 相手のローカル軸に互いの回転後頂点を投影
	for (int nCntAxis = 0; nCntAxis < 3; nCntAxis++)
	{
		// お互いのオブジェクトの投影値の最大最小を初期化
		float
			fSelfMin = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &SelfWorldCornars[0]),
			fSelfMax = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &SelfWorldCornars[0]),

			fOtherMin = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &OtherWorldCornars[0]),
			fOtherMax = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &OtherWorldCornars[0]);

		// 相手の回転後頂点を投影
		for (int nCntVtx = 0; nCntVtx < 8; nCntVtx++)
		{
			// 互いの頂点の投影線分を求める
			float fSelfLength = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &SelfWorldCornars[nCntVtx]);
			float fOtherLength = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &OtherWorldCornars[nCntVtx]);

			// 自身の投影値の最低値を更新
			if (fSelfLength < fSelfMin)
			{
				fSelfMin = fSelfLength;
			}

			// 自身の投影値の最大値を更新
			if (fSelfLength > fSelfMax)
			{
				fSelfMax = fSelfLength;
			}

			// 相手の投影値の最低値を更新
			if (fOtherLength < fOtherMin)
			{
				fOtherMin = fOtherLength;
			}

			// 相手の投影値の最大値を更新
			if (fOtherLength > fOtherMax)
			{
				fOtherMax = fOtherLength;
			}
		}

		// 分離軸が存在しているので処理を終了
		if (fSelfMax < fOtherMin || fOtherMax < fSelfMin)
		{
			return false;
		}
	}

	// === それぞれの分離軸と直行する分離軸と互いの回転後頂点を投影 ===

	D3DXVECTOR3 CrossSepAxis[9] = {}; // 互いの分離軸と直行する分離軸
	int nCntArray = 0; // 配列カウント用

	// 互いの分離軸と直行する分離軸を求める
	for (int nCntSelfSepAxis = 0; nCntSelfSepAxis < 3; nCntSelfSepAxis++)
	{
		for (int nCntOtherSepAxis = 0; nCntOtherSepAxis < 3; nCntOtherSepAxis++)
		{
			D3DXVec3Cross(&CrossSepAxis[nCntArray], &SelfSepAxis[nCntSelfSepAxis], &OtherSepAxis[nCntOtherSepAxis]);
			D3DXVec3Normalize(&CrossSepAxis[nCntArray], &CrossSepAxis[nCntArray]); // 正規化
			nCntArray++;
		}
	}

	// 求めた直行する分離軸に互いの回転後頂点を投影し判定する
	for (int nCntAxis = 0; nCntAxis < 9; nCntAxis++)
	{
		// 長さの２条がほぼ0ならスキップ（0.00001f)
		if (D3DXVec3LengthSq(&CrossSepAxis[nCntAxis]) < 1e-5f)
		{
			continue;
		}

		// お互いのオブジェクトの投影値の最大最小を初期化
		float
			fSelfMin = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &SelfWorldCornars[0]),
			fSelfMax = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &SelfWorldCornars[0]),

			fOtherMin = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &OtherWorldCornars[0]),
			fOtherMax = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &OtherWorldCornars[0]);

		// 相手の回転後頂点を投影
		for (int nCntVtx = 0; nCntVtx < 8; nCntVtx++)
		{
			// 互いの頂点の投影線分を求める
			float fSelfLength = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &SelfWorldCornars[nCntVtx]);
			float fOtherLength = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &OtherWorldCornars[nCntVtx]);


			// 自身の投影値の最低値を更新
			if (fSelfLength < fSelfMin)
			{
				fSelfMin = fSelfLength;
			}

			// 自身の投影値の最大値を更新
			if (fSelfLength > fSelfMax)
			{
				fSelfMax = fSelfLength;
			}

			// 相手の投影値の最低値を更新
			if (fOtherLength < fOtherMin)
			{
				fOtherMin = fOtherLength;
			}

			// 相手の投影値の最大値を更新
			if (fOtherLength > fOtherMax)
			{
				fOtherMax = fOtherLength;
			}
		}

		// 分離軸が存在しているので処理を終了
		if (fSelfMax < fOtherMin || fOtherMax < fSelfMin)
		{
			return false;
		}
	}

	// 分離軸が存在しないので当たっている
	return true;
}

//=====================================================
// AABB対回転円柱衝突判定
//=====================================================
bool collision::HitAABBToRotatingCylinder(const CCollisionInfo_BoundingBox* const Self, const CRotatingCylinder* const Other)
{
	// === 判定に必要な情報を宣言、初期化 ===

	const D3DXVECTOR3&
		SelfPos = Self->GetPos(),       // 自身の位置
		SelfVtxMax = Self->GetVtxMax(), // 自身の最大頂点
		SelfVtxMin = Self->GetVtxMin(); // 自身の最小頂点

	const D3DXVECTOR3&
		OtherPos = Other->GetPos(),       // 相手の位置
		OtherVtxMax = Other->GetVtxMax(), // 相手の最大頂点
		OtherVtxMin = Other->GetVtxMin(), // 相手の最小頂点
		OtherDownPos = D3DXVECTOR3(0.0f, OtherVtxMin.y, 0.0f), // 円柱下側
		OtherUpPos = D3DXVECTOR3(0.0f, OtherVtxMax.y, 0.0f),     // 円柱上側
		OtherRot = Other->GetRot();       // 相手の向き
	float fOtherRadius = OtherVtxMax.x;   // 相手の半径

	D3DXVECTOR3 SelfBoxCornars[8]; // 自分の箱の角
	SelfBoxCornars[0] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMax.z); // 左上前
	SelfBoxCornars[1] = SelfVtxMax; // 右上前
	SelfBoxCornars[2] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMin.z); // 左上後
	SelfBoxCornars[3] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMax.y, SelfVtxMin.z); // 右上後
	SelfBoxCornars[4] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMin.y, SelfVtxMax.z); // 左下前
	SelfBoxCornars[5] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMax.z); // 右下前
	SelfBoxCornars[6] = SelfVtxMin; // 左下後
	SelfBoxCornars[7] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMin.z); // 右下後

	// === 相手の回転行列を求める ===

	D3DXMATRIX MtxRot; // 回転行列
	D3DXMatrixIdentity(&MtxRot); // 回転行列初期化
	D3DXMatrixRotationYawPitchRoll(&MtxRot, OtherRot.y, OtherRot.x, OtherRot.z); // 回転行列を求める

	D3DXMATRIX InvMtxRot;          // 回転行列の逆行列
	D3DXMatrixIdentity(&InvMtxRot); // 回転行列の逆行列を初期化
	D3DXMatrixInverse(&InvMtxRot, NULL, &MtxRot); // 回転行列の逆行列を求める

	// === 相手の回転逆行列を掛けた自身の箱のワールド空間上での角の位置を求める ===

	// ※　回転した円柱とAABBの判定を行う際、円柱が回転している場合、計算が非常に困難です。
	// そこで、回転した円柱の逆行回転行列をAABBに掛けることで、
	// 円柱が回転していない状態でAABBの角だけが傾いている状態になるので、
	// 円柱との判定ロジックを変えずに点が円柱内に入っているかどうかで判定をすることが出来ます。

	D3DXVECTOR3 SelfWorldBoxCornarsToInvRot[8];
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		D3DXVECTOR3 worldPos = SelfBoxCornars[nCnt] + SelfPos;
		D3DXVECTOR3 relativeToCylinder = worldPos - OtherPos;  // InvMtxRotは円柱基準なので、円柱を基準とした相対的な箱の角の位置を求める

		// 上記で求めた相対的な位置に逆行列を掛け、円柱を縦にした状態での点の位置を求める
		D3DXVec3TransformCoord(&SelfWorldBoxCornarsToInvRot[nCnt], &relativeToCylinder, &InvMtxRot); 

		// 高さチェック（Y軸）
		if (SelfWorldBoxCornarsToInvRot[nCnt].y < OtherVtxMin.y ||
			SelfWorldBoxCornarsToInvRot[nCnt].y > OtherVtxMax.y)
		{
			continue;
		}

		// XZ距離チェック
		float fLengthXZ = sqrtf(
			SelfWorldBoxCornarsToInvRot[nCnt].x * SelfWorldBoxCornarsToInvRot[nCnt].x +
			SelfWorldBoxCornarsToInvRot[nCnt].z * SelfWorldBoxCornarsToInvRot[nCnt].z);

		// XZ距離が円柱の半径よりも小さければ当たっている
		if (fLengthXZ <= fOtherRadius)
		{
			return true;
		}
	}

	return false;
}

//=====================================================
// AABB対ワールド空間回転円柱衝突判定
//=====================================================
bool collision::HitAABBToIsWorldRotatingCylinder(const CCollisionInfo_BoundingBox* const Self, const CRotatingCylinder* const Other, 
	D3DXVECTOR3 OtherParentPos, D3DXVECTOR3 OtherParentRot)
{
	// === 判定に必要な情報を宣言、初期化 ===

	const D3DXVECTOR3& SelfPos = Self->GetPos();       // 自身の位置
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax(); // 自身の最大頂点
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin(); // 自身の最小頂点

	const D3DXVECTOR3& OtherPos = Other->GetPos();       // 円柱の位置
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // 円柱の最大頂点
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // 円柱の最小頂点
	const D3DXVECTOR3& OtherRot = Other->GetRot();       // 円柱の回転

	float fOtherRadius = OtherVtxMax.x; // 円柱の半径

	// 箱の８頂点を定義する
	D3DXVECTOR3 SelfBoxCornars[8];
	SelfBoxCornars[0] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMax.z); // 左上前
	SelfBoxCornars[1] = SelfVtxMax; // 右上前
	SelfBoxCornars[2] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMin.z); // 左上後
	SelfBoxCornars[3] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMax.y, SelfVtxMin.z); // 右上後
	SelfBoxCornars[4] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMin.y, SelfVtxMax.z); // 左下前
	SelfBoxCornars[5] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMax.z); // 右下前
	SelfBoxCornars[6] = SelfVtxMin; // 左下後
	SelfBoxCornars[7] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMin.z); // 右下後

	// === 円柱（親を含む）の回転行列 ===

	// 
	D3DXMATRIX 
		mtxParentRot,   // 親の回転行列
		mtxParentTrans, // 親の位置行列
		mtxOtherRot,    // 相手の回転行列
		mtxOtherTrans;  // 相手の位置行列

	// 親の回転行列を求める
	D3DXMatrixRotationYawPitchRoll(&mtxParentRot, OtherParentRot.y, OtherParentRot.x, OtherParentRot.z);

	// 親の位置行列を求める
	D3DXMatrixTranslation(&mtxParentTrans, OtherParentPos.x, OtherParentPos.y, OtherParentPos.z);

	// 相手の回転行列を求める
	D3DXMatrixRotationYawPitchRoll(&mtxOtherRot, OtherRot.y, OtherRot.x, OtherRot.z);

	// 相手の位置行列を求める
	D3DXMatrixTranslation(&mtxOtherTrans, OtherPos.x, OtherPos.y, OtherPos.z);

	// === 円柱のワールド行列(子供の行列を掛け合わせてから親の行列を掛け合わせましょう）===

	D3DXMATRIX mtxOtherWorld = (mtxOtherRot * mtxOtherTrans) * (mtxParentRot * mtxParentTrans);

	// === 円柱の逆行列（AABB側を円柱のローカル空間に戻す）===

	D3DXMATRIX mtxInvOtherWorld;
	D3DXMatrixInverse(&mtxInvOtherWorld, NULL, &mtxOtherWorld);

	// === 自身の8頂点を円柱のローカル空間に変換し、XZ円＋Y範囲で判定 ===

	// ※現状穴あきオブジェクトの穴との判定だけで使うので、
	//今回は一つでも頂点が円柱の中になければ処理を中断します。（完全に円柱の中）
	for (int i = 0; i < 8; ++i)
	{
		D3DXVECTOR3
			worldPos = SelfBoxCornars[i] + SelfPos,        // 自身のワールド座標
		    localToCylinder; // 円柱を基準としたローカル座標

		// 円柱基準の位置に変換し、通常の円柱の判定で出来るようにします。
		D3DXVec3TransformCoord(&localToCylinder, &worldPos, &mtxInvOtherWorld);

		// 高さチェック
		if (localToCylinder.z < OtherVtxMin.z || localToCylinder.z > OtherVtxMax.z)
		{
			return false;
		}

		// XZ平面での距離をチェックし、相手の半径より短ければ当たっている
		float distXZ = sqrtf(localToCylinder.x * localToCylinder.x + localToCylinder.y * localToCylinder.y);
		if (distXZ <= fOtherRadius)
		{
			CRenderer::SetDebugString("当たった！");
		}
		else
		{
			return false;
		}
	}

	return true;
}

//=====================================================
// AABB対ワールド空間回転円柱衝突判定_甘め
//=====================================================
bool collision::HitAABBToIsWorldRotatingCylinder_Sweat(const CCollisionInfo_BoundingBox* const Self, const CRotatingCylinder* const Other, D3DXVECTOR3 OtherParentPos, D3DXVECTOR3 OtherParentRot)
{
	// === 判定に必要な情報を宣言、初期化 ===

	const D3DXVECTOR3& SelfPos = Self->GetPos();       // 自身の位置
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax(); // 自身の最大頂点
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin(); // 自身の最小頂点

	const D3DXVECTOR3& OtherPos = Other->GetPos();       // 円柱の位置
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // 円柱の最大頂点
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // 円柱の最小頂点
	const D3DXVECTOR3& OtherRot = Other->GetRot();       // 円柱の回転

	float fOtherRadius = OtherVtxMax.x; // 円柱の半径

	// 箱の８頂点を定義する
	D3DXVECTOR3 SelfBoxCornars[8];
	SelfBoxCornars[0] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMax.z); // 左上前
	SelfBoxCornars[1] = SelfVtxMax; // 右上前
	SelfBoxCornars[2] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMin.z); // 左上後
	SelfBoxCornars[3] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMax.y, SelfVtxMin.z); // 右上後
	SelfBoxCornars[4] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMin.y, SelfVtxMax.z); // 左下前
	SelfBoxCornars[5] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMax.z); // 右下前
	SelfBoxCornars[6] = SelfVtxMin; // 左下後
	SelfBoxCornars[7] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMin.z); // 右下後

	// === 円柱（親を含む）の回転行列 ===

	// 
	D3DXMATRIX
		mtxParentRot,   // 親の回転行列
		mtxParentTrans, // 親の位置行列
		mtxOtherRot,    // 相手の回転行列
		mtxOtherTrans;  // 相手の位置行列

	// 親の回転行列を求める
	D3DXMatrixRotationYawPitchRoll(&mtxParentRot, OtherParentRot.y, OtherParentRot.x, OtherParentRot.z);

	// 親の位置行列を求める
	D3DXMatrixTranslation(&mtxParentTrans, OtherParentPos.x, OtherParentPos.y, OtherParentPos.z);

	// 相手の回転行列を求める
	D3DXMatrixRotationYawPitchRoll(&mtxOtherRot, OtherRot.y, OtherRot.x, OtherRot.z);

	// 相手の位置行列を求める
	D3DXMatrixTranslation(&mtxOtherTrans, OtherPos.x, OtherPos.y, OtherPos.z);

	// === 円柱のワールド行列(子供の行列を掛け合わせてから親の行列を掛け合わせましょう）===

	D3DXMATRIX mtxOtherWorld = (mtxOtherRot * mtxOtherTrans) * (mtxParentRot * mtxParentTrans);

	// === 円柱の逆行列（AABB側を円柱のローカル空間に戻す）===

	D3DXMATRIX mtxInvOtherWorld;
	D3DXMatrixInverse(&mtxInvOtherWorld, NULL, &mtxOtherWorld);

	// === 自身の8頂点を円柱のローカル空間に変換し、XZ円＋Y範囲で判定 ===

	// １つでも頂点が円柱内に入っていたら成功とする
	for (int i = 0; i < 8; ++i)
	{
		D3DXVECTOR3
			worldPos = SelfBoxCornars[i] + SelfPos,        // 自身のワールド座標
			localToCylinder; // 円柱を基準としたローカル座標

		// 円柱基準の位置に変換し、通常の円柱の判定で出来るようにします。
		D3DXVec3TransformCoord(&localToCylinder, &worldPos, &mtxInvOtherWorld);

		// 高さチェック（どれか一つでも入っていたら判定するのでcontinueで済ます)
		if (localToCylinder.z < OtherVtxMin.z || localToCylinder.z > OtherVtxMax.z)
		{
			continue;
		}

		// XZ平面での距離をチェックし、相手の半径より短ければ当たっている
		float distXZ = sqrtf(localToCylinder.x * localToCylinder.x + localToCylinder.y * localToCylinder.y);
		if (distXZ <= fOtherRadius)
		{
			CRenderer::SetDebugString("当たった！");
			return true;
		}
	}

	return false;
}

//=====================================================
// 箱対箱押し出し判定：X軸
//=====================================================
void collision::PushOutBoxToBoxFunction::PushX(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other, bool& bSuccess)
{
	// 自分
	const D3DXVECTOR3& SelfPos = Self->GetPos();       // 自分の位置
	const D3DXVECTOR3& SelfPosOld = Self->GetPosOld(); // 自分の1f前の位置
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax(); // 自分の最大頂点
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin(); // 自分の最小頂点

	// 相手
	const D3DXVECTOR3& OtherPos = Other->GetPos();       // 相手の位置
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // 相手の最大頂点
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // 相手の最小頂点

	if (SelfPos.x + SelfVtxMax.x > OtherPos.x + OtherVtxMin.x
		&& SelfPosOld.x + SelfVtxMax.x <= OtherPos.x + OtherVtxMin.x
		&& SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y
		&& SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y
		&& SelfPos.z + SelfVtxMax.z > OtherPos.z + OtherVtxMin.z
		&& SelfPos.z + SelfVtxMin.z < OtherPos.z + OtherVtxMax.z)
	{//自分の右端が相手の左端に当たったら
		Self->SetPos(D3DXVECTOR3(OtherPos.x + OtherVtxMin.x - SelfVtxMax.x, SelfPos.y, SelfPos.z));
		Self->SetPushX(true);//X軸に押し出された
		bSuccess = true;// 成功
	}
	else if (SelfPos.x + SelfVtxMin.x < OtherPos.x + OtherVtxMax.x
		&& SelfPosOld.x + SelfVtxMin.x >= OtherPos.x + OtherVtxMax.x
		&& SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y
		&& SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y
		&& SelfPos.z + SelfVtxMax.z > OtherPos.z + OtherVtxMin.z
		&& SelfPos.z + SelfVtxMin.z < OtherPos.z + OtherVtxMax.z)
	{//自分の左端が相手の右端に当たったら
		Self->SetPos(D3DXVECTOR3(OtherPos.x + OtherVtxMax.x - SelfVtxMin.x, SelfPos.y, SelfPos.z));
		Self->SetPushX(true);//X軸に押し出された
		bSuccess = true;// 成功
	}
}

//=====================================================
// 箱対箱押し出し判定：Y軸
//=====================================================
void collision::PushOutBoxToBoxFunction::PushY(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other, bool& bSuccess)
{
	// 自分
	const D3DXVECTOR3& SelfPos = Self->GetPos();       // 自分の位置
	const D3DXVECTOR3& SelfPosOld = Self->GetPosOld(); // 自分の1f前の位置
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax(); // 自分の最大頂点
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin(); // 自分の最小頂点

	// 相手
	const D3DXVECTOR3& OtherPos = Other->GetPos();       // 相手の位置
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // 相手の最大頂点
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // 相手の最小頂点

		//上
	if (SelfPos.x + SelfVtxMax.x > OtherPos.x + OtherVtxMin.x
		&& SelfPos.x + SelfVtxMin.x < OtherPos.x + OtherVtxMax.x
		&& SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y
		&& SelfPosOld.y + SelfVtxMin.y >= OtherPos.y + OtherVtxMax.y
		&& SelfPos.z + SelfVtxMax.z > OtherPos.z + OtherVtxMin.z
		&& SelfPos.z + SelfVtxMin.z < OtherPos.z + OtherVtxMax.z)
	{//自分の下端が相手の上端に当たったら
		Self->SetPos(D3DXVECTOR3(SelfPos.x, OtherPos.y + OtherVtxMax.y - SelfVtxMin.y, SelfPos.z));
		Self->SetLanding(true); //地面にいる
		bSuccess = true;// 成功
	}
	//下
	else if (SelfPos.x + SelfVtxMax.x > OtherPos.x + OtherVtxMin.x
		&& SelfPos.x + SelfVtxMin.x < OtherPos.x + OtherVtxMax.x
		&& SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y
		&& SelfPosOld.y + SelfVtxMax.y <= OtherPos.y + OtherVtxMin.y
		&& SelfPos.z + SelfVtxMax.z > OtherPos.z + OtherVtxMin.z
		&& SelfPos.z + SelfVtxMin.z < OtherPos.z + OtherVtxMax.z)
	{//自分の上端が相手の下端に当たったら
		Self->SetPos(D3DXVECTOR3(SelfPos.x, OtherPos.y + OtherVtxMin.y - SelfVtxMax.y, SelfPos.z));
		bSuccess = true;// 成功
	}
}

//=====================================================
// 箱対箱押し出し判定：Z軸
//=====================================================
void collision::PushOutBoxToBoxFunction::PushZ(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other, bool& bSuccess)
{
	// 自分
	const D3DXVECTOR3& SelfPos = Self->GetPos();       // 自分の位置
	const D3DXVECTOR3& SelfPosOld = Self->GetPosOld(); // 自分の1f前の位置
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax(); // 自分の最大頂点
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin(); // 自分の最小頂点

	// 相手
	const D3DXVECTOR3& OtherPos = Other->GetPos();       // 相手の位置
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // 相手の最大頂点
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // 相手の最小頂点

	if (SelfPos.z + SelfVtxMax.z > OtherPos.z + OtherVtxMin.z
		&& SelfPosOld.z + SelfVtxMax.z <= OtherPos.z + OtherVtxMin.z
		&& SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y
		&& SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y
		&& SelfPos.x + SelfVtxMax.x > OtherPos.x + OtherVtxMin.x
		&& SelfPos.x + SelfVtxMin.x < OtherPos.x + OtherVtxMax.x)
	{//自分の前端が相手の後ろ端に当たったら
		Self->SetPos(D3DXVECTOR3(SelfPos.x, SelfPos.y, OtherPos.z + OtherVtxMin.z - SelfVtxMax.z));
		bSuccess = true;// 成功
	}
	else if (SelfPos.z + SelfVtxMin.z < OtherPos.z + OtherVtxMax.z
		&& SelfPosOld.z + SelfVtxMin.z >= OtherPos.z + OtherVtxMax.z
		&& SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y
		&& SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y
		&& SelfPos.x + SelfVtxMax.x > OtherPos.x + OtherVtxMin.x
		&& SelfPos.x + SelfVtxMin.x < OtherPos.x + OtherVtxMax.x)
	{//自分の後ろ端が相手の前端に当たったら
		Self->SetPos(D3DXVECTOR3(SelfPos.x, SelfPos.y, OtherPos.z + OtherVtxMax.z - SelfVtxMin.z));
		bSuccess = true;// 成功
	}
}
