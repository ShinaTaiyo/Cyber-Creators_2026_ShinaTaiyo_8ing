//============================================================================
// 
// ���U���g�A�w�b�_�t�@�C�� [result.h]
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
// ���U���g�N���X
//****************************************************
class CResult final : public CScene
{
public:

	/**
	 * ���U���g�p�̍X�V�����ł��A
	 * �V�[���}�l�[�W���[�̍X�V�Ŏ����I�ɌĂяo����܂�
	 */
	void Update() override;

	/**
	 * ���U���g�p�̕`�揈���ł��A
	 * �V�[���}�l�[�W���[�̕`��Ŏ����I�ɌĂяo����܂�
	 */
	void Draw() override;

	/**
	 * �X�V���ɂ��̊֐����ĂԂƁA�ݒ肳��Ă��鎟�̃V�[���ɑJ�ڂ��܂�
	 * ��{�I�Ƀt���[���X�V�̍Ō�ɌĂяo���Ă�������
	 */
	void To_Next() override;

	/**
	 * ���U���g�V�[���̃C���X�^���X�𐶐����܂�
	 */
	static CResult* Create();

	// �C���Q�[���̘A�������񐔂̈��p��
	inline void SetPhaseComboCnt(int count) { m_nPheseComboCntKeep = count; }

	// �G���[�V�����̈��p��
	inline void SetEnemy(CMotion_Set* p) { m_pEnemy = p; }

	// �^�������[�V�����̈��p�� (����p�̂�)
	inline void SetFakeField(CMotion_Set* p) { m_pFakeField = p; }

	// �R�I�u�W�F�N�g�̈��p��
	void SetMountain(CObject_X* p);

private:

	//****************************************************
	// special function
	//****************************************************
	CResult();				// �f�t�H���g�R���X�g���N�^
	~CResult() override;	// �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************
	HRESULT Init() override;	// ����������
	void	Uninit() override;	// �I������
	void	UpdatePhase();		// �t�F�[�Y�̍X�V����

	// ���߉���ɍ��킹�ăJ�������Z�b�g
	void SetCamImpact(const char* key);

	//****************************************************
	// data
	//****************************************************
	int m_nPheseComboCntKeep;		// �C���Q�[���̘A��������
	CObject_HUD* m_pAtk;			// �U���Ñ�
	CObject_HUD* m_pTY;				// ����
	CMotion_Set* m_pEnemy;			// �G���[�V����
	CMotion_Set* m_pHuman;			// ��l�����[�V����
	CMotion_Set* m_pFakeField;		// �^�������[�V����
	CObject_X* m_pMountain;			// �R���[�V����
	CMotion_Set* m_pMountBreakable;	// �R(��)���[�V����

	int	m_nPhase;			// �C�x���g�t�F�[�Y
	int	m_nFrameCounter;	// �ėp�t���[���J�E�^�[
};