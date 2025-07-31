//============================================================================
// 
// �Q�O�Q�T�N�V���P�O���F�����֌W�̃o�O�̉��P�ƃv���C���[����������X�e�[�g�p�^�[���ɂ��� [PlayerState_Returning.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h
//****************************************************
#include "PlayerState_Returning.h"
#include "camera.h"
#include "GamePlayer.h"
#include "calculation.h"
#include "camera_manager.h"
#include "camera_state_targetzoom.h"
#include "target.h"
#include "game.h"
#include "summon_manager_state_start.h"
#include "transparent_info.h"
#include "PlayerState_BeforeThrust.h"
#include "renderer.h" // �`��
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

//============================================================================
// �R���X�g���N�^
//============================================================================
CPlayerState_Returning::CPlayerState_Returning(CPlayer* pPlayer) :
	CPlayerState(pPlayer),
	m_GoalPos{VEC3_INIT},
	m_bIsReturnComplete{false}
{
	// �����Ȃ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayerState_Returning::~CPlayerState_Returning()
{
	// �����Ȃ�
}

//============================================================================
// ����������
//============================================================================
void CPlayerState_Returning::Init(CPlayer* pPlayer)
{
	CCamera* pCamera = &CCamera_Manager::GetInstance()->RefSelectCamera();
	pCamera->ChangeState(DBG_NEW CCamera_State_TargetZoom(VEC3_INIT, CCamera::GetInitDistance(), 0.2f, 0.2f, false, pCamera));
	CTarget* pTarget = pPlayer->GetTarget(); // �^�[�Q�b�g�ւ̃|�C���^
	pTarget->SetPosTarget(VEC3_INIT); // ���S�ɖ߂�
	pTarget->SetUseMove(true); // �ړ��o����悤�ɂ���
	pTarget->SetUseControl(false); // �܂����삳���Ȃ�

	// �^�[�Q�b�g�̓����x���U�O�t���[���Ō��ɖ߂�
	CTransparent_Info* pTargetTransparentInfo = pTarget->GetTransparent_Info();
	pTargetTransparentInfo->SetSpeed(pTargetTransparentInfo->GetMaxValue() / 60);

	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	// �����}�l�[�W���[�̃I�u�W�F�N�g�͂��̏�ԂɂȂ����u�Ԃɏ���
	while (pObj)
	{
		CObject* pNext = pObj->GetNext();

		if (pObj->GetType() == CObject::TYPE::OBSTACLE ||
			pObj->GetType() == CObject::TYPE::HOLEOBSTACLE ||
			pObj->GetType() == CObject::TYPE::ZONE)
		{
			pObj->SetRelease();
		}

		pObj = pNext;
	}

	// �v���C���[��Ԃ����O�ŕ\��
	CLogger::GetInstance()->AddDuration(u8"�v���C���[���u�߂��ԁv�ɂȂ�܂����I", 1);
}

//============================================================================
// �I������
//============================================================================
void CPlayerState_Returning::Uninit(CPlayer* pPlayer)
{
	// �����Ȃ�
}

//============================================================================
// �X�V����
//============================================================================
void CPlayerState_Returning::Update(CPlayer* pPlayer)
{
	// �ړI�n������
	CCamera* pCamera = &CCamera_Manager::GetInstance()->RefSelectCamera(); // ���݂̃J�����ւ̃|�C���^
	Calculation::Pos::ScreenToWorld(&m_GoalPos, FSCREEN_WIDTH + 100.0f, FSCREEN_HEIGHT - 100.0f, 0.75f, 1280, 720,
		pCamera->GetMtxView(), pCamera->GetMtxProj());

	const D3DXVECTOR3& SelfPos = pPlayer->GetPos(); // ���g�̈ʒu
	D3DXVECTOR3 VecSelfToGoal = m_GoalPos - SelfPos; // ���g����ړI�n�ւ̃x�N�g��

	// �w�������ŖړI�n�ֈړ�����
	pPlayer->SetPos(SelfPos + VecSelfToGoal * s_fPlayerMoveAdjust);

	VecSelfToGoal = m_GoalPos - SelfPos; // ���g����ړI�n�ւ̃x�N�g���X�V

	float fVecLength = D3DXVec3Length(&VecSelfToGoal); // ���g����ړI�n�ւ̃x�N�g���̒���

	// �ړI�n�ւ̋��������e���������߂��Ȃ����猳�̈ʒu�֖߂銮���t���O��true�ɂ���
	if (fVecLength < s_fPlayerGoalToleranceDistance)
	{
		m_bIsReturnComplete = true;
	}

	// �f�o�b�O�\��
	CRenderer::SetDebugString("�v���C���[�̏�ԁF�߂�");
}

//============================================================================
// ���̈ʒu�֖߂�̂������������ǂ����擾
//============================================================================
const bool& CPlayerState_Returning::GetIsReturnComplete() const
{
	return m_bIsReturnComplete;
}
