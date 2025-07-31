//============================================================================
// 
// �R���W�����n�֐��A�w�b�_�t�@�C�� [collision.h]
// Author : ���c����
// 
//============================================================================

#ifndef _COLLISION_H_
#define _COLLISION_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************

//****************************************************
// �O���錾
//****************************************************
class CCollisionInfo_BoundingBox;        // ��������
class CCollisionInfo_BoundingCylinder;   // �~��������
class COBB; // OBB������
class CRotatingCylinder; // ��]�~��������

//****************************************************
// �R���W���������N���X���`
//****************************************************
class CCollisionInfo
{
public:

	//������
	enum class TYPE
	{
		NONE = 0, // �Ȃ�
		BOX,      // ��
		CYLINDER, // �~��
		OBB,      // OBB
		MAX       // �ő�
	};

	CCollisionInfo();                 // �R���X�g���N�^
	virtual ~CCollisionInfo();        // �f�X�g���N�^

	//< Setter/Getter>
	void SetPos(D3DXVECTOR3 Pos);         // �ʒu�ݒ�
	const D3DXVECTOR3& GetPos() const;    // �ʒu�擾

	//< Setter/Getter>
	void SetPosOld(D3DXVECTOR3 PosOld);   // 1f�O�̈ʒu�ݒ�
	const D3DXVECTOR3& GetPosOld() const; // 1f�O�̈ʒu�擾

	//< Setter/Getter>
	void SetLanding(bool bIs);            // �n�ʂɏ���Ă��邩�ǂ����ݒ�
	const bool& GetLanding() const;       // �n�ʂɏ���Ă��邩�ǂ����擾

	//< Setter/Getter>                    
	void SetLandingOld(bool bIsOld);     // 1f�O�ɒn�ʂɏ���Ă��邩�ǂ����ݒ�
	const bool& GetLandingOld() const;   // 1f�O�ɒn�ʂɏ���Ă��邩�ǂ����擾

	//<setter/getter>


	//< Getter>
	const TYPE& GetType() const;          // ����^�C�v�擾

	//< Setter>
	void SetUpdateInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld,bool bIsLanding,bool bIsLandingOld);// ���X�V�p�i����O�ɑS�Ă̏����蓮�ōX�V�j

protected:
	//< Setter>
	void SetType(TYPE Type);              // ����^�C�v�ݒ�
private:
	D3DXVECTOR3 m_Pos;     // �ʒu
	D3DXVECTOR3 m_PosOld;  // 1f�O�̈ʒu
	TYPE m_CollisionType;  // ����^�C�v
	bool m_bIsLanding;     // �n�ʂɏ���Ă��邩�ǂ���
	bool m_bIsLandingOld;  // 1f�O�ɒn�ʂɏ���Ă��邩�ǂ���
};

//****************************************************
// ���O���collision���`
//****************************************************
namespace collision
{
	//****************************************************
	// using�f�B���N�e�B�u
	//****************************************************
	using namespace useful;

	namespace PushOutBoxToBoxFunction
	{
		void PushX(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other,bool & bSuccess);
		void PushY(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other,bool & bSuccess);
		void PushZ(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other,bool & bSuccess);
	}

	//// �����m
	//bool HitSphereToSphere(const CBounding_Sphere* const Self, const CBounding_Sphere* const Other);

	//// ����AABB
	//bool HitSphereToAABB(const CBounding_Sphere* const Self, const CBounding_Box* const Other);

	//// �~�����m
	//bool HitCylinderToCylinder(const CBounding_Cylinder* const Self, const CBounding_Cylinder* const Other);

	//// �~���Ɠ_
	//bool HitCylinderToPoint(const CBounding_Cylinder* const Self, const Vec3& Other);

	//// �~���Ƌ�
	//bool HitCylinderToSphere(const CBounding_Cylinder* const Self, const CBounding_Sphere* const Other);
	//bool HitCylinderToSphere(const Vec3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const float& fOtherRadius);

	//// �~����AABB
	//bool HitCylinderToAABB(const CBounding_Cylinder* const Self, const CBounding_Box* const Other);
	//bool HitCylinderToAABB(const Vec3& SelfPos, const float& fSelfRadius, const float& fHeight, const Vec3& PosTarget, const Vec3& SizeTarget);

	// �~����AABB�̏Փ˖ʂ��擾
	//int GetCylinderToAABB(const Vec3& SelfOldPos, const Vec3& SelfNowPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const Vec3& OtherSize);

	// �����m�Փ˔���uconst ���� const�v�́A�w���l���|�C���^�������ύX�s��
	bool HitBoxToBox(const CCollisionInfo_BoundingBox* const Self, const CCollisionInfo_BoundingBox* const Other);

	// �����m�����o������uconst ���� const�v�́A�w���l���|�C���^�������ύX�s��
	bool PushOutBoxToBox(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other);

	// ���Ή~���Փ˔���
	bool HitBoxToCylinder(const CCollisionInfo_BoundingBox* const  Self,const CCollisionInfo_BoundingCylinder* const Other);

	// ���Ή~�������o������
	bool PushOutBoxToCylynder(CCollisionInfo_BoundingBox* const Self, const CCollisionInfo_BoundingCylinder* const Other);

	// �~�����m�Փ˔���
	bool HitCylinderToCylinder(const CCollisionInfo_BoundingCylinder* const Self, const CCollisionInfo_BoundingCylinder* const Other);

	// �~�����m�����o������
	bool PushOutCylinderToCylinder(CCollisionInfo_BoundingCylinder* const Self,CCollisionInfo_BoundingCylinder* const Other);

	// OBB���m�Փ˔���
	bool HitOBBToOBB(COBB* const Self, COBB* const Other);

	// AABB�Ɖ�]Cylinder�Փ˔���
	bool HitAABBToRotatingCylinder(const CCollisionInfo_BoundingBox * const Self,const CRotatingCylinder* const Other );

	// AABB�ƃ��[���h��ԉ�]�V�����_�[�Փ˔���
	bool HitAABBToIsWorldRotatingCylinder(const CCollisionInfo_BoundingBox* const Self, const CRotatingCylinder* const Other,
		D3DXVECTOR3 OtherParentPos,D3DXVECTOR3 OtherParentRot);

	// AABB�ƃ��[���h��]�V�����_�[�Փ˔���_�Â�
	bool HitAABBToIsWorldRotatingCylinder_Sweat(const CCollisionInfo_BoundingBox* const Self, const CRotatingCylinder* const Other,
		D3DXVECTOR3 OtherParentPos, D3DXVECTOR3 OtherParentRot);
	//======================
	//�I�u�W�F�N�gX
	//======================
	//namespace ObjectX
	//{
	//	bool HitSquareToAABB(CObject_X* pMyObj, CObject_X* pComObj);//AABB�����蔻��
	//}
};
#endif