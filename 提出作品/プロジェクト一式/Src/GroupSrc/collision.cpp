//============================================================================
// 
// �R���W�����n�֐� [collision.cpp]
// Author : ���c����
// 
////============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "collision.h"    // ���w�b�_
#include "calculation.h"  // �v�Z�p���
#include "OBB.h"          // OBB���
#include "bounding_box.h" // ��������
#include "bounding_cylinder.h" // �~�����
#include "RotatingCylinder.h"  // ��]�~�����
#include "pch.h"
#include "renderer.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

////============================================================================
//// �����m
////============================================================================
//bool collision::HitSphereToSphere(const CBounding_Sphere* const Self, const CBounding_Sphere* const Other)
//{
//	// ���݂��̋������v�Z
//	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();
//
//	// �������A���݂��̔��a�̘a�ȉ��Ȃ�ΏՓ�
//	if (sqrtf(Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z) <= Self->GetRadius() + Other->GetRadius())
//	{
//		return 1;
//	}
//
//	return 0;
//}
//
////============================================================================
//// ����AABB
////============================================================================
//bool collision::HitSphereToAABB(const CBounding_Sphere* const Self, const CBounding_Box* const Other)
//{
//	// �p�����[�^���R�s�[
//	const Vec3& SelfPos = Self->GetCenterPos(), OtherPos = Other->GetCenterPos(), OtherSize = Other->GetSize();
//	const float& fSelfRadius = Self->GetRadius();
//
//	// �S�Ă̎������ւ̎ˉe���m���d�Ȃ��Ă���ΏՓ�
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
//// �~�����m
////============================================================================
//bool collision::HitCylinderToCylinder(const CBounding_Cylinder* const Self, const CBounding_Cylinder* const Other)
//{
//	// ���݂��̋������v�Z
//	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();
//
//	// �������A���݂��̔��a�̘a���傫����ΏՓ˂��Ă��Ȃ�
// 	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius() + Other->GetRadius())
//	{
//		return 0;
//	}
//
//	// Y���ւ̎ˉe���m���d�Ȃ��Ă�����Փ�
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
//// �~���Ɠ_
////============================================================================
//bool collision::HitCylinderToPoint(const CBounding_Cylinder* const Self, const Vec3& Other)
//{
//	// ���݂��̋������v�Z
//	const Vec3& Distance = Other - Self->GetCenterPos();
//
//	// �������A�~���̔��a���傫����ΏՓ˂��Ă��Ȃ�
//	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius())
//	{
//		return 0;
//	}
//
//	// Y���ւ̎ˉe�ɓ_�����݂���ΏՓ�
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
//// �~���Ƌ�
////============================================================================
//bool collision::HitCylinderToSphere(const CBounding_Cylinder* const Self, const CBounding_Sphere* const Other)
//{
//	// ���݂��̋������v�Z
//	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();
//
//	// �������A���݂��̔��a�̘a���傫����ΏՓ˂��Ă��Ȃ�
//	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius() + Other->GetRadius())
//	{
//		return 0;
//	}
//
//	// Y���ւ̎ˉe���m���d�Ȃ��Ă���ΏՓ�
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
//// �~���Ƌ�
////============================================================================
//bool collision::HitCylinderToSphere(const Vec3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const float& fOtherRadius)
//{
//	// ���݂��̋������v�Z
//	const Vec3& Distance = OtherPos - SelfPos;
//
//	// �������A���݂��̔��a�̘a���傫����ΏՓ˂��Ă��Ȃ�
//	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > fSelfRadius + fOtherRadius)
//	{
//		return 0;
//	}
//
//	// Y���ւ̎ˉe���m���d�Ȃ��Ă���ΏՓ�
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
//// �~����AABB
////============================================================================
//bool collision::HitCylinderToAABB(const CBounding_Cylinder* const Self, const CBounding_Box* const Other)
//{
//	// �p�����[�^���R�s�[
//	const Vec3& SelfPos = Self->GetCenterPos(), OtherPos = Other->GetCenterPos(), OtherSize = Other->GetSize();
//	const float& fSelfRadius = Self->GetRadius(), fSelfHeight = Self->GetHeight();
//
//	// �S�Ă̎������ւ̎ˉe���m���d�Ȃ��Ă���ΏՓ�
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
//// �~����AABB
////============================================================================
//bool collision::HitCylinderToAABB(const Vec3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const Vec3& OtherSize)
//{
//	// �S�Ă̎������ւ̎ˉe���m���d�Ȃ��Ă���ΏՓ�
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
//// �~����AABB�̏Փ˖ʂ��擾
////============================================================================
//int collision::GetCylinderToAABB(const Vec3& SelfOldPos, const Vec3& SelfNowPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const Vec3& OtherSize)
//{
//#ifdef _DEBUG
//	// �E�B���h�E��\��
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
//	// �Փˎ��̂��������
//	if (!HitCylinderToAABB(SelfNowPos, fSelfRadius, fSelfHeight, OtherPos, OtherSize))
//	{
//		return 0;
//	}
//
//	// ���Ƃ���X���ւ̎ˉe���m���d�Ȃ��Ă����� (�v�Z��Z�����ɏՓ˂͂��Ȃ�)
//	if (SelfOldPos.x + fSelfRadius > OtherPos.x - OtherSize.x &&
//		SelfOldPos.x - fSelfRadius < OtherPos.x + OtherSize.x)
//	{ // �d�Ȃ��Ă��� -> �㉺�̔���
//
//		// ��ɂ���
//		if (SelfOldPos.y > OtherPos.y)
//		{
//			return 1;
//		}
//
//		// ���ɂ���
//		if (SelfOldPos.y < OtherPos.y)
//		{
//			return 2;
//		}
//	}
//	else
//	{ // �d�Ȃ��Ă��Ȃ����� -> ���E�̔���
//
//		// ���ɂ���
//		if (SelfOldPos.x < OtherPos.x)
//		{
//#ifdef _DEBUG
//			CRenderer::SetTimeString("[��ꂫ]�Փ˖� : ��", 60);
//#endif	// _DEBUG
//			return 3;
//		}
//
//		// �E�ɂ���
//		if (SelfOldPos.x > OtherPos.x)
//		{
//#ifdef _DEBUG
//			CRenderer::SetTimeString("[��ꂫ]�Փ˖� : �E", 60);
//#endif	// _DEBUG
//			return 4;
//		}
//	}
//
//	return 0;
//}

//******************************************************************************************
// �R���W�����X�g���e�W�[�N���X�i�X�g���e�W�[�ɂ��邱�Ƃɂ��A�`�ɉ��������肪�\�j
//******************************************************************************************

//=====================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//=====================================================
CCollisionInfo::~CCollisionInfo()
{

}

//=====================================================
// �ʒu�ݒ�
//=====================================================
void CCollisionInfo::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//=====================================================
// �ʒu�擾
//=====================================================
const D3DXVECTOR3& CCollisionInfo::GetPos() const
{
	return m_Pos;
}

//=====================================================
// 1f�O�̈ʒu�ݒ�
//=====================================================
void CCollisionInfo::SetPosOld(D3DXVECTOR3 PosOld)
{
	m_PosOld = PosOld;
}

//=====================================================
// 1f�O�̈ʒu�擾
//=====================================================
const D3DXVECTOR3& CCollisionInfo::GetPosOld() const
{
	return m_PosOld;
}

//=====================================================
// �n�ʂɏ���Ă��邩�ǂ����ݒ�
//=====================================================
void CCollisionInfo::SetLanding(bool bIs)
{
	m_bIsLanding = bIs;
}

