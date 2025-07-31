//============================================================================
// 
// �h��Đ� [swaying_player.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "swaying_player.h"
#include "manager.h"

// �V�[��
#include "game.h"

// ���O�o�͗p
#include "logger.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �ÓI�����o�萔�̒�`
//****************************************************

// �r���[�㏑���p�p�����[�^�̕ێ�
const std::array<JSON, 2> CSwayingPlayer::s_aOverWriteViewParam = {
	hws::SafeOpenJsonFile("Data\\JSON\\INITIALIZE\\BEZIER\\Play.json"),
	hws::SafeOpenJsonFile("Data\\JSON\\INITIALIZE\\BEZIER\\Piston.json")
};

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// SWAY_STATE :: �X�V
//============================================================================
bool CSwayingPlayer::SWAY_STATE::UpdateState(CSwayingPlayer*& refSwayingPlayer)
{
	up<SWAY_STATE>& refState = refSwayingPlayer->m_upState;

	if (!refState)
	{
		return false;
	}

	if (refState->Execute(refSwayingPlayer))
	{
		refState->Change(refState);
	}

	return true;
}

//============================================================================
// SWAY_STATE :: ���s
//============================================================================
bool CSwayingPlayer::SWAY_STATE::Execute(CSwayingPlayer*& refSwayingPlayer)
{
	throw std::runtime_error("'CSpace::SWAY_STATE::Execute()' was Failed : " + static_cast<std::string>(typeid(refSwayingPlayer).name()));
}

//============================================================================
// SWAY_STATE :: �ύX
//============================================================================
void CSwayingPlayer::SWAY_STATE::Change(useful::up<SWAY_STATE>& refState)
{
	throw std::runtime_error("'CSpace::SWAY_STATE::Change()' was Failed : " + static_cast<std::string>(typeid(refState).name()));
}

//============================================================================
// SWAY_INTRO :: ���s
//============================================================================
bool CSwayingPlayer::SWAY_INTRO::Execute(CSwayingPlayer*& refSwayingPlayer)
{
	CCamera* pCam = refSwayingPlayer->m_pMainCamera;

	// �㏑�����������ݒ�̏ꍇ
	if (!pCam->GetStateOverwriteView())
	{
		// �w�������̎g�p
		pCam->SetUseDecayState(true);

		// �r���[�㏑�������邪�A�J�����C���X�^���X�̃L���v�`�������Ȃ�
		pCam->OverwriteView(
			[](CCamera& refCam)
			{
				// �w���������g�p
				refCam.SetUseDecayState(true);

				// �ڕW�������쐬
				Vec3 RotTarget = { 0.0f, 0.0f, 0.0f };
				refCam.SetRotTarget(RotTarget);

				// �ڕW�ʒu���r���[�㏑���p�����[�^�́A1�ڂ̐���_�̍��W�ɐݒ�
				refCam.SetPosTarget(hws::JsonConvertToVec3(s_aOverWriteViewParam[0]["Pos"][0]));

				// �ڕW�������Œ�
				refCam.SetDistanceTarget(400.0f);
			});
	}

	// �󂯎���Ă��鏢���N�[���^�C���J�E���g���A�K��l�ɓ��B�ŏ�ԕύX
	if (refSwayingPlayer->m_nCoolTimeCnt)
	{
		// �ύX��A����X�V���̂݃r���[�㏑�������邽�߂�nullptr���w��
		pCam->OverwriteView(nullptr);

		return true;
	}

	return false;
}

//============================================================================
// SWAY_INTRO :: �ύX
//============================================================================
void CSwayingPlayer::SWAY_INTRO::Change(useful::up<SWAY_STATE>& refState)
{
	ChangeUniquePtr<SWAY_STATE, SWAY_PLAY>(refState);
}

//============================================================================
// SWAY_PLAY :: ���s
//============================================================================
bool CSwayingPlayer::SWAY_PLAY::Execute(CSwayingPlayer*& refSwayingPlayer)
{
	CCamera* pCam = refSwayingPlayer->m_pMainCamera;

	// �㏑�����������ݒ�̏ꍇ
	if (!pCam->GetStateOverwriteView())
	{
		// �x�W�F�Ȑ��̏���������
		m_Bezier.Initialize();

		// �e����_���W�̐ݒ�
		for (unsigned short wCntLoop = 0; wCntLoop < 3; ++wCntLoop)
		{
			Vec3 ControlPos = hws::JsonConvertToVec3(s_aOverWriteViewParam[0]["Pos"][wCntLoop]);

			m_Bezier.SetPosToControlPointForSelectedIdx(ControlPos, wCntLoop);
		}

		// �i�s���x�̐ݒ�
		m_Bezier.SetSpeed(static_cast<float>(s_aOverWriteViewParam[0]["Speed"]));

		// ���]�g�p�̃t���O�𗧂Ă�
		m_Bezier.SetUseReverse(true);

		// �w���������g�p
		pCam->SetUseDecayState(true);

		// �r���[�㏑�������邪�A�J�����C���X�^���X�̃L���v�`�������Ȃ�
		pCam->OverwriteView(
			[this](CCamera& refCam)
			{
				// �ڕW��]�x�N�g�����Œ�l�ō쐬
				Vec3 RotTarget = VEC3_INIT;
				refCam.SetRotTarget(RotTarget);

				// �x�W�F�Ȑ��̍X�V
				m_Bezier.Update();

				// �O�Ղ̍��W���擾
				Vec3 TrajectoryPos = m_Bezier.GetPosTracePointOnTrajectoryLine();

				// �ʒu���O�Ղ̍��W�ɓ���
				refCam.SetPosTarget(TrajectoryPos);

				// �ڕW�������Œ�
				refCam.SetDistanceTarget(400.0f);
			});
	}

	// �󂯎���Ă��鏢���N�[���^�C���J�E���g���A�K��l�ɓ��B�ŏ�ԕύX
	if (refSwayingPlayer->m_nCoolTimeCnt <= NUM_MAXCOUNT)
	{
		// �ύX��A����X�V���̂݃r���[�㏑�������邽�߂�nullptr���w��
		pCam->OverwriteView(nullptr);

		return true;
	}

	return false;
}

