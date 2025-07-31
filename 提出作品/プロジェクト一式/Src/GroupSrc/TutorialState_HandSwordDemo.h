//============================================================================
// 
// �Q�O�Q�T�N�V���Q�Q���F�`���[�g���A������������ [TutorialState_HandSwordDemo.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _TUTORIALSTATE_HANDSWORDDEMO_H_
#define _TUTORIALSTATE_HANDSWORDDEMO_H_	

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
// �`���[�g���A����ԁF�u�蓁�f���v�N���X
//****************************************************
class CTutorialState_HandSwordDemo : public CTutorialState
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

	CTutorialState_HandSwordDemo(CTutorial* pTutorial); // �R���X�g���N�^
	~CTutorialState_HandSwordDemo() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init(CTutorial* pTutorial) override; // ����������
	void Uninit(CTutorial* pTutorial) override;  // �I������
	void Update(CTutorial* pTutorial) override; // �X�V����

private:
	// === �ÓI�����o�ϐ� ===
	static const std::string s_StrExplanation[static_cast<int>(EXPLANATION::MAX)];

	// === �����o�ϐ� ===
	int
		m_nChooseExplanation, // �����ԍ�
		m_nCntFrame; // �t���[�����J�E���g
	CButton* m_pButton; // �{�^���ւ̃|�C���^
	// === �����o�֐� ===
	void ChangeStrExplanation(CTutorial* pTutorial); // �����������ύX����

};
#endif // _COLLISION_H_