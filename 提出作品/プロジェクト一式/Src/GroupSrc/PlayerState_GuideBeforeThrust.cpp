//============================================================================
// 
// �Q�O�Q�T�N�V���Q�R���F�`���[�g���A������������ [PlayerState_GuideBeforeThrust.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h
//****************************************************

// === �I�u�W�F�N�g ===

#include "player.h" // �v���C���[
#include "target.h" 
#include "obstacle.h" // ��Q��
#include "HoleObstacle.h" // ��������Q��
#include "zone.h" // �]�[��
#include "summon_manager.h" // �����}�l�[�W���[
#include "object.h"
#include "SummonGroupSelector.h" // �����O���[�v�I��
#include "particle.h" // �^�[�Q�b�g�ʒu�m�F�p

// === ��� ===
#include "PlayerState_GuideBeforeThrust.h" // �˂��̑O���
#include "PlayerState_Thrusting.h" // �˂�
#include "PlayerState_Returning.h" // �߂�
#include "summon_manager_state.h" // �����}�l�[�W���[���
#include "summon_manager_state_start.h" // �����}�l�[�W���[�����J�n���
#include "camera_state_targetzoom.h" // �J�����Y�[�����
#include "camera_state_normal.h" // �J�����ʏ���
#include "summon_manager_state_choose.h" // �����}�l�[�W���[�����I�����

// === ��� ===