//=====================================================
// �n�ʂɏ���Ă��邩�ǂ����擾
//=====================================================
const bool& CCollisionInfo::GetLanding() const
{
	return m_bIsLanding;
}

//=====================================================
// 1f�O�ɒn�ʂɏ���Ă��邩�ǂ����ݒ�
//=====================================================
void CCollisionInfo::SetLandingOld(bool bIsOld)
{
	m_bIsLandingOld = bIsOld;
}

//=====================================================
// 1f�O�ɒn�ʂɏ���Ă��邩�ǂ����擾
//=====================================================
const bool& CCollisionInfo::GetLandingOld() const
{
	return m_bIsLandingOld;
}

//=====================================================
// ����^�C�v�擾
//=====================================================
const CCollisionInfo::TYPE& CCollisionInfo::GetType() const
{
	return m_CollisionType;
}

//=====================================================
// ����^�C�v�ݒ�
//=====================================================
void CCollisionInfo::SetType(TYPE Type)
{
	m_CollisionType = Type;
}

//=====================================================
// ���X�V�p
//=====================================================
void CCollisionInfo::SetUpdateInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld, bool bIsLanding, bool bIsLandingOld)
{
	m_Pos = Pos;               // �ʒu
	m_PosOld = PosOld;         // 1f�O�̈ʒu
	m_bIsLanding = bIsLanding; // �n�ʂɏ���Ă��邩�ǂ���
	m_bIsLandingOld = bIsLandingOld;// 1f�O�ɒn�ʂɏ���Ă��邩�ǂ���
}

//******************************************************************************************
// �R���W�����֐�(namespace)
//******************************************************************************************

