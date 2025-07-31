//============================================================================
// 
// �Q�O�Q�T�N�V���Q�S���F�`���[�g���A������������ [TutorialState_HandSwordPractice.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _TUTORIALSTATE_HANDSWORDPRACTICE_H_
#define _TUTORIALSTATE_HANDSWORDPRACTICE_H_	

//****************************************************
// �C���N���[�h
//****************************************************
#include "TutorialState.h"

//****************************************************
// �O���錾
//****************************************************
class CButton; // �{�^��

//****************************************************
// �`���[�g���A����ԁF�u�˂����K�v�N���X
//****************************************************
class CTutorialState_HandSwordPractice : public CTutorialState
{
public:
	// �R�����g���
	enum class COMMENT
	{
	    INTRO = 0, // ����
		TAKOSIA,   // ��������
		KONOMAMAZYA, // ���̂܂܂��Ⴀ����
		EXECUTE, // �蓁�����R�����g
		SUMMARY, // �܂Ƃ�
		MAX
	};

	// === ����֐� ===

	CTutorialState_HandSwordPractice(CTutorial* pTutorial); // �R���X�g���N�^
	~CTutorialState_HandSwordPractice() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init(CTutorial* pTutorial) override; // ����������
	void Uninit(CTutorial* pTutorial) override;  // �I������
	void Update(CTutorial* pTutorial) override; // �X�V����

private:
	// === �ÓI�����o�ϐ� ===
	
	// === �����o�ϐ� ===
	int
		m_nCommentNum, // ���݂̃R�����g�ԍ�
		m_nCommentDispFrame; // �R�����g�\���t���[��
	bool
		m_bExecuteHandSword; // �蓁�𔭓����邩�ǂ���
	JSON m_Json;       // �W�F�C�\��
    // === �����o�֐� ===
	void ChangeStrExplanation(CTutorial * pTutorial); // �����������ύX����
};
#endif // _COLLISION_H_