//============================================================================
// SWAY_PLAY :: �ύX
//============================================================================
void CSwayingPlayer::SWAY_PLAY::Change(useful::up<SWAY_STATE>& refState)
{
	ChangeUniquePtr<SWAY_STATE, SWAY_OUTRO>(refState);
}

//============================================================================
// SWAY_OUTRO :: ���s
//============================================================================
bool CSwayingPlayer::SWAY_OUTRO::Execute(CSwayingPlayer*& refSwayingPlayer)
{
	CCamera* pCam = refSwayingPlayer->m_pMainCamera;

	// �㏑�����������ݒ�̏ꍇ
	if (!pCam->GetStateOverwriteView())
	{
		// �x�W�F�Ȑ��̏���������
		m_Bezier.Initialize();

		// �e����_���W�̐ݒ�
		for (unsigned short wCntLoop = 0; wCntLoop < 3; ++wCntLoop)
		{
			Vec3 ControlPos = hws::JsonConvertToVec3(s_aOverWriteViewParam[1]["Pos"][wCntLoop]);

			m_Bezier.SetPosToControlPointForSelectedIdx(ControlPos, wCntLoop);
		}

		// �i�s���x�̐ݒ�
		m_Bezier.SetSpeed(static_cast<float>(s_aOverWriteViewParam[1]["Speed"]));

		// �w�������̎g�p
		pCam->SetUseDecayState(true);

		// �r���[�㏑�������邪�A�J�����C���X�^���X�̃L���v�`�������Ȃ�
		pCam->OverwriteView(
			[this](CCamera& refCam)
			{
				// �w���������g�p
				refCam.SetUseDecayState(true);

				// �x�W�F�Ȑ��̍X�V
				m_Bezier.Update();

				// �O�Ղ̍��W���擾
				Vec3 TrajectoryPos = m_Bezier.GetPosTracePointOnTrajectoryLine();

				// �ʒu���O�Ղ̍��W�ɓ���
				refCam.SetPosTarget(TrajectoryPos);

				// �ڕW�������Œ�
				refCam.SetDistanceTarget(400.0f);
			});
	}

	// �I�����薳���A�O�����疾���I�ɉ�����A��Ԃ����Z�b�g���Ă�������
	return false;
}

//============================================================================
// SWAY_OUTRO :: �ύX
//============================================================================
void CSwayingPlayer::SWAY_OUTRO::Change(useful::up<SWAY_STATE>& refState)
{}

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CSwayingPlayer::CSwayingPlayer() :
	m_upState(nullptr),
	m_pMainCamera(nullptr),
	m_nCoolTimeCnt(0)
{}

//============================================================================
// �h��
//============================================================================
bool CSwayingPlayer::Sway(int coolTimeCnt)
{
	// �h���Ԃ���������Ă��Ȃ����
	if (!m_upState)
	{
		// �C���g���ɕύX
		m_upState = make_unique<SWAY_INTRO>();
	}

	// ���̎��g�p����Ă��郁�C���J�����̏���n��
	m_pMainCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game");

	// �����N�[���^�C�����R�s�[
	m_nCoolTimeCnt = coolTimeCnt;

	// this�|�C���^���獶�Ӓl�̃|�C���^��
	CSwayingPlayer* Tmp = this;

	// ��Ԃ̍X�V
	SWAY_STATE::UpdateState(Tmp);

#ifdef _DEBUG
	std::string tmpStr = to_string(coolTimeCnt);
	if (typeid(*m_upState) == typeid(SWAY_INTRO))
		CLogger::GetInstance()->AddSynchronization(u8"�C���g�� : " + tmpStr);
	else if (typeid(*m_upState) == typeid(SWAY_PLAY))
		CLogger::GetInstance()->AddSynchronization(u8"�Đ� : " + tmpStr);
	else if (typeid(*m_upState) == typeid(SWAY_OUTRO))
		CLogger::GetInstance()->AddSynchronization(u8"�A�E�g�� : " + tmpStr);
#endif // _DEBUG

	return false;
}