//============================================================================
// 
// コリジョン系関数、ヘッダファイル [collision.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _COLLISION_H_
#define _COLLISION_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************

//****************************************************
// 前方宣言
//****************************************************
class CCollisionInfo_BoundingBox;        // 箱判定情報
class CCollisionInfo_BoundingCylinder;   // 円柱判定情報
class COBB; // OBB判定情報
class CRotatingCylinder; // 回転円柱判定情報

//****************************************************
// コリジョン情報基底クラスを定義
//****************************************************
class CCollisionInfo
{
public:

	//判定種類
	enum class TYPE
	{
		NONE = 0, // なし
		BOX,      // 箱
		CYLINDER, // 円柱
		OBB,      // OBB
		MAX       // 最大
	};

	CCollisionInfo();                 // コンストラクタ
	virtual ~CCollisionInfo();        // デストラクタ

	//< Setter/Getter>
	void SetPos(D3DXVECTOR3 Pos);         // 位置設定
	const D3DXVECTOR3& GetPos() const;    // 位置取得

	//< Setter/Getter>
	void SetPosOld(D3DXVECTOR3 PosOld);   // 1f前の位置設定
	const D3DXVECTOR3& GetPosOld() const; // 1f前の位置取得

	//< Setter/Getter>
	void SetLanding(bool bIs);            // 地面に乗っているかどうか設定
	const bool& GetLanding() const;       // 地面に乗っているかどうか取得

	//< Setter/Getter>                    
	void SetLandingOld(bool bIsOld);     // 1f前に地面に乗っているかどうか設定
	const bool& GetLandingOld() const;   // 1f前に地面に乗っているかどうか取得

	//<setter/getter>


	//< Getter>
	const TYPE& GetType() const;          // 判定タイプ取得

	//< Setter>
	void SetUpdateInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld,bool bIsLanding,bool bIsLandingOld);// 情報更新用（判定前に全ての情報を手動で更新）

protected:
	//< Setter>
	void SetType(TYPE Type);              // 判定タイプ設定
private:
	D3DXVECTOR3 m_Pos;     // 位置
	D3DXVECTOR3 m_PosOld;  // 1f前の位置
	TYPE m_CollisionType;  // 判定タイプ
	bool m_bIsLanding;     // 地面に乗っているかどうか
	bool m_bIsLandingOld;  // 1f前に地面に乗っているかどうか
};

//****************************************************
// 名前空間collisionを定義
//****************************************************
namespace collision
{
	//****************************************************
	// usingディレクティブ
	//****************************************************
	using namespace useful;

	namespace PushOutBoxToBoxFunction
	{
		void PushX(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other,bool & bSuccess);
		void PushY(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other,bool & bSuccess);
		void PushZ(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other,bool & bSuccess);
	}

	//// 球同士
	//bool HitSphereToSphere(const CBounding_Sphere* const Self, const CBounding_Sphere* const Other);

	//// 球とAABB
	//bool HitSphereToAABB(const CBounding_Sphere* const Self, const CBounding_Box* const Other);

	//// 円柱同士
	//bool HitCylinderToCylinder(const CBounding_Cylinder* const Self, const CBounding_Cylinder* const Other);

	//// 円柱と点
	//bool HitCylinderToPoint(const CBounding_Cylinder* const Self, const Vec3& Other);

	//// 円柱と球
	//bool HitCylinderToSphere(const CBounding_Cylinder* const Self, const CBounding_Sphere* const Other);
	//bool HitCylinderToSphere(const Vec3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const float& fOtherRadius);

	//// 円柱とAABB
	//bool HitCylinderToAABB(const CBounding_Cylinder* const Self, const CBounding_Box* const Other);
	//bool HitCylinderToAABB(const Vec3& SelfPos, const float& fSelfRadius, const float& fHeight, const Vec3& PosTarget, const Vec3& SizeTarget);

	// 円柱とAABBの衝突面を取得
	//int GetCylinderToAABB(const Vec3& SelfOldPos, const Vec3& SelfNowPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const Vec3& OtherSize);

	// 箱同士衝突判定「const ○○ const」は、指す値もポインタも両方変更不可
	bool HitBoxToBox(const CCollisionInfo_BoundingBox* const Self, const CCollisionInfo_BoundingBox* const Other);

	// 箱同士押し出し判定「const ○○ const」は、指す値もポインタも両方変更不可
	bool PushOutBoxToBox(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other);

	// 箱対円柱衝突判定
	bool HitBoxToCylinder(const CCollisionInfo_BoundingBox* const  Self,const CCollisionInfo_BoundingCylinder* const Other);

	// 箱対円柱押し出し判定
	bool PushOutBoxToCylynder(CCollisionInfo_BoundingBox* const Self, const CCollisionInfo_BoundingCylinder* const Other);

	// 円柱同士衝突判定
	bool HitCylinderToCylinder(const CCollisionInfo_BoundingCylinder* const Self, const CCollisionInfo_BoundingCylinder* const Other);

	// 円柱同士押し出し判定
	bool PushOutCylinderToCylinder(CCollisionInfo_BoundingCylinder* const Self,CCollisionInfo_BoundingCylinder* const Other);

	// OBB同士衝突判定
	bool HitOBBToOBB(COBB* const Self, COBB* const Other);

	// AABBと回転Cylinder衝突判定
	bool HitAABBToRotatingCylinder(const CCollisionInfo_BoundingBox * const Self,const CRotatingCylinder* const Other );

	// AABBとワールド空間回転シリンダー衝突判定
	bool HitAABBToIsWorldRotatingCylinder(const CCollisionInfo_BoundingBox* const Self, const CRotatingCylinder* const Other,
		D3DXVECTOR3 OtherParentPos,D3DXVECTOR3 OtherParentRot);

	// AABBとワールド回転シリンダー衝突判定_甘め
	bool HitAABBToIsWorldRotatingCylinder_Sweat(const CCollisionInfo_BoundingBox* const Self, const CRotatingCylinder* const Other,
		D3DXVECTOR3 OtherParentPos, D3DXVECTOR3 OtherParentRot);
	//======================
	//オブジェクトX
	//======================
	//namespace ObjectX
	//{
	//	bool HitSquareToAABB(CObject_X* pMyObj, CObject_X* pComObj);//AABB当たり判定
	//}
};
#endif