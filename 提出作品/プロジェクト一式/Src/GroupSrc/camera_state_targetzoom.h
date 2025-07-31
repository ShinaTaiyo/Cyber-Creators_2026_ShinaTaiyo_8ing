//============================================================================
// 
// �Q�O�Q�T�N�T���Q�O���F�J������ԃN���X����� [camera_state_targetzoom.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _CAMERA_STATE_TARGETZOOM_H_
#define _CAMERA_STATE_TARGETZOOM_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h
//****************************************************
#include "camera_state.h" // �J�������

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �J�����X�e�[�g�N���X
//****************************************************
class CCamera_State_TargetZoom : public CCamera_State
{
public:
	// === ����֐� ===

	// �R���X�g���N�^
	CCamera_State_TargetZoom(D3DXVECTOR3 GoalPos, // �ړI�n
		                     float fGoalDistance, // �ړI�̋���
		                     float fPosAdjust, // �ʒu�␳�l
		                     float fDistanceAdjust, // �����␳�l
		                     bool bAfterTurn,     // �������I�������Ɍ��̈ʒu�ɖ߂����ǂ��� 
		                     CCamera * pCamera);  // �J�����ւ̃|�C���^
	~CCamera_State_TargetZoom(); // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	void Update(CCamera* pCamera) override; // �X�V����

	// === �����o�֐� ===

private:
	// === �ÓI�ϐ� ===

	static constexpr float s_fPerDistance = 0.7f; // ���e���鋗��

	// === �ϐ� ===

	D3DXVECTOR3 m_GoalPos;  // �ڕW�̌���
	float 
		m_fGoalDistance,  // �ڕW�̈ʒu�Ƃ̋���
		m_fPosAdjust,     // �ʒu�␳�l
		m_fRotAdjust,     // �����␳�l
	    m_fDistanceAdjust; // �����␳�l
	bool m_bAfterTurn;      // �������I�������Ɍ��̈ʒu�ɖ߂����ǂ��� 

};
#endif // _Summon_MANAGER_H_