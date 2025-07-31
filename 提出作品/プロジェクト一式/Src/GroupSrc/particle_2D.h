//============================================================================
// 
// �Q�O�Q�T�N�T���P�Q���F�^�C�}�[�ɂ��V�[���J�ڎ��� [particle_2D.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _PARTICLE_2D_H_
#define _PARTICLE_2D_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_2D.h"

//****************************************************
// �O���錾
//****************************************************
class CMove_Info; // �ړ����

//****************************************************
// �v���C���[�N���X
//****************************************************
class CParticle_2D : public CObject_2D
{
public:
	// <special function>
	CParticle_2D();				// �R���X�g���N�^
	~CParticle_2D() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <getter/setter>
	const int& GetLife() const; // �̗͎擾
	void SetLife(int nLife);    // �̗͐ݒ�

	// <getter/setter>
	const int& GetMaxLife() const; // �ő�̗͎擾
	void SetMaxLife(int nMaxLife); // �ő�̗͐ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetInitSize() const; // �ŏ��̃T�C�Y�擾
	void SetInitSize(D3DXVECTOR3 InitSize); // �ŏ��̃T�C�Y�ݒ�

	// <getter>
	CMove_Info* GetMoveInfo() const; // �ړ������擾����

	// <static function>
	static CParticle_2D* Create(D3DXCOLOR Col,D3DXVECTOR3 InitPos);	// ����
private:
	D3DXVECTOR3 m_InitSize;      // �ŏ��̃T�C�Y
	CMove_Info* m_pMove_Info;    // �ړ����
	int m_nLife;                 // �̗�
	int m_nMaxLife;              // �ő�̗�
};

#endif // _PALYER_H_