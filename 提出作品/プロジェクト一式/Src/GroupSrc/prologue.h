//============================================================================
// 
// �v�����[�O�A�w�b�_�t�@�C�� [prologue.h]
// Author : ���c����
// 
//============================================================================

// �C���N���[�h�K�[�h
#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "scene.h"

//****************************************************
// �O���錾
//****************************************************
class CObject_HUD;
class CObject_X;
class CMotion_Set;

//****************************************************
// �v�����[�O�N���X
//****************************************************
class CPrologue : public CScene
{
public:

	// <function>
	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��
	void To_Next() override;	// ���̃V�[����

	// <static function>
	static CPrologue* Create();	// �v�����[�O�𐶐�

private:

	// <special function>
	CPrologue();			// �R���X�g���N�^
	~CPrologue() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	UpdateText();		// �e�L�X�g�̍X�V����
	void	UpdatePhase();		// �t�F�[�Y�̍X�V����
	void	SetLeaves();		// ��ʂ̗t���σZ�b�g
	void	UpdateLeaves();		// ��ʂ̗t���ύX�V����

	//****************************************************
	// data
	//****************************************************
	int						m_nPhase;			// �C�x���g�t�F�[�Y
	int						m_nFrameCounter;	// �ėp�t���[���J�E�^�[
	CObject_HUD*			m_pText;			// �e�L�X�g
	CMotion_Set*			m_pEnemy;			// �G�̃��[�V�����Z�b�g
	CMotion_Set*			m_pHuman;			// ��l���̃��[�V�����Z�b�g
	CObject_X*				m_pGold;			// ���C���S�b�g
	std::vector<CObject_X*> m_vpLeaves;			// ��ʂ̗t����
	float					m_fAlphaRate;		// �H
};