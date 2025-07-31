//============================================================================
// 
// �Q�O�Q�T�N�U���P�O���F��]�~����AABB�̓����蔻����������� [bgmodel.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _BGMODEL_H_
#define _BGMODEL_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

//****************************************************
// �O���錾
//****************************************************
class CRotatingCylinder; // ��]�~�����
class CCollisionInfo; // ������
class CMove_Info; // �ړ����

//****************************************************
// �v���C���[�N���X
//****************************************************
class CBgModel : public CObject_X
{
public:

	// === ����֐� ===

	CBgModel();				// �R���X�g���N�^
	~CBgModel() override;	// �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// === �����o�֐� ===

	// �������Z��
	void SetAddRot(D3DXVECTOR3 AddRot);   // �ݒ�
	const D3DXVECTOR3& GetAddRot() const; // �擾

	CCollisionInfo* GetCollisionInfo() const; // ������擾
	CMove_Info* GetMoveInfo() const; // �ړ����擾

	// === �ÓI�����o�֐� ===

	static CBgModel* Create(D3DXVECTOR3 InitPos); // ����

private:
	// === �����o�ϐ� ===

	D3DXVECTOR3 m_AddRot; // ���Z�������
	CRotatingCylinder * m_pRotating_Cylinder; // ��]�~����� 
	CMove_Info* m_pMoveInfo; // �ړ����
};

#endif // _PALYER_H_