//=====================================================
// ���Δ��Փ˔���
//=====================================================
bool collision::HitBoxToBox(const CCollisionInfo_BoundingBox* const Self, const CCollisionInfo_BoundingBox* const Other)
{
	const D3DXVECTOR3& 
		SelfPos = Self->GetPos(),            // �����̈ʒu
	    SelfVtxMax = Self->GetVtxMax(),      // �����̍ő咸�_
	    SelfVtxMin = Self->GetVtxMin(),      // �����̍ŏ����_
	    OtherPos = Other->GetPos(),          // ����̈ʒu
	    OtherVtxMax = Other->GetVtxMax(),    // ����̍ő咸�_
	    OtherVtxMin = Other->GetVtxMin();    // ����̍ŏ����_

	// ����J�n
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
// ���Δ������o������
//=====================================================
bool collision::PushOutBoxToBox(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other)
{
	// ����
	const bool& bIsLandingOld = Self->GetLandingOld();      // 1f�O�Ɏ������g���n�ʂɂ������ǂ���
	const bool& bPushXOld = Self->GetPushXOld();            // 1f�O��X���ɉ����o����Ă������ǂ���
	bool bSuccess = false;                                  // ���肪��������

	if (bPushXOld)
	{
		CRenderer::SetDebugString("X���D��");

		if (bIsLandingOld)
		{
			PushOutBoxToBoxFunction::PushY(Self, Other, bSuccess);         // Y�������o��
			PushOutBoxToBoxFunction::PushX(Self, Other, bSuccess);         // X�������o��
			PushOutBoxToBoxFunction::PushZ(Self, Other, bSuccess);         // Z�������o��
		}
		else
		{
			PushOutBoxToBoxFunction::PushX(Self, Other, bSuccess);         // X�������o��
			PushOutBoxToBoxFunction::PushZ(Self, Other, bSuccess);         // Z�������o��
			PushOutBoxToBoxFunction::PushY(Self, Other, bSuccess);         // Y�������o��

		}
	}
	else
	{
		CRenderer::SetDebugString("Z���D��");
		if (bIsLandingOld)
		{
			PushOutBoxToBoxFunction::PushY(Self, Other, bSuccess);         // Y�������o��
			PushOutBoxToBoxFunction::PushZ(Self, Other, bSuccess);         // Z�������o��
			PushOutBoxToBoxFunction::PushX(Self, Other, bSuccess);         // X�������o��
		}
		else
		{
			PushOutBoxToBoxFunction::PushZ(Self, Other, bSuccess);         // Z�������o��
			PushOutBoxToBoxFunction::PushX(Self, Other, bSuccess);         // X�������o��
			PushOutBoxToBoxFunction::PushY(Self, Other, bSuccess);         // Y�������o��
		}
	}
	return bSuccess;
}

//=====================================================
// ���Ή~���Փ˔���
//=====================================================
bool collision::HitBoxToCylinder(const CCollisionInfo_BoundingBox * const Self, const CCollisionInfo_BoundingCylinder* const Other)
{
	// ����
	const D3DXVECTOR3& SelfPos = Self->GetPos();           // �����̈ʒu
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax();     // �����̍ő咸�_
	const D3DXVECTOR3 SelfVtxMaxPos = SelfPos + SelfVtxMax;// �����̍ő咸�_�̈ʒu
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin();     // �����̍ŏ����_
	const D3DXVECTOR3 SelfVtxMinPos = SelfPos + SelfVtxMin;// �����̍ŏ����_�̈ʒu

	// ����
	const D3DXVECTOR3& OtherPos = Other->GetPos();       // ����̈ʒu
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // ����̍ő咸�_
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // ����̍ŏ����_
	const D3DXVECTOR3& OtherSize = Other->GetSize();     // ����̃T�C�Y

	if (SelfPos.y + SelfVtxMax.y < OtherPos.y + OtherVtxMin.y ||
		SelfPos.y + SelfVtxMin.y > OtherPos.y + OtherVtxMax.y)
	{//Y���ŏd�Ȃ��Ă��Ȃ�
		return false;
	}

	float fOtherRadius = OtherSize.x; //����̔��a�����߂�

	float fNearX = 0.0f;// �ŋߓ_X
	float fNearZ = 0.0f;// �ŋߓ_Z

	if (SelfVtxMinPos.x < OtherPos.x && OtherPos.x < SelfVtxMaxPos.x)
	{//����̒��S�_��������AABB�̓����ɓ����Ă�����
		fNearX = OtherPos.x;// �ŋߓ_�͓����ɓ����Ă���̂œ����ʒu
		CRenderer::SetDebugString("���S�_X��AABB�̒���");
	}
	else
	{//�ŋߓ_�͎�����Max��Min
		float fVtxMinLengthX = fabsf(SelfVtxMinPos.x - OtherPos.x);// �����̍ŏ����_�Ƃ̋���
		float fVtxMaxLengthX = fabsf(SelfVtxMaxPos.x - OtherPos.x);// �����̍ő咸�_�Ƃ̋���

		if (fVtxMinLengthX < fVtxMaxLengthX)
		{//�ŋߓ_�͎����̍ŏ����_�̈ʒu
			fNearX = SelfVtxMinPos.x;
			CRenderer::SetDebugString("�ŋߓ_X��AABB�̍ŏ�");
		}
		else
		{//�ŋߓ_�͎����̍ő咸�_�̈ʒu
			fNearX = SelfVtxMaxPos.x;
			CRenderer::SetDebugString("�ŋߓ_X��AABB�̍ő�");
		}
	}

	if (SelfVtxMinPos.z < OtherPos.z && OtherPos.z < SelfVtxMaxPos.z)
	{//����̒��S�_��������AABB�̓����ɓ����Ă�����
		fNearZ = OtherPos.z;// �ŋߓ_�͓����ɓ����Ă���̂œ����ʒu
		CRenderer::SetDebugString("���S�_Z��AABB�̒���");
	}
	else
	{//�ŋߓ_�͎�����Max��Min
		float fVtxMinLengthZ = fabsf(SelfVtxMinPos.z - OtherPos.z);// �����̍ŏ����_�Ƃ̋���
		float fVtxMaxLengthZ = fabsf(SelfVtxMaxPos.z - OtherPos.z);// �����̍ő咸�_�Ƃ̋���

		if (fVtxMinLengthZ < fVtxMaxLengthZ)
		{//�ŋߓ_�͎����̍ŏ����_�̈ʒu
			fNearZ = SelfVtxMinPos.z;
			CRenderer::SetDebugString("�ŋߓ_Z��AABB�̍ŏ�");
		}
		else
		{//�ŋߓ_�͎����̍ő咸�_�̈ʒu
			fNearZ = SelfVtxMaxPos.z;
			CRenderer::SetDebugString("�ŋߓ_Z��AABB�̍ő�");
		}
	}

	float fLength = sqrtf(powf(fNearX - OtherPos.x, 2) + powf(fNearZ - OtherPos.z,2)); // �ŋߓ_�Ƃ̋��������

	if (fLength < fOtherRadius)
	{//�ŒZ�_�Ƃ̋���������̔��a�����Z�����
		return true;
	}

	return false;
}

//=====================================================
// ���Ή~�������o������
//=====================================================
bool collision::PushOutBoxToCylynder(CCollisionInfo_BoundingBox* const Self, const CCollisionInfo_BoundingCylinder* const Other)
{
	// ����
	const D3DXVECTOR3& SelfPos = Self->GetPos();           // �����̈ʒu
	const D3DXVECTOR3& SelfPosOld = Self->GetPosOld();     // ������1f�O�̈ʒu
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax();     // �����̍ő咸�_
	const D3DXVECTOR3 SelfVtxMaxPos = SelfPos + SelfVtxMax;// �����̍ő咸�_�̈ʒu
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin();     // �����̍ŏ����_
	const D3DXVECTOR3 SelfVtxMinPos = SelfPos + SelfVtxMin;// �����̍ŏ����_�̈ʒu

	// ����
	const D3DXVECTOR3& OtherPos = Other->GetPos();       // ����̈ʒu
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // ����̍ő咸�_
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // ����̍ŏ����_
	const D3DXVECTOR3& OtherSize = Other->GetSize();     // ����̃T�C�Y

	float fOtherRadius = OtherSize.x; //����̔��a�����߂�

	float fNearX = 0.0f;// �ŋߓ_X
	float fNearZ = 0.0f;// �ŋߓ_Z

	if (SelfVtxMinPos.x < OtherPos.x && OtherPos.x < SelfVtxMaxPos.x)
	{//����̒��S�_��������AABB�̓����ɓ����Ă�����
		fNearX = OtherPos.x;// �ŋߓ_�͓����ɓ����Ă���̂œ����ʒu
		CRenderer::SetDebugString("���S�_X��AABB�̒���");
	}
	else
	{//�ŋߓ_�͎�����Max��Min
		float fVtxMinLengthX = fabsf(SelfVtxMinPos.x - OtherPos.x);// �����̍ŏ����_�Ƃ̋���
		float fVtxMaxLengthX = fabsf(SelfVtxMaxPos.x - OtherPos.x);// �����̍ő咸�_�Ƃ̋���

		if (fVtxMinLengthX < fVtxMaxLengthX)
		{//�ŋߓ_�͎����̍ŏ����_�̈ʒu
			fNearX = SelfVtxMinPos.x;
			CRenderer::SetDebugString("�ŋߓ_X��AABB�̍ŏ�");
		}
		else
		{//�ŋߓ_�͎����̍ő咸�_�̈ʒu
			fNearX = SelfVtxMaxPos.x;
			CRenderer::SetDebugString("�ŋߓ_X��AABB�̍ő�");
		}
	}

	if (SelfVtxMinPos.z < OtherPos.z && OtherPos.z < SelfVtxMaxPos.z)
	{//����̒��S�_��������AABB�̓����ɓ����Ă�����
		fNearZ = OtherPos.z;// �ŋߓ_�͓����ɓ����Ă���̂œ����ʒu
		CRenderer::SetDebugString("���S�_Z��AABB�̒���");
	}
	else
	{//�ŋߓ_�͎�����Max��Min
		float fVtxMinLengthZ = fabsf(SelfVtxMinPos.z - OtherPos.z);// �����̍ŏ����_�Ƃ̋���
		float fVtxMaxLengthZ = fabsf(SelfVtxMaxPos.z - OtherPos.z);// �����̍ő咸�_�Ƃ̋���

		if (fVtxMinLengthZ < fVtxMaxLengthZ)
		{//�ŋߓ_�͎����̍ŏ����_�̈ʒu
			fNearZ = SelfVtxMinPos.z;
			CRenderer::SetDebugString("�ŋߓ_Z��AABB�̍ŏ�");
		}
		else
		{//�ŋߓ_�͎����̍ő咸�_�̈ʒu
			fNearZ = SelfVtxMaxPos.z;
			CRenderer::SetDebugString("�ŋߓ_Z��AABB�̍ő�");
		}
	}

	D3DXVECTOR3 ClosestVec = { fNearX - OtherPos.x, 0.0f, fNearZ - OtherPos.z }; // �ŋߓ_�Ɖ~���̒��S�Ƃ̋���
	float fLength = D3DXVec3Length(&ClosestVec);                                 // �x�N�g���̑傫��
	CRenderer::SetDebugString("����" + to_string(fLength));                      // �����f�o�b�O�\��
	bool bSuccess = false;                                                       // ���肪�����������ǂ���

	// �P�F���Ɖ~�����d�Ȃ��Ă��遨���Y�������o��
	// �Q�F���̂�����Y���ŏd�Ȃ��Ă���ꍇ��XZ�����o��

	//�P
	if (fLength < fOtherRadius)
	{// XZ���ʂœ������Ă�����

		if (SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y &&
			SelfPosOld.y + SelfVtxMin.y >= OtherPos.y + OtherVtxMax.y)
		{// ��[�ւ̉����o��
			Self->SetPos(D3DXVECTOR3(SelfPos.x, OtherPos.y + OtherVtxMax.y - SelfVtxMin.y, SelfPos.z)); // ������ɉ����o��
			Self->SetLanding(true); // �n�ʂɏ���Ă���
			bSuccess = true;
		}
		else if (SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y &&
			SelfPosOld.y + SelfVtxMax.y <= OtherPos.y + OtherVtxMin.y)
		{// ���[�ւ̉����o��
			Self->SetPos(D3DXVECTOR3(SelfPos.x, OtherPos.y + OtherVtxMin.y - SelfVtxMax.y, SelfPos.z)); // �������ɉ����o��
			bSuccess = true;
		}
	}

	//�Q
	if (SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y &&
		SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y)
	{//Y���ŏd�Ȃ��Ă���

		if (fLength < fOtherRadius && fLength > 0.0001f)
		{// ������0�ȏザ��Ȃ��Ɖ����o���Ȃ�

			D3DXVECTOR3 PushVec;
			D3DXVec3Normalize(&PushVec, &ClosestVec); // ���K�������o���x�N�g��

			float fDiffLength = fOtherRadius - fLength; // ���a�ƍŋߓ_�Ƃ̋����̍���

			D3DXVECTOR3 NewPos = Self->GetPos() + PushVec * fDiffLength; // ��L�̏����ŋ��߂��������������o��
			Self->SetPos(NewPos);                                        // �ʒu��ݒ�

			bSuccess = true;
		}
	}
	return bSuccess;

}

//=====================================================
// �~�����m�Փ˔���
//=====================================================
bool collision::HitCylinderToCylinder(const CCollisionInfo_BoundingCylinder* const Self, const CCollisionInfo_BoundingCylinder* const Other)
{
	const D3DXVECTOR3& SelfPos = Self->GetPos();         // �����̈ʒu
	const D3DXVECTOR3& SelfSize = Self->GetSize();       // �����̃T�C�Y
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax();   // �����̍ő咸�_
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin();   // �����̍ŏ����_

	const D3DXVECTOR3& OtherPos = Other->GetPos();       // ����̈ʒu
	const D3DXVECTOR3& OtherSize = Other->GetSize();     // ����̃T�C�Y
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // ����̍ő咸�_
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // ����̍ŏ����_


	float fLength = sqrtf(powf(OtherPos.x - SelfPos.x, 2) + powf(OtherPos.z - SelfPos.z, 2));// �~�����m�̋���


	if (SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y &&
		SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y)
	{//Y�����d�Ȃ��Ă���
		if (fLength < SelfSize.x + OtherSize.x)
		{//��̉~���̔��a�̍��v��蒆�S�_�Ԃ̋������Z����Γ������Ă���
			return true;
		}
	}
	return false;
}

//=====================================================
// �~�����m�����o������
//=====================================================
bool collision::PushOutCylinderToCylinder(CCollisionInfo_BoundingCylinder* const Self, CCollisionInfo_BoundingCylinder* const Other)
{
	const D3DXVECTOR3& SelfPos = Self->GetPos();         // �����̈ʒu
	const D3DXVECTOR3& SelfPosOld = Self->GetPosOld();   // ������1f�O�̈ʒu
	const D3DXVECTOR3& SelfSize = Self->GetSize();       // �����̃T�C�Y
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax();   // �����̍ő咸�_
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin();   // �����̍ŏ����_

	const D3DXVECTOR3& OtherPos = Other->GetPos();       // ����̈ʒu
	const D3DXVECTOR3& OtherSize = Other->GetSize();     // ����̃T�C�Y
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // ����̍ő咸�_
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // ����̍ŏ����_


	float fLength = sqrtf(powf(OtherPos.x - SelfPos.x, 2) + powf(OtherPos.z - SelfPos.z, 2));// �~�����m�̋���

	bool bSuccess = false; // ���肪�����������ǂ���

	if (fLength < SelfSize.x + OtherSize.x)
	{//��̉~���̔��a�̍��v��蒆�S�_�Ԃ̋������Z����Γ������Ă���

		if (SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y &&
			SelfPosOld.y + SelfVtxMin.y >= OtherPos.y + OtherVtxMax.y)
		{// ��[�ւ̉����o��
			Self->SetPos(D3DXVECTOR3(SelfPos.x, OtherPos.y + OtherVtxMax.y - SelfVtxMin.y, SelfPos.z)); // ������ɉ����o��
			Self->SetLanding(true); // �n�ʂɏ���Ă���
			bSuccess = true; // ����
		}
		else if (SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y &&
			SelfPosOld.y + SelfVtxMax.y <= OtherPos.y + OtherVtxMin.y)
		{// ���[�ւ̉����o��
			Self->SetPos(D3DXVECTOR3(SelfPos.x, OtherPos.y + OtherVtxMin.y - SelfVtxMax.y, SelfPos.z)); // �������ɉ����o��
			bSuccess = true; // ����
		}

		// XZ�����o���̂ݍs���iY�����d�Ȃ��Ă���Ƃ��j
		if (SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y &&
			SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y)
		{
			D3DXVECTOR3 PushVecXZ = SelfPos - OtherPos; // �����o�������ւ̃x�N�g��
			PushVecXZ.y = 0.0f; // Y���͖���

			if (fLength != 0.0f) 
			{
				D3DXVec3Normalize(&PushVecXZ, &PushVecXZ); // �����o���x�N�g���𐳋K������
				float fPushDist = SelfSize.x + OtherSize.x - fLength; // ���a�̘a�Ƌ����̍������������o��
				Self->SetPos(SelfPos + PushVecXZ * fPushDist); // �ݒ�
				bSuccess = true;                               // ����
			}
		}
	}

	return bSuccess;
}

//=====================================================
// OBB��OBB�Փ˔���
//=====================================================
bool collision::HitOBBToOBB(COBB* const Self, COBB* const Other)
{
	// === �񋓌^ ===

	// ��
	enum class AXIS
	{
		X = 0,
		Y,
		Z,
		MAX
	};

	// === �����Ɏg�p�������錾�A������ ===

	const D3DXVECTOR3&
		SelfPos = Self->GetPos(),        // �������g�̈ʒu
		SelfRot = Self->GetRot(),        // �������g�̌���
		SelfVtxMax = Self->GetVtxMax(),  // �������g�̍ő咸�_
		SelfVtxMin = Self->GetVtxMin();  // �������g�̍ŏ����_
	D3DXVECTOR3
		SelfBoxCornars[8] = {},   // �������g�̐����`�́u�p�v��\��
		SelfWorldCornars[8] = {}; // �������g�̉�]��́u�p�v��\��

	const D3DXVECTOR3&
		OtherPos = Other->GetPos(),        // ����̈ʒu
		OtherRot = Other->GetRot(),        // ����̌���       
		OtherVtxMax = Other->GetVtxMax(),  // ����̍ő咸�_
		OtherVtxMin = Other->GetVtxMin();  // ����̍ŏ����_
	D3DXVECTOR3
		OtherBoxCornars[8] = {},   // ����̐����`�́u�p�v��\��
		OtherWorldCornars[8] = {}; // ����̉�]��́u�p�v��\��

	// === �����`�W���_�̈ʒu�����߂� ===

	// ���g
	SelfBoxCornars[0] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMax.z); // ����O
	SelfBoxCornars[1] = SelfVtxMax; // �E��O
	SelfBoxCornars[2] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMin.z); // �����
	SelfBoxCornars[3] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMax.y, SelfVtxMin.z); // �E���
	SelfBoxCornars[4] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMin.y, SelfVtxMax.z); // �����O
	SelfBoxCornars[5] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMax.z); // �E���O
	SelfBoxCornars[6] = SelfVtxMin; // ������
	SelfBoxCornars[7] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMin.z); // �E����

	// ����
	OtherBoxCornars[0] = D3DXVECTOR3(OtherVtxMin.x, OtherVtxMax.y, OtherVtxMax.z); // ����O
	OtherBoxCornars[1] = OtherVtxMax; // �E��O
	OtherBoxCornars[2] = D3DXVECTOR3(OtherVtxMin.x, OtherVtxMax.y, OtherVtxMin.z); // �����
	OtherBoxCornars[3] = D3DXVECTOR3(OtherVtxMax.x, OtherVtxMax.y, OtherVtxMin.z); // �E���
	OtherBoxCornars[4] = D3DXVECTOR3(OtherVtxMin.x, OtherVtxMin.y, OtherVtxMax.z); // �����O
	OtherBoxCornars[5] = D3DXVECTOR3(OtherVtxMax.x, OtherVtxMin.y, OtherVtxMax.z); // �E���O
	OtherBoxCornars[6] = OtherVtxMin; // ������
	OtherBoxCornars[7] = D3DXVECTOR3(OtherVtxMax.x, OtherVtxMin.y, OtherVtxMin.z); // �E����

	// === ���g�̉�]��̂W���_�̈ʒu�����߂� ===

	D3DXMATRIX SelfRotMatrix; // �������g�̉�]�s��
	D3DXMatrixIdentity(&SelfRotMatrix); // ������

	// ���g�̉�]�s������߂�
	D3DXMatrixRotationYawPitchRoll(&SelfRotMatrix, SelfRot.y, SelfRot.x, SelfRot.z);

	// ���g�̉�]�s���p���ĉ�]��̂W���_�����߂�
	for (int i = 0; i < 8; ++i) {
		D3DXVec3TransformCoord(&SelfWorldCornars[i], &SelfBoxCornars[i], &SelfRotMatrix);
		SelfWorldCornars[i] += SelfPos;
	}

	// === ����̉�]��̂W���_�̈ʒu�����߂� ===

	D3DXMATRIX OtherRotMatrix; // ����̉�]�s��
	D3DXMatrixIdentity(&OtherRotMatrix); // ������

	// ����̉�]�s������߂�
	D3DXMatrixRotationYawPitchRoll(&OtherRotMatrix, OtherRot.y, OtherRot.x, OtherRot.z);

	// ����̉�]�s���p���ĉ�]��̂W���_�����߂�
	for (int i = 0; i < 8; ++i) {
		D3DXVec3TransformCoord(&OtherWorldCornars[i], &OtherBoxCornars[i], &OtherRotMatrix);
		OtherWorldCornars[i] += OtherPos;
	}


	// �W���_���\���Ă��邩�m�F�p
	{
		//// ��]��̂W���_��\���Ă��邩�`�F�b�N
		//for (int nCnt = 0; nCnt < 8; nCnt++)
		//{
		//	CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 2, 30.0f, 30.0f, SelfWorldCornars[nCnt], D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
		//}
	}

	// === ���[�J�������������߂� ===

	// ��ƂȂ鎲
	D3DXVECTOR3 BaseAxis[static_cast<int>(AXIS::MAX)] =
	{ {1.0f,0.0f,0.0f},
	 {0.0f,1.0f,0.0f},
	 {0.0f,0.0f,1.0f}
	};

	// ���g�̕�����
	D3DXVECTOR3 SelfSepAxis[static_cast<int>(AXIS::MAX)];

	// ���ꂼ��̕����������߂�
	for (int nCnt = 0; nCnt < static_cast<int>(AXIS::MAX); nCnt++)
	{
		D3DXVec3TransformCoord(&SelfSepAxis[nCnt], &BaseAxis[nCnt], &SelfRotMatrix);
	}

	// ����̕�����
	D3DXVECTOR3 OtherSepAxis[static_cast<int>(AXIS::MAX)];

	// ���ꂼ��̕����������߂�
	for (int nCnt = 0; nCnt < static_cast<int>(AXIS::MAX); nCnt++)
	{
		D3DXVec3TransformCoord(&OtherSepAxis[nCnt], &BaseAxis[nCnt], &OtherRotMatrix);
	}

	// === ���[�J���������ɉ�]��̒��_�𓊉e���A�������Ă��邩�ǂ��������߂� ===

	// �����̃��[�J�����Ɍ݂��̉�]�㒸�_�𓊉e
	for (int nCntAxis = 0; nCntAxis < 3; nCntAxis++)
	{
		// ���݂��̃I�u�W�F�N�g�̓��e�l�̍ő�ŏ���������
		float
			fSelfMin = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &SelfWorldCornars[0]),
			fSelfMax = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &SelfWorldCornars[0]),

			fOtherMin = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &OtherWorldCornars[0]),
			fOtherMax = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &OtherWorldCornars[0]);

		// ����̉�]�㒸�_�𓊉e
		for (int nCntVtx = 0; nCntVtx < 8; nCntVtx++)
		{
			// �݂��̒��_�̓��e���������߂�
			float fSelfLength = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &SelfWorldCornars[nCntVtx]);
			float fOtherLength = D3DXVec3Dot(&SelfSepAxis[nCntAxis], &OtherWorldCornars[nCntVtx]);

			// ���g�̓��e�l�̍Œ�l���X�V
			if (fSelfLength < fSelfMin)
			{
				fSelfMin = fSelfLength;
			}

			// ���g�̓��e�l�̍ő�l���X�V
			if (fSelfLength > fSelfMax)
			{
				fSelfMax = fSelfLength;
			}

			// ����̓��e�l�̍Œ�l���X�V
			if (fOtherLength < fOtherMin)
			{
				fOtherMin = fOtherLength;
			}

			// ����̓��e�l�̍ő�l���X�V
			if (fOtherLength > fOtherMax)
			{
				fOtherMax = fOtherLength;
			}
		}

		// �����������݂��Ă���̂ŏ������I��
		if (fSelfMax < fOtherMin || fOtherMax < fSelfMin)
		{
			return false;
		}
	}

	// ����̃��[�J�����Ɍ݂��̉�]�㒸�_�𓊉e
	for (int nCntAxis = 0; nCntAxis < 3; nCntAxis++)
	{
		// ���݂��̃I�u�W�F�N�g�̓��e�l�̍ő�ŏ���������
		float
			fSelfMin = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &SelfWorldCornars[0]),
			fSelfMax = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &SelfWorldCornars[0]),

			fOtherMin = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &OtherWorldCornars[0]),
			fOtherMax = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &OtherWorldCornars[0]);

		// ����̉�]�㒸�_�𓊉e
		for (int nCntVtx = 0; nCntVtx < 8; nCntVtx++)
		{
			// �݂��̒��_�̓��e���������߂�
			float fSelfLength = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &SelfWorldCornars[nCntVtx]);
			float fOtherLength = D3DXVec3Dot(&OtherSepAxis[nCntAxis], &OtherWorldCornars[nCntVtx]);

			// ���g�̓��e�l�̍Œ�l���X�V
			if (fSelfLength < fSelfMin)
			{
				fSelfMin = fSelfLength;
			}

			// ���g�̓��e�l�̍ő�l���X�V
			if (fSelfLength > fSelfMax)
			{
				fSelfMax = fSelfLength;
			}

			// ����̓��e�l�̍Œ�l���X�V
			if (fOtherLength < fOtherMin)
			{
				fOtherMin = fOtherLength;
			}

			// ����̓��e�l�̍ő�l���X�V
			if (fOtherLength > fOtherMax)
			{
				fOtherMax = fOtherLength;
			}
		}

		// �����������݂��Ă���̂ŏ������I��
		if (fSelfMax < fOtherMin || fOtherMax < fSelfMin)
		{
			return false;
		}
	}

	// === ���ꂼ��̕������ƒ��s���镪�����ƌ݂��̉�]�㒸�_�𓊉e ===

	D3DXVECTOR3 CrossSepAxis[9] = {}; // �݂��̕������ƒ��s���镪����
	int nCntArray = 0; // �z��J�E���g�p

	// �݂��̕������ƒ��s���镪���������߂�
	for (int nCntSelfSepAxis = 0; nCntSelfSepAxis < 3; nCntSelfSepAxis++)
	{
		for (int nCntOtherSepAxis = 0; nCntOtherSepAxis < 3; nCntOtherSepAxis++)
		{
			D3DXVec3Cross(&CrossSepAxis[nCntArray], &SelfSepAxis[nCntSelfSepAxis], &OtherSepAxis[nCntOtherSepAxis]);
			D3DXVec3Normalize(&CrossSepAxis[nCntArray], &CrossSepAxis[nCntArray]); // ���K��
			nCntArray++;
		}
	}

	// ���߂����s���镪�����Ɍ݂��̉�]�㒸�_�𓊉e�����肷��
	for (int nCntAxis = 0; nCntAxis < 9; nCntAxis++)
	{
		// �����̂Q�����ق�0�Ȃ�X�L�b�v�i0.00001f)
		if (D3DXVec3LengthSq(&CrossSepAxis[nCntAxis]) < 1e-5f)
		{
			continue;
		}

		// ���݂��̃I�u�W�F�N�g�̓��e�l�̍ő�ŏ���������
		float
			fSelfMin = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &SelfWorldCornars[0]),
			fSelfMax = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &SelfWorldCornars[0]),

			fOtherMin = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &OtherWorldCornars[0]),
			fOtherMax = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &OtherWorldCornars[0]);

		// ����̉�]�㒸�_�𓊉e
		for (int nCntVtx = 0; nCntVtx < 8; nCntVtx++)
		{
			// �݂��̒��_�̓��e���������߂�
			float fSelfLength = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &SelfWorldCornars[nCntVtx]);
			float fOtherLength = D3DXVec3Dot(&CrossSepAxis[nCntAxis], &OtherWorldCornars[nCntVtx]);


			// ���g�̓��e�l�̍Œ�l���X�V
			if (fSelfLength < fSelfMin)
			{
				fSelfMin = fSelfLength;
			}

			// ���g�̓��e�l�̍ő�l���X�V
			if (fSelfLength > fSelfMax)
			{
				fSelfMax = fSelfLength;
			}

			// ����̓��e�l�̍Œ�l���X�V
			if (fOtherLength < fOtherMin)
			{
				fOtherMin = fOtherLength;
			}

			// ����̓��e�l�̍ő�l���X�V
			if (fOtherLength > fOtherMax)
			{
				fOtherMax = fOtherLength;
			}
		}

		// �����������݂��Ă���̂ŏ������I��
		if (fSelfMax < fOtherMin || fOtherMax < fSelfMin)
		{
			return false;
		}
	}

	// �����������݂��Ȃ��̂œ������Ă���
	return true;
}

