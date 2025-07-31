//============================================================================
// 
// �Q�O�Q�T�N�U���P�O���F��]�~����AABB�̓����蔻����������� [Hole.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _HOLE_H_
#define _HOLE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

//****************************************************
// �O���錾
//****************************************************
class CRotatingCylinder; // ��]�~�����

//****************************************************
// ���N���X
//****************************************************
class CHole : public CObject_X
{
public:

	// === ����֐� ===

	CHole();// �R���X�g���N�^
	~CHole() override;	// �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	ExtraUpdate();	    // �X�V�����i�I�u�W�F�N�g�N���X�ŌĂ΂Ȃ��j
	void	Draw() override;	// �`�揈��

	// === �����o�֐� ===

	CRotatingCylinder* GetRotatingCylinder() const; // ��]�~�����擾

	void SetParentPos(D3DXVECTOR3 ParentPos); // �e�̈ʒu��ݒ�
	void SetParentRot(D3DXVECTOR3 ParentRot); // �e�̌�����ݒ�

	// === �ÓI�����o�֐� ===

	static CHole* Create(D3DXVECTOR3 InitPos); // ����

private:
	// === �����o�ϐ� ===

	CRotatingCylinder* m_pRotating_Cylinder; // ��]�~����� 
	D3DXVECTOR3
		m_ParentPos,   // �ʒu
		m_ParentRot;   // ����

	// === �����o�֐� ===

	void UpdateMtxWorld() override; // ���[���h�}�g���b�N�X��ݒ肷��
};

#endif // _PALYER_H_