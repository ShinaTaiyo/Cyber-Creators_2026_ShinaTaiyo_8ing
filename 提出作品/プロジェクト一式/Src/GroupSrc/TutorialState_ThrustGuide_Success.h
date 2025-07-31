//============================================================================
// 
// �Q�O�Q�T�N�V���Q�Q���F�`���[�g���A������������ [TutorialState_ThrustGuide_Success.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _TUTORIALSTATE_THRUSTGUIDE_SUCCESS_H_
#define _TUTORIALSTATE_THRUSTGUIDE_SUCCESS_H_	

//****************************************************
// �C���N���[�h
//****************************************************
#include "TutorialState.h"

//****************************************************
// �ÓI�����o�錾
//****************************************************
class CButton; // �{�^��

//****************************************************
// using�f�B���N�e�B�u
//****************************************************

//****************************************************
// �`���[�g���A����ԁF�u�蓁�K�C�h�����v�N���X
//****************************************************
class CTutorialState_ThrustGuide_Success : public CTutorialState
{
public:

	// �������
	enum class EXPLANATION
	{
		START = 0,
		DEMO_1,
		DEMO_2,
		MAX
	};

	// === ����֐� ===

	CTutorialState_ThrustGuide_Success(CTutorial* pTutorial); // �R���X�g���N�^
	~CTutorialState_ThrustGuide_Success() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init(CTutorial* pTutorial) override; // ����������
	void Uninit(CTutorial* pTutorial) override;  // �I������
	void Update(CTutorial* pTutorial) override; // �X�V����

private:
	// === �����o�ϐ� ===
	int
		m_nChooseExplanation, // �����ԍ�
		m_nCntFrame; // �t���[�����J�E���g
	bool m_bMotion; // ���[�V�������邩�ǂ���
	JSON m_JsonExplanation; // �����W�F�C�\���t�@�C��
	// === �����o�֐� ===
	bool ChangeStrExplanation(CTutorial* pTutorial); // �����������ύX����

};
#endif // _COLLISION_H_