//=====================================================
// AABB�Ή�]�~���Փ˔���
//=====================================================
bool collision::HitAABBToRotatingCylinder(const CCollisionInfo_BoundingBox* const Self, const CRotatingCylinder* const Other)
{
	// === ����ɕK�v�ȏ���錾�A������ ===

	const D3DXVECTOR3&
		SelfPos = Self->GetPos(),       // ���g�̈ʒu
		SelfVtxMax = Self->GetVtxMax(), // ���g�̍ő咸�_
		SelfVtxMin = Self->GetVtxMin(); // ���g�̍ŏ����_

	const D3DXVECTOR3&
		OtherPos = Other->GetPos(),       // ����̈ʒu
		OtherVtxMax = Other->GetVtxMax(), // ����̍ő咸�_
		OtherVtxMin = Other->GetVtxMin(), // ����̍ŏ����_
		OtherDownPos = D3DXVECTOR3(0.0f, OtherVtxMin.y, 0.0f), // �~������
		OtherUpPos = D3DXVECTOR3(0.0f, OtherVtxMax.y, 0.0f),     // �~���㑤
		OtherRot = Other->GetRot();       // ����̌���
	float fOtherRadius = OtherVtxMax.x;   // ����̔��a

	D3DXVECTOR3 SelfBoxCornars[8]; // �����̔��̊p
	SelfBoxCornars[0] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMax.z); // ����O
	SelfBoxCornars[1] = SelfVtxMax; // �E��O
	SelfBoxCornars[2] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMin.z); // �����
	SelfBoxCornars[3] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMax.y, SelfVtxMin.z); // �E���
	SelfBoxCornars[4] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMin.y, SelfVtxMax.z); // �����O
	SelfBoxCornars[5] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMax.z); // �E���O
	SelfBoxCornars[6] = SelfVtxMin; // ������
	SelfBoxCornars[7] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMin.z); // �E����

	// === ����̉�]�s������߂� ===

	D3DXMATRIX MtxRot; // ��]�s��
	D3DXMatrixIdentity(&MtxRot); // ��]�s�񏉊���
	D3DXMatrixRotationYawPitchRoll(&MtxRot, OtherRot.y, OtherRot.x, OtherRot.z); // ��]�s������߂�

	D3DXMATRIX InvMtxRot;          // ��]�s��̋t�s��
	D3DXMatrixIdentity(&InvMtxRot); // ��]�s��̋t�s���������
	D3DXMatrixInverse(&InvMtxRot, NULL, &MtxRot); // ��]�s��̋t�s������߂�

	// === ����̉�]�t�s����|�������g�̔��̃��[���h��ԏ�ł̊p�̈ʒu�����߂� ===

	// ���@��]�����~����AABB�̔�����s���ہA�~������]���Ă���ꍇ�A�v�Z�����ɍ���ł��B
	// �����ŁA��]�����~���̋t�s��]�s���AABB�Ɋ|���邱�ƂŁA
	// �~������]���Ă��Ȃ���Ԃ�AABB�̊p�������X���Ă����ԂɂȂ�̂ŁA
	// �~���Ƃ̔��胍�W�b�N��ς����ɓ_���~�����ɓ����Ă��邩�ǂ����Ŕ�������邱�Ƃ��o���܂��B

	D3DXVECTOR3 SelfWorldBoxCornarsToInvRot[8];
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		D3DXVECTOR3 worldPos = SelfBoxCornars[nCnt] + SelfPos;
		D3DXVECTOR3 relativeToCylinder = worldPos - OtherPos;  // InvMtxRot�͉~����Ȃ̂ŁA�~������Ƃ������ΓI�Ȕ��̊p�̈ʒu�����߂�

		// ��L�ŋ��߂����ΓI�Ȉʒu�ɋt�s����|���A�~�����c�ɂ�����Ԃł̓_�̈ʒu�����߂�
		D3DXVec3TransformCoord(&SelfWorldBoxCornarsToInvRot[nCnt], &relativeToCylinder, &InvMtxRot); 

		// �����`�F�b�N�iY���j
		if (SelfWorldBoxCornarsToInvRot[nCnt].y < OtherVtxMin.y ||
			SelfWorldBoxCornarsToInvRot[nCnt].y > OtherVtxMax.y)
		{
			continue;
		}

		// XZ�����`�F�b�N
		float fLengthXZ = sqrtf(
			SelfWorldBoxCornarsToInvRot[nCnt].x * SelfWorldBoxCornarsToInvRot[nCnt].x +
			SelfWorldBoxCornarsToInvRot[nCnt].z * SelfWorldBoxCornarsToInvRot[nCnt].z);

		// XZ�������~���̔��a������������Γ������Ă���
		if (fLengthXZ <= fOtherRadius)
		{
			return true;
		}
	}

	return false;
}

