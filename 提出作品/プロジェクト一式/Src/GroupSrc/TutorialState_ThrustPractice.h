//============================================================================
// 
// �Q�O�Q�T�N�V���Q�S���F�`���[�g���A������������ [TutorialState_ThrustPractice.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _TUTORIALSTATE_THRUSTPRACTICE_H_
#define _TUTORIALSTATE_THRUSTPRACTICE_H_	

//****************************************************
// �C���N���[�h
//****************************************************
#include "TutorialState.h"

//****************************************************
// �`���[�g���A����ԁF�u�˂����K�v�N���X
//****************************************************
class CTutorialState_ThrustPractice : public CTutorialState
{
public:
	// �R�����g���
	enum class COMMENT
	{
		GOOD = 0,
		BAD,
		MAX
	};

	// === ����֐� ===

	CTutorialState_ThrustPractice(CTutorial* pTutorial); // �R���X�g���N�^
	~CTutorialState_ThrustPractice() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init(CTutorial* pTutorial) override; // ����������
	void Uninit(CTutorial* pTutorial) override;  // �I������
	void Update(CTutorial* pTutorial) override; // �X�V����

private:
	// === �ÓI�����o�ϐ� ===

	static constexpr int
		s_nGoalSuccessNum = 5, // �ڕW�˂�������
		s_nInitNextSceneCntDown = 120; // ���̃V�[���܂ł̃J�E���g�_�E�������l

	// === �����o�ϐ� ===
	int
		m_nSuccessNum, // ������
		m_nNextSceneCntDown; // ���̃V�[���܂ł̃J�E���g�_�E��
	bool
		m_bStartPhaseOld, // 1f�O�Ƀt�F�[�Y���J�n���Ă��邩�ǂ���
		m_bStartPhase;    // �t�F�[�Y���J�n���Ă��邩�ǂ���
	JSON m_Json;     // �W�F�C�\��

	// === �����o�֐� ===

	void ExecuteReaction(); // �s���ɑ΂��ă��A�N�V�����𔭓�����
};
#endif // _COLLISION_H_