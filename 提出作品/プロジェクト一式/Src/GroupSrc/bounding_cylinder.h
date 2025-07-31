//============================================================================
// 
// �Q�O�Q�T�N�S���Q�W���F�R���W�����n�N���X [bounding_Cylinder.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _BOUNDING_Cylinder_H_
#define _BOUNDING_Cylinder_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "collision.h" //Collision���N���X������̂�

//****************************************************
// �V�����_�[���E�����蔻��N���X
//****************************************************
class CCollisionInfo_BoundingCylinder : public CCollisionInfo
{
public:
	CCollisionInfo_BoundingCylinder();                       // �R���X�g���N�^
	~CCollisionInfo_BoundingCylinder() override;             // �f�X�g���N�^

	//< Setter>
	void SetUpdateInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld,D3DXVECTOR3 Size, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin,
		bool bIsLanding, bool bIsLandingOld);// ���X�V�p�i����O�ɑS�Ă̏����蓮�ōX�V�j
	//< Setter/Getter>
	void SetSize(const D3DXVECTOR3 Size);                   // �T�C�Y�ݒ�
	const D3DXVECTOR3& GetSize() const;                     // �T�C�Y�擾

	//<setter/getter>
	void SetVtxMax(D3DXVECTOR3 VtxMax);                     // �ő咸�_�ݒ�
	const D3DXVECTOR3& GetVtxMax() const;                   // �ő咸�_�擾

	//<setter/getter>
	void SetVtxMin(D3DXVECTOR3 VtxMin);                     // �ŏ����_�ݒ�
	const D3DXVECTOR3& GetVtxMin() const;                   // �ŏ����_�擾
private:
	D3DXVECTOR3 
		m_Size,   // �T�C�Y
	    m_VtxMax, // �ő咸�_
	    m_VtxMin; // �ŏ����_
};
#endif // _COLLISION_H_