//=====================================================
// AABB�΃��[���h��ԉ�]�~���Փ˔���
//=====================================================
bool collision::HitAABBToIsWorldRotatingCylinder(const CCollisionInfo_BoundingBox* const Self, const CRotatingCylinder* const Other, 
	D3DXVECTOR3 OtherParentPos, D3DXVECTOR3 OtherParentRot)
{
	// === ����ɕK�v�ȏ���錾�A������ ===

	const D3DXVECTOR3& SelfPos = Self->GetPos();       // ���g�̈ʒu
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax(); // ���g�̍ő咸�_
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin(); // ���g�̍ŏ����_

	const D3DXVECTOR3& OtherPos = Other->GetPos();       // �~���̈ʒu
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // �~���̍ő咸�_
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // �~���̍ŏ����_
	const D3DXVECTOR3& OtherRot = Other->GetRot();       // �~���̉�]

	float fOtherRadius = OtherVtxMax.x; // �~���̔��a

	// ���̂W���_���`����
	D3DXVECTOR3 SelfBoxCornars[8];
	SelfBoxCornars[0] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMax.z); // ����O
	SelfBoxCornars[1] = SelfVtxMax; // �E��O
	SelfBoxCornars[2] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMin.z); // �����
	SelfBoxCornars[3] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMax.y, SelfVtxMin.z); // �E���
	SelfBoxCornars[4] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMin.y, SelfVtxMax.z); // �����O
	SelfBoxCornars[5] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMax.z); // �E���O
	SelfBoxCornars[6] = SelfVtxMin; // ������
	SelfBoxCornars[7] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMin.z); // �E����

	// === �~���i�e���܂ށj�̉�]�s�� ===

	// 
	D3DXMATRIX 
		mtxParentRot,   // �e�̉�]�s��
		mtxParentTrans, // �e�̈ʒu�s��
		mtxOtherRot,    // ����̉�]�s��
		mtxOtherTrans;  // ����̈ʒu�s��

	// �e�̉�]�s������߂�
	D3DXMatrixRotationYawPitchRoll(&mtxParentRot, OtherParentRot.y, OtherParentRot.x, OtherParentRot.z);

	// �e�̈ʒu�s������߂�
	D3DXMatrixTranslation(&mtxParentTrans, OtherParentPos.x, OtherParentPos.y, OtherParentPos.z);

	// ����̉�]�s������߂�
	D3DXMatrixRotationYawPitchRoll(&mtxOtherRot, OtherRot.y, OtherRot.x, OtherRot.z);

	// ����̈ʒu�s������߂�
	D3DXMatrixTranslation(&mtxOtherTrans, OtherPos.x, OtherPos.y, OtherPos.z);

	// === �~���̃��[���h�s��(�q���̍s����|�����킹�Ă���e�̍s����|�����킹�܂��傤�j===

	D3DXMATRIX mtxOtherWorld = (mtxOtherRot * mtxOtherTrans) * (mtxParentRot * mtxParentTrans);

	// === �~���̋t�s��iAABB�����~���̃��[�J����Ԃɖ߂��j===

	D3DXMATRIX mtxInvOtherWorld;
	D3DXMatrixInverse(&mtxInvOtherWorld, NULL, &mtxOtherWorld);

	// === ���g��8���_���~���̃��[�J����Ԃɕϊ����AXZ�~�{Y�͈͂Ŕ��� ===

	// �����󌊂����I�u�W�F�N�g�̌��Ƃ̔��肾���Ŏg���̂ŁA
	//����͈�ł����_���~���̒��ɂȂ���Ώ����𒆒f���܂��B�i���S�ɉ~���̒��j
	for (int i = 0; i < 8; ++i)
	{
		D3DXVECTOR3
			worldPos = SelfBoxCornars[i] + SelfPos,        // ���g�̃��[���h���W
		    localToCylinder; // �~������Ƃ������[�J�����W

		// �~����̈ʒu�ɕϊ����A�ʏ�̉~���̔���ŏo����悤�ɂ��܂��B
		D3DXVec3TransformCoord(&localToCylinder, &worldPos, &mtxInvOtherWorld);

		// �����`�F�b�N
		if (localToCylinder.z < OtherVtxMin.z || localToCylinder.z > OtherVtxMax.z)
		{
			return false;
		}

		// XZ���ʂł̋������`�F�b�N���A����̔��a���Z����Γ������Ă���
		float distXZ = sqrtf(localToCylinder.x * localToCylinder.x + localToCylinder.y * localToCylinder.y);
		if (distXZ <= fOtherRadius)
		{
			CRenderer::SetDebugString("���������I");
		}
		else
		{
			return false;
		}
	}

	return true;
}

