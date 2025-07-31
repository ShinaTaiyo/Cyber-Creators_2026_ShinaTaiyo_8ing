//============================================================================
// 
// �Q�O�Q�T�N�S���Q�W���F�R���W�����n�N���X [bounding_Cylinder.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "bounding_Cylinder.h"//���w�b�_

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//============================================================================
// �R���X�g���N�^
//============================================================================
CCollisionInfo_BoundingCylinder::CCollisionInfo_BoundingCylinder() :
	m_Size(VEC3_INIT),
	m_VtxMax(VEC3_INIT),
	m_VtxMin(VEC3_INIT)
{
	SetType(CCollisionInfo::TYPE::CYLINDER); //����^�C�v���u�V�����_�[�v�ɐݒ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CCollisionInfo_BoundingCylinder::~CCollisionInfo_BoundingCylinder()
{

}

//============================================================================
// ���X�V�p�i����O�ɑS�Ă̏����蓮�ōX�V�j
//============================================================================
void CCollisionInfo_BoundingCylinder::SetUpdateInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld, D3DXVECTOR3 Size, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin,
	bool bIsLanding, bool bIsLandingOld)
{
	m_Size = Size;     // �T�C�Y
	m_VtxMax = VtxMax; // �ő咸�_
	m_VtxMin = VtxMin; // �ŏ����_
	CCollisionInfo::SetUpdateInfo(Pos, PosOld, bIsLanding, bIsLandingOld); // ���N���X�̏��X�V
}

//============================================================================
// �T�C�Y�ݒ�
//============================================================================
void CCollisionInfo_BoundingCylinder::SetSize(const D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// �T�C�Y�擾
//============================================================================
const D3DXVECTOR3& CCollisionInfo_BoundingCylinder::GetSize() const
{
	return m_Size;
}

//============================================================================
// �ő咸�_�ݒ�
//============================================================================
void CCollisionInfo_BoundingCylinder::SetVtxMax(D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//============================================================================
// �ő咸�_�擾
//============================================================================
const D3DXVECTOR3& CCollisionInfo_BoundingCylinder::GetVtxMax() const
{
	return m_VtxMax;
}

//============================================================================
// �ŏ����_�ݒ�
//============================================================================
void CCollisionInfo_BoundingCylinder::SetVtxMin(D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//============================================================================
// �ŏ����_�擾
//============================================================================
const D3DXVECTOR3& CCollisionInfo_BoundingCylinder::GetVtxMin() const
{
	return m_VtxMin;
}