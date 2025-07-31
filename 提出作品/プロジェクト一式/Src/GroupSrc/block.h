//============================================================================
// 
// �Q�O�Q�T�N�S���Q�W���F�u���b�N�N���X���쐬 [block.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _BLOCK_H_
#define _BLOCK_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h" // �I�u�W�F�N�gX

//****************************************************
// �O���錾
//****************************************************
class CCollisionInfo_BoundingBox; // ��������
class CCollisionInfo; // ������

//****************************************************
// �v���C���[�N���X
//****************************************************
class CBlock : public CObject_X
{
public:

	// === ����֐� ===

	CBlock();				// �R���X�g���N�^
	~CBlock() override;	// �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��
	
	// === �����o�֐� ===

	CCollisionInfo* GetCollisionInfo() const;   // ������擾

	// === �ÓI�����o�֐� ===

	static CBlock* Create(D3DXVECTOR3 InitPos);	// ����

private:

	// === �����o�ϐ� ===

	CCollisionInfo_BoundingBox * m_pCollisionInfo_Box; // ��������
};

#endif // _PALYER_H_