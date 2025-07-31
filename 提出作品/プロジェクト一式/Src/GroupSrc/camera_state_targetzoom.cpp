//============================================================================
// 
// �Q�O�Q�T�N�T���Q�O���F�J������ԃN���X����� [Camera_State_TargetZoom_targetzoom.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "camera_state_targetzoom.h" // ���w�b�_
#include "camera_state_normal.h" // �J�����ʏ���
#include "camera.h"    // �J����  
#include "pch.h"       // �l�X�ȃC���N���[�h�����g��
#include "renderer.h"  // �f�o�b�O�Ȃ�
#include "calculation.h" // �v�Z
#include "logger.h" // ���O

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;


//============================================================================
//
// public�����o
//
//============================================================================

//====================================================
// �R���X�g���N�^
//====================================================
CCamera_State_TargetZoom::CCamera_State_TargetZoom(D3DXVECTOR3 GoalPos, // �ړI�n
	float fGoalDistance,
	float fPosAdjust,
	float fDistanceAdjust,
	bool bAfterTurn,  
	CCamera* pCamera) : 
	m_GoalPos{ GoalPos },
	m_fGoalDistance{fGoalDistance},
	m_fPosAdjust{fPosAdjust},
	m_fDistanceAdjust{fDistanceAdjust},
	m_bAfterTurn{bAfterTurn}
{
	// �J������Ԃ����O�ŕ\��
	CLogger::GetInstance()->AddDuration(u8"�J�������u�Y�[����ԁv�ɂȂ�܂����I", 1);
}

//====================================================
// �f�X�g���N�^
//====================================================
CCamera_State_TargetZoom::~CCamera_State_TargetZoom()
{
	// �����Ȃ�
}

//====================================================
// �X�V����
//====================================================
void CCamera_State_TargetZoom::Update(CCamera* pCamera)
{
	// === �����Ɏg�p�������錾 ===

	const D3DXVECTOR3&
		Pos = pCamera->GetPos(), // �ʒu
		Rot = pCamera->GetRot(); // ����

	D3DXVECTOR3
		GoalVec = m_GoalPos - Pos; // �ړI�n�ւ̃x�N�g��

	const float&
		fDistance = pCamera->GetDistance(), // ���_�ƒ����_�̋���
		fDistanceDiff = m_fGoalDistance - fDistance; // �ڕW�̋����Ƃ̍���

	pCamera->SetDistance(fDistance + fDistanceDiff * m_fDistanceAdjust); // �ړI�̋����܂ŏk�ߑ�����

	// ���������ɖ߂�
	D3DXVECTOR3
		VecDiffToZero = VEC3_INIT - Rot;
	pCamera->SetRot(D3DXVECTOR3(Rot.x + VecDiffToZero.x * 0.1f, Rot.y + VecDiffToZero.y * 0.1f, 0.0f));
    
	// ���X�ɖړI�n�Ɋ񂹂Ă����܂�
	pCamera->SetPos(Pos + GoalVec * m_fPosAdjust); // �ʒu�����X�ɖړI�n�֊񂹂�

	// �����̍��������ȉ��ɂȂ�����ړI�̋����ɐݒ肵�Ēʏ��Ԃɖ߂�
	if (fDistance < m_fGoalDistance + s_fPerDistance && 
		fDistance > m_fGoalDistance - s_fPerDistance)
	{
		pCamera->SetDistance(m_fGoalDistance);
		pCamera->ChangeState(DBG_NEW CCamera_State_Normal());
	}

	// �f�o�b�O�\��
	CRenderer::SetDebugString("�ڕW�n�_�Ƃ̋����F" + to_string(fDistance));
	CRenderer::SetDebugString("�ڕW�n�_�ւ̃x�N�g���FX = " + to_string(GoalVec.x) + "�AY = " + to_string(GoalVec.y) + "�AZ = " + to_string(GoalVec.z));
}