#include "transparent_info.h" // �����x���
#include "game.h" // �Q�[���V�[��
#include "manager.h" // �}�l�[�W���[
#include "input.h" // ����
#include "renderer.h" // �`��
#include "logger.h" // ���O
#include "camera.h" // �J����
#include "move_info.h" // �ړ����
#include "calculation.h" // �v�Z

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;
using namespace std;

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CPlayerState_GuideBeforeThrust::CPlayerState_GuideBeforeThrust(CPlayer* pPlayer, int nAnticipationFrame) :
	m_nNowMotion{0},
	m_nMotionFrame{0},
	m_GoalPos{VEC3_INIT},
	m_Vec3LerpInfo{},
	m_bNowEndMotionAfterThrust{false},
	CPlayerState_BeforeThrust(pPlayer,nAnticipationFrame)
{
	// �����Ȃ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayerState_GuideBeforeThrust::~CPlayerState_GuideBeforeThrust()
{
	// �����Ȃ�
}

//============================================================================
// ����������
//============================================================================
void CPlayerState_GuideBeforeThrust::Init(CPlayer* pPlayer)
{
	CTarget* pTarget = pPlayer->GetTarget();
	pTarget->SetUseControl(false);
	pTarget->SetUseMove(true);

	std::fstream ReadingFile; // �t�@�C���ǂݍ��ݗp

	// === ���[�V�����W�F�C�\���t�@�C���ǂݍ��� ===

	ReadingFile.open("Data\\JSON\\PlayerState_GuideBeforeThrust\\GuideMotion.Json", std::ios::in);

	// �t�@�C��������ɊJ���Ă����珢���p�^�[�����X�g���i�[
	if (ReadingFile.good())
	{
		ReadingFile >> m_JsonMotion; // �W�F�C�\���i�[

		// �z�񂶂�Ȃ������珈�����I��
		if (!m_JsonMotion.is_array())
		{
			assert(false && "�����p�^�[�����X�g���z��^���Ⴀ��܂���");
		}
	}

	ReadingFile.close(); // �t�@�C�������

	if (!m_JsonMotion[m_nNowMotion].contains("LerpFrame"))
	{
		assert(false && "�v���C���[�̃K�C�h�˂��O��Ԃ̃��[�V�����̕�ԃt���[�����̃L�[�����݂��Ȃ�");
	}

	int nLerpFrame = m_JsonMotion[m_nNowMotion]["LerpFrame"];
	vector<float> vGoalPos = m_JsonMotion[m_nNowMotion]["GoalPos"];
	m_Vec3LerpInfo.SetInitialize(nLerpFrame, pPlayer->GetTarget()->GetPos(),
		D3DXVECTOR3(vGoalPos[0], vGoalPos[1], vGoalPos[2]));
}

//============================================================================
// �I������
//============================================================================
void CPlayerState_GuideBeforeThrust::Uninit(CPlayer* pPlayer)
{
	// �����Ȃ�
}

//============================================================================
// �X�V����
//============================================================================
void CPlayerState_GuideBeforeThrust::Update(CPlayer* pPlayer)
{
	// ���`��ԏ���
	LerpProcess(pPlayer);

	// ���[�V���������ɐi�߂�i��Ԃ��ς������true���Ԃ���A�������I������j
	if (NextMotion(pPlayer))
	{
		return;
	}

	// ���N���X�X�V
	CPlayerState_BeforeThrust::Update(pPlayer);

	// ���[�V�����t���[�����J�E���g�A�b�v
	m_nMotionFrame++;
}

//============================================================================
// ���`��ԏ���
//============================================================================
void CPlayerState_GuideBeforeThrust::LerpProcess(CPlayer* pPlayer)
{
	CTarget* pTarget = pPlayer->GetTarget();
	D3DXVECTOR3 TargetPos = pTarget->GetPos();
	m_Vec3LerpInfo.Update(TargetPos, pTarget);
	pTarget->SetPos(TargetPos);

	CRenderer::SetDebugString("�ڕW�ʒu�FX = " + to_string(m_GoalPos.x) +
		"Y = " + to_string(m_GoalPos.y) + "Z = " + to_string(m_GoalPos.z));
}

//============================================================================
// �ړI�n�����߂鑀��
//============================================================================
bool CPlayerState_GuideBeforeThrust::ControlDecisionGoal()
{
	return false;
}

//============================================================================
// �蓁����
//============================================================================
bool CPlayerState_GuideBeforeThrust::HandSword(CPlayer* pPlayer)
{
	// ���̏�Ԃł͔������Ȃ�
	return false;
}

//============================================================================
// ���[�V���������ɐi�߂�
//============================================================================
bool CPlayerState_GuideBeforeThrust::NextMotion(CPlayer* pPlayer)
{
	// ����g���L�[���ǂꂩ��ł��Ȃ��������O����
	if (!m_JsonMotion[m_nNowMotion].contains("LerpFrame") ||
		!m_JsonMotion[m_nNowMotion].contains("GoalPos") ||
		!m_JsonMotion[m_nNowMotion].contains("ExecuteThrust"))
	{
		assert(false && "�v���C���[�̃K�C�h�˂��O��Ԃ̃��[�V�����̎g�p����L�[�ő��݂��Ȃ����̂�����I");
	}

	int nLerpFrame = m_JsonMotion[m_nNowMotion]["LerpFrame"];

	// ���`��ԃt���[�����ɒB�����烂�[�V���������ɐi�߂�
	if (m_nMotionFrame >= nLerpFrame)
	{
		// ����̃��[�V�������I�������˂��𔭓�����Ȃ���
		if (m_bNowEndMotionAfterThrust)
		{
			DecisionGoal(pPlayer);
			return true;
		}

		m_nNowMotion++;
		int nSize = m_JsonMotion.size();
		if (m_nNowMotion == nSize)
		{
			m_nNowMotion = 0;
		}
		CLogger::GetInstance()->AddDuration(u8"�ǂݍ��񂾃��[�V�����F" + to_string(m_nNowMotion),5);
		m_nMotionFrame = 0; // ���[�V�����t���[�����Z�b�g

		// ���`��ԃt���[����
		nLerpFrame = m_JsonMotion[m_nNowMotion]["LerpFrame"];

		// ���݂̃��[�V�������I�������˂��𔭓����邩�ǂ���
		m_bNowEndMotionAfterThrust = m_JsonMotion[m_nNowMotion]["ExecuteThrust"];

		// �ړI�n
		vector<float> vGoalPos = m_JsonMotion[m_nNowMotion]["GoalPos"];
		m_GoalPos = D3DXVECTOR3(vGoalPos[0], vGoalPos[1], vGoalPos[2]);

		// �ǂݍ��񂾏����g���Đ��`��ԏ���ύX
		m_Vec3LerpInfo.SetInitialize(nLerpFrame,pPlayer->GetTarget()->GetPos(),m_GoalPos);
	}

	return false;
}
