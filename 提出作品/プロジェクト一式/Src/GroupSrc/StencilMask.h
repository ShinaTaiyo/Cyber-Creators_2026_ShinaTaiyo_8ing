//============================================================================
// 
// �Q�O�Q�T�N�U���Q�U���F�X�e���V���o�b�t�@�����̂��߃}�X�N�p�|���S����`�� [StencilMask.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _STENCILMASK_H_
#define _STENCILMASK_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_2D.h" // �I�u�W�F�N�g2D

//****************************************************
// �X�e���V���}�X�N�N���X
//****************************************************
class CStencilMask : public CObject_2D
{
public:
	// === ����֐� ===

	CStencilMask();	// �f�t�H���g�R���X�g���N�^
	~CStencilMask() override;	// �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// === �ÓI�����o�֐� ===

	static CStencilMask* Create(D3DXVECTOR3 InitPos);	// ����
	static void SetOtherStencilState(); // ���̃I�u�W�F�N�g���X�e���V���o�b�t�@�̐ݒ���s��
	static void SetOtherStencilAvoid(); // �X�e���V���}�X�N���������Ȃ��I�u�W�F�N�g�̕`��ŌĂт܂�
	static void SetOtherResetStencilState(); // ���̃I�u�W�F�N�g���X�e���V���o�b�t�@�̐ݒ��߂�
	static void SetUseStencil(bool bUse); // �X�e���V���o�b�t�@���g�p���邩�ݒ�
private:
	// === �ÓI�����o�ϐ� ===

	static bool s_bStencil; // �X�e���V���o�b�t�@���g�p���邩�ǂ���

	// === �����o�֐� ===

	void SetStencilState();   // �X�e���V���o�b�t�@�̐ݒ���s��
	void ResetStencilState(); // �X�e���V���o�b�t�@�̐ݒ�����ɖ߂�
};

#endif // _PALYER_H_