//=====================================================
// AABB�΃��[���h��ԉ�]�~���Փ˔���_�Â�
//=====================================================
bool collision::HitAABBToIsWorldRotatingCylinder_Sweat(const CCollisionInfo_BoundingBox* const Self, const CRotatingCylinder* const Other, D3DXVECTOR3 OtherParentPos, D3DXVECTOR3 OtherParentRot)
{
	// === ����ɕK�v�ȏ���錾�A������ ===

	const D3DXVECTOR3& SelfPos = Self->GetPos();       // ���g�̈ʒu
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax(); // ���g�̍ő咸�_
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin(); // ���g�̍ŏ����_

	const D3DXVECTOR3& OtherPos = Other->GetPos();       // �~���̈ʒu
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // �~���̍ő咸�_
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // �~���̍ŏ����_
	const D3DXVECTOR3& OtherRot = Other->GetRot();       // �~���̉�]

	float fOtherRadius = OtherVtxMax.x; // �~���̔��a

	// ���̂W���_���`����
	D3DXVECTOR3 SelfBoxCornars[8];
	SelfBoxCornars[0] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMax.z); // ����O
	SelfBoxCornars[1] = SelfVtxMax; // �E��O
	SelfBoxCornars[2] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMax.y, SelfVtxMin.z); // �����
	SelfBoxCornars[3] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMax.y, SelfVtxMin.z); // �E���
	SelfBoxCornars[4] = D3DXVECTOR3(SelfVtxMin.x, SelfVtxMin.y, SelfVtxMax.z); // �����O
	SelfBoxCornars[5] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMax.z); // �E���O
	SelfBoxCornars[6] = SelfVtxMin; // ������
	SelfBoxCornars[7] = D3DXVECTOR3(SelfVtxMax.x, SelfVtxMin.y, SelfVtxMin.z); // �E����

	// === �~���i�e���܂ށj�̉�]�s�� ===

	// 
	D3DXMATRIX
		mtxParentRot,   // �e�̉�]�s��
		mtxParentTrans, // �e�̈ʒu�s��
		mtxOtherRot,    // ����̉�]�s��
		mtxOtherTrans;  // ����̈ʒu�s��

	// �e�̉�]�s������߂�
	D3DXMatrixRotationYawPitchRoll(&mtxParentRot, OtherParentRot.y, OtherParentRot.x, OtherParentRot.z);

	// �e�̈ʒu�s������߂�
	D3DXMatrixTranslation(&mtxParentTrans, OtherParentPos.x, OtherParentPos.y, OtherParentPos.z);

	// ����̉�]�s������߂�
	D3DXMatrixRotationYawPitchRoll(&mtxOtherRot, OtherRot.y, OtherRot.x, OtherRot.z);

	// ����̈ʒu�s������߂�
	D3DXMatrixTranslation(&mtxOtherTrans, OtherPos.x, OtherPos.y, OtherPos.z);

	// === �~���̃��[���h�s��(�q���̍s����|�����킹�Ă���e�̍s����|�����킹�܂��傤�j===

	D3DXMATRIX mtxOtherWorld = (mtxOtherRot * mtxOtherTrans) * (mtxParentRot * mtxParentTrans);

	// === �~���̋t�s��iAABB�����~���̃��[�J����Ԃɖ߂��j===

	D3DXMATRIX mtxInvOtherWorld;
	D3DXMatrixInverse(&mtxInvOtherWorld, NULL, &mtxOtherWorld);

	// === ���g��8���_���~���̃��[�J����Ԃɕϊ����AXZ�~�{Y�͈͂Ŕ��� ===

	// �P�ł����_���~�����ɓ����Ă����琬���Ƃ���
	for (int i = 0; i < 8; ++i)
	{
		D3DXVECTOR3
			worldPos = SelfBoxCornars[i] + SelfPos,        // ���g�̃��[���h���W
			localToCylinder; // �~������Ƃ������[�J�����W

		// �~����̈ʒu�ɕϊ����A�ʏ�̉~���̔���ŏo����悤�ɂ��܂��B
		D3DXVec3TransformCoord(&localToCylinder, &worldPos, &mtxInvOtherWorld);

		// �����`�F�b�N�i�ǂꂩ��ł������Ă����画�肷��̂�continue�ōς܂�)
		if (localToCylinder.z < OtherVtxMin.z || localToCylinder.z > OtherVtxMax.z)
		{
			continue;
		}

		// XZ���ʂł̋������`�F�b�N���A����̔��a���Z����Γ������Ă���
		float distXZ = sqrtf(localToCylinder.x * localToCylinder.x + localToCylinder.y * localToCylinder.y);
		if (distXZ <= fOtherRadius)
		{
			CRenderer::SetDebugString("���������I");
			return true;
		}
	}

	return false;
}

//=====================================================
// ���Δ������o������FX��
//=====================================================
void collision::PushOutBoxToBoxFunction::PushX(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other, bool& bSuccess)
{
	// ����
	const D3DXVECTOR3& SelfPos = Self->GetPos();       // �����̈ʒu
	const D3DXVECTOR3& SelfPosOld = Self->GetPosOld(); // ������1f�O�̈ʒu
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax(); // �����̍ő咸�_
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin(); // �����̍ŏ����_

	// ����
	const D3DXVECTOR3& OtherPos = Other->GetPos();       // ����̈ʒu
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // ����̍ő咸�_
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // ����̍ŏ����_

	if (SelfPos.x + SelfVtxMax.x > OtherPos.x + OtherVtxMin.x
		&& SelfPosOld.x + SelfVtxMax.x <= OtherPos.x + OtherVtxMin.x
		&& SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y
		&& SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y
		&& SelfPos.z + SelfVtxMax.z > OtherPos.z + OtherVtxMin.z
		&& SelfPos.z + SelfVtxMin.z < OtherPos.z + OtherVtxMax.z)
	{//�����̉E�[������̍��[�ɓ���������
		Self->SetPos(D3DXVECTOR3(OtherPos.x + OtherVtxMin.x - SelfVtxMax.x, SelfPos.y, SelfPos.z));
		Self->SetPushX(true);//X���ɉ����o���ꂽ
		bSuccess = true;// ����
	}
	else if (SelfPos.x + SelfVtxMin.x < OtherPos.x + OtherVtxMax.x
		&& SelfPosOld.x + SelfVtxMin.x >= OtherPos.x + OtherVtxMax.x
		&& SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y
		&& SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y
		&& SelfPos.z + SelfVtxMax.z > OtherPos.z + OtherVtxMin.z
		&& SelfPos.z + SelfVtxMin.z < OtherPos.z + OtherVtxMax.z)
	{//�����̍��[������̉E�[�ɓ���������
		Self->SetPos(D3DXVECTOR3(OtherPos.x + OtherVtxMax.x - SelfVtxMin.x, SelfPos.y, SelfPos.z));
		Self->SetPushX(true);//X���ɉ����o���ꂽ
		bSuccess = true;// ����
	}
}

//=====================================================
// ���Δ������o������FY��
//=====================================================
void collision::PushOutBoxToBoxFunction::PushY(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other, bool& bSuccess)
{
	// ����
	const D3DXVECTOR3& SelfPos = Self->GetPos();       // �����̈ʒu
	const D3DXVECTOR3& SelfPosOld = Self->GetPosOld(); // ������1f�O�̈ʒu
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax(); // �����̍ő咸�_
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin(); // �����̍ŏ����_

	// ����
	const D3DXVECTOR3& OtherPos = Other->GetPos();       // ����̈ʒu
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // ����̍ő咸�_
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // ����̍ŏ����_

		//��
	if (SelfPos.x + SelfVtxMax.x > OtherPos.x + OtherVtxMin.x
		&& SelfPos.x + SelfVtxMin.x < OtherPos.x + OtherVtxMax.x
		&& SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y
		&& SelfPosOld.y + SelfVtxMin.y >= OtherPos.y + OtherVtxMax.y
		&& SelfPos.z + SelfVtxMax.z > OtherPos.z + OtherVtxMin.z
		&& SelfPos.z + SelfVtxMin.z < OtherPos.z + OtherVtxMax.z)
	{//�����̉��[������̏�[�ɓ���������
		Self->SetPos(D3DXVECTOR3(SelfPos.x, OtherPos.y + OtherVtxMax.y - SelfVtxMin.y, SelfPos.z));
		Self->SetLanding(true); //�n�ʂɂ���
		bSuccess = true;// ����
	}
	//��
	else if (SelfPos.x + SelfVtxMax.x > OtherPos.x + OtherVtxMin.x
		&& SelfPos.x + SelfVtxMin.x < OtherPos.x + OtherVtxMax.x
		&& SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y
		&& SelfPosOld.y + SelfVtxMax.y <= OtherPos.y + OtherVtxMin.y
		&& SelfPos.z + SelfVtxMax.z > OtherPos.z + OtherVtxMin.z
		&& SelfPos.z + SelfVtxMin.z < OtherPos.z + OtherVtxMax.z)
	{//�����̏�[������̉��[�ɓ���������
		Self->SetPos(D3DXVECTOR3(SelfPos.x, OtherPos.y + OtherVtxMin.y - SelfVtxMax.y, SelfPos.z));
		bSuccess = true;// ����
	}
}

//=====================================================
// ���Δ������o������FZ��
//=====================================================
void collision::PushOutBoxToBoxFunction::PushZ(CCollisionInfo_BoundingBox* const Self, CCollisionInfo_BoundingBox* const Other, bool& bSuccess)
{
	// ����
	const D3DXVECTOR3& SelfPos = Self->GetPos();       // �����̈ʒu
	const D3DXVECTOR3& SelfPosOld = Self->GetPosOld(); // ������1f�O�̈ʒu
	const D3DXVECTOR3& SelfVtxMax = Self->GetVtxMax(); // �����̍ő咸�_
	const D3DXVECTOR3& SelfVtxMin = Self->GetVtxMin(); // �����̍ŏ����_

	// ����
	const D3DXVECTOR3& OtherPos = Other->GetPos();       // ����̈ʒu
	const D3DXVECTOR3& OtherVtxMax = Other->GetVtxMax(); // ����̍ő咸�_
	const D3DXVECTOR3& OtherVtxMin = Other->GetVtxMin(); // ����̍ŏ����_

	if (SelfPos.z + SelfVtxMax.z > OtherPos.z + OtherVtxMin.z
		&& SelfPosOld.z + SelfVtxMax.z <= OtherPos.z + OtherVtxMin.z
		&& SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y
		&& SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y
		&& SelfPos.x + SelfVtxMax.x > OtherPos.x + OtherVtxMin.x
		&& SelfPos.x + SelfVtxMin.x < OtherPos.x + OtherVtxMax.x)
	{//�����̑O�[������̌��[�ɓ���������
		Self->SetPos(D3DXVECTOR3(SelfPos.x, SelfPos.y, OtherPos.z + OtherVtxMin.z - SelfVtxMax.z));
		bSuccess = true;// ����
	}
	else if (SelfPos.z + SelfVtxMin.z < OtherPos.z + OtherVtxMax.z
		&& SelfPosOld.z + SelfVtxMin.z >= OtherPos.z + OtherVtxMax.z
		&& SelfPos.y + SelfVtxMax.y > OtherPos.y + OtherVtxMin.y
		&& SelfPos.y + SelfVtxMin.y < OtherPos.y + OtherVtxMax.y
		&& SelfPos.x + SelfVtxMax.x > OtherPos.x + OtherVtxMin.x
		&& SelfPos.x + SelfVtxMin.x < OtherPos.x + OtherVtxMax.x)
	{//�����̌��[������̑O�[�ɓ���������
		Self->SetPos(D3DXVECTOR3(SelfPos.x, SelfPos.y, OtherPos.z + OtherVtxMax.z - SelfVtxMin.z));
		bSuccess = true;// ����
	}
}
