//============================================================================
// 
// �Q�O�Q�T�N�V���P�O���F�����֌W�̃o�O�̉��P�ƃv���C���[����������X�e�[�g�p�^�[���ɂ��� [PlayerState_BeforeThrust.h]
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
#include "PlayerState_BeforeThrust.h" // �˂��̑O���
#include "PlayerState_Thrusting.h" // �˂�
#include "PlayerState_Returning.h" // �߂�
#include "summon_manager_state.h" // �����}�l�[�W���[���
#include "summon_manager_state_start.h" // �����}�l�[�W���[�����J�n���
#include "camera_state_targetzoom.h" // �J�����Y�[�����
#include "camera_state_normal.h" // �J�����ʏ���

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
#include "sound.h" // �T�E���h

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
CPlayerState_BeforeThrust::CPlayerState_BeforeThrust(CPlayer* pPlayer, int nAnticipationFrame) :
	CPlayerState(pPlayer),
    m_nAnticipationFrame{ nAnticipationFrame },
	m_bStartAnticipation{false}
{
	// �����Ȃ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayerState_BeforeThrust::~CPlayerState_BeforeThrust()
{
	// �����Ȃ�
}

//============================================================================
// ����������
//============================================================================
void CPlayerState_BeforeThrust::Init(CPlayer* pPlayer)
{
	// �v���C���[�̃^�[�Q�b�g�𑀍�\�ɂ��A�J�����̈ʒu���^�[�Q�b�g��ɂ���
	CTarget* pTarget = pPlayer->GetTarget(); // �^�[�Q�b�g�ւ̃|�C���^
	pTarget->SetUseControl(true);
	pTarget->SetUseAdjustCameraPos(true);
	pTarget->SetUseMove(true);

	// �Q�[���̃J�����̏�Ԃ�ʏ�ɂ���
	CCamera_Manager::GetInstance()->RefSelectCamera().ChangeState(DBG_NEW CCamera_State_Normal());

	// �v���C���[��Ԃ����O�ŕ\��
	CLogger::GetInstance()->AddDuration(u8"�v���C���[���u�˂��O��ԁv�ɂȂ�܂����I", 1);
}

//============================================================================
// �I������
//============================================================================
void CPlayerState_BeforeThrust::Uninit(CPlayer* pPlayer)
{
	// �����Ȃ�
}

//============================================================================
// �X�V����
//============================================================================
void CPlayerState_BeforeThrust::Update(CPlayer* pPlayer)
{
	bool bDecisionGoal = false; // �ړI�n�����߂����ǂ���

	if (m_bStartAnticipation)
	{
		// �ړI�n�����߂鑀��
		bDecisionGoal = ControlDecisionGoal();
	}

	// �ʒu�Œ菈��
	FixedPos(pPlayer);

	// �������ꂽ�I�u�W�F�N�g���X���[��Ԃɂ���
	JudgeLerpPosToSummonObj(pPlayer);

	// ���؂肪�n�܂������Ɏ蓁���\�ɂ���
	if (m_bStartAnticipation)
	{
		if (HandSword(pPlayer)) // �蓁����
		{
			// �X�e�[�g���ς��̂ŏ������I��
			return;
		}
	}

	// �ړI�n�����肷��Ȃ�itrue�ɂȂ����u��)
	if (bDecisionGoal)
	{
		DecisionGoal(pPlayer); // �ړI�n���菈��

		return; // �ڕW�ʒu�����߂��̂ŏ������I������
	}

	// ���؂肪�J�n�����炻�̃t���[���������炷
	if (m_bStartAnticipation)
	{
		// ���؂�t���[���������炷
		if (m_nAnticipationFrame > 0)
		{
			m_nAnticipationFrame--;
		}
	}

	// �f�o�b�O�\��
	CRenderer::SetDebugString("�v���C���[�̏�ԁF�˂��O");
	CRenderer::SetDebugString("���؂�t���[�����F" + to_string(m_nAnticipationFrame));
}

//============================================================================
// ���؂�t���[�����擾
//============================================================================
const int& CPlayerState_BeforeThrust::GetAnticipationFrame() const
{
	return m_nAnticipationFrame;
}

//============================================================================
// ���؂�t���[�����ݒ�
//============================================================================
void CPlayerState_BeforeThrust::SetAnticipationFrame(int nFrame)
{
	m_nAnticipationFrame = nFrame;
}

//============================================================================
// �蓁�̃R�X�g���擾����
//===========================================================================
const int& CPlayerState_BeforeThrust::GetHandSwordCost()
{
	return s_nHandSwordCost;
}

//============================================================================
// �ʏ팩�؂�t���[�������擾����
//===========================================================================
const int& CPlayerState_BeforeThrust::GetNormalAnticipationFrame()
{
	return s_nNormalAnticipationFrame;
}

//============================================================================
// �蓁����
//============================================================================
bool CPlayerState_BeforeThrust::HandSword(CPlayer* pPlayer)
{
	CInputKeyboard* pInputKeyboard = CManager::GetManager()->GetKeyboard(); // �L�[���͏��
	auto* pPad = CManager::GetManager()->GetPad(); // �p�b�h���͏��
	int nAirPower = pPlayer->GetAirPower(); // �C
	int nHandSwordCost = s_nHandSwordCost; // �蓁�R�X�g

	// �蓁�p���[���K��l�ɒB���Ă��Ȃ���Ζ���
	if (nAirPower < nHandSwordCost)
	{
		return false;
	}

	// �蓁�p���[�����ȏ㑶�݂��Ă����Ԃ�X�������A�܂��͉E�X�e�B�b�N��|���Ǝ蓁���J�n����
	if (pInputKeyboard->GetTrigger(DIK_X) ||
		pPad->GetTrigger(CInputPad::JOYKEY::X) ||
		abs(pPad->GetJoyStickR().X) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		abs(pPad->GetJoyStickR().Y) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
#if 1
		// �U��SE�Đ�
		WORD wRand = rand() % 3;
		CSound::LABEL Label = CSound::LABEL::ATK_0;
		switch (wRand)
		{
		case 0:		Label = CSound::LABEL::ATK_0;	break;
		case 1:		Label = CSound::LABEL::ATK_1;	break;
		case 2:		Label = CSound::LABEL::ATK_2;	break;
		default:	Label = CSound::LABEL::ATK_0;	break;
		}
		CSound::GetInstance()->Play(Label);
#endif
		m_nAnticipationFrame = 0; // �������؂�K�v���Ȃ��̂Ō��؂�t���[�������O�ɂ���
		pPlayer->ExecuteUpSpace(); // �蓁���o������
		CTarget* pTarget = pPlayer->GetTarget(); // �^�[�Q�b�g�ւ̃|�C���^
		CTransparent_Info* pTargetTransparentInfo = pTarget->GetTransparent_Info(); // �^�[�Q�b�g�̓����x���
		pTargetTransparentInfo->SetSpeed(-pTargetTransparentInfo->GetValue() / 10); // �c��̓����x���P�O�t���[���őS�Č��炷

		// �����}�l�[�W���[�ւ̃|�C���^�擾
		CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));

		// �����}�l�[�W���[�̏����J�n��Ԃւ̃|�C���^
		CSummon_Manager_State_Start* pSummonManagerState_Start = dynamic_cast<CSummon_Manager_State_Start*>(pSummonManager->GetState());

		if (pSummonManagerState_Start)
		{
			CSummonGroupSelector* pSummonGroupSelector = pSummonManagerState_Start->GetSummonGroupSelector(); // �����O���[�v�I���ւ̃|�C���^
			int nDifficultyScore = pSummonGroupSelector->GetDifficultyScore(); // ��Փx�X�R�A
			pSummonManagerState_Start->GetSummonGroupSelector()->SetDifficultyScore(nDifficultyScore - 4);
		}
		else
		{
			// �����J�n��Ԃ��Ȃ��̂łȂɂ����Ȃ�
		}

		pPlayer->SetAirPower(nAirPower - nHandSwordCost); // �C�����炷

		CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // �f�t�H���g���C���[�̐擪���X�g���擾

		// ��Q���ƃ]�[�������X�g���猟��������
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();// ���̃I�u�W�F�N�g���擾

			// ��ނ���Q���Ȃ�
			if (pObj->GetType() == CObject::TYPE::OBSTACLE || pObj->GetType() == CObject::TYPE::HOLEOBSTACLE)
			{
				CObstacle* pObstacle = dynamic_cast<CObstacle*>(pObj);

				// ��Q�����Ȃ������̂Ŏ��Ƀ��X�g��i�߂�
				if (pObstacle == nullptr)
				{
					pObj = pNext;
					continue;
				}
				// ��Q�����j��\�Ȃ�j��
				if (pObstacle->GetCanbeDestroy())
				{
					pObstacle->SetRelease();
				}
			}
			// ��ނ��]�[���Ȃ�
			else if (pObj->GetType() == CObject::TYPE::ZONE)
			{
				pObj->SetRelease(); // �j������
			}

			pObj = pNext;//���X�g�����ɐi�߂�
		}

		// �߂��ԂɕύX
		pPlayer->SetState(DBG_NEW CPlayerState_Returning(pPlayer));

		return true;
	}

	return false;
}

//============================================================================
// �ʒu�Œ菈��
//============================================================================
void CPlayerState_BeforeThrust::FixedPos(CPlayer* pPlayer)
{
	D3DXVECTOR3 SelfPos = pPlayer->GetPos(); // ���g�̈ʒu
	// �Q�[���J�����ւ̃|�C���^���擾
	CCamera* pCamera = &CCamera_Manager::GetInstance()->RefSelectCamera();
	Calculation::Pos::ScreenToWorld(&SelfPos, FSCREEN_WIDTH + 100.0f, FSCREEN_HEIGHT - 100.0f, 0.75f, 1280, 720,
		pCamera->GetMtxView(), pCamera->GetMtxProj());

	pPlayer->SetPos(SelfPos); // �ʒu�ݒ�
}

//============================================================================
// �����I�u�W�F�N�g����`��Ԃ����鏈�������邩�ǂ������f����
//============================================================================
void CPlayerState_BeforeThrust::JudgeLerpPosToSummonObj(CPlayer* pPlayer)
{
	// ���؂肪�J�n���Ă���Ȃ珈�������Ȃ�
	if (m_bStartAnticipation)
	{
		return;
	}

	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // �擪�I�u�W�F�N�g���擾
	CTarget* pTarget = pPlayer->GetTarget(); // �^�[�Q�b�g�ւ̃|�C���^

	// �X���[��Ԃɂ���I�u�W�F�N�g�����X�g���猟������
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // ���X�g�̎��̃|�C���^
		D3DXVECTOR3 OtherTargetPos = VEC3_INIT; // ����̖ڕW�ʒu
		float fOtherVtxMin_Z = 0.0f; // ����̍ŏ����_Z
		CObject::TYPE OtherType = pObj->GetType(); // ����̃^�C�v
		CSummonCommonData* pSummonCommonData = nullptr; // �����I�u�W�F�N�g���ʃf�[�^�ւ̃|�C���^

		// �����}�l�[�W���[���珢�������I�u�W�F�N�g�ȊO�Ȃ烊�X�g�����ɐi�߂�
		if (OtherType != CObject::TYPE::OBSTACLE &&
			OtherType != CObject::TYPE::HOLEOBSTACLE &&
			OtherType != CObject::TYPE::ZONE)
		{
			pObj = pNext;
			continue;
		}

		// �^�C�v���u��Q���v���́u��������Q���v�Ȃ炻�̈ʒu���擾����
		if (OtherType == CObject::TYPE::OBSTACLE ||
			OtherType == CObject::TYPE::HOLEOBSTACLE)
		{
			CObstacle* pObstacle = dynamic_cast<CObstacle*>(pObj); // ��Q���ւ̃|�C���^
			OtherTargetPos = pObstacle->GetPos(); // ����̈ʒu�擾
			fOtherVtxMin_Z = pObstacle->GetVtxMin().z; // ����̍ŏ����_�擾
			pSummonCommonData = &pObstacle->GetSummonCommonData(); // �����I�u�W�F�N�g���ʃf�[�^�ւ̃|�C���^�擾
		}
		// �^�C�v���u�]�[���v�Ȃ炻�̈ʒu���擾����
		else if (OtherType == CObject::TYPE::ZONE)
		{
			CZone* pZone = dynamic_cast<CZone*>(pObj); // �]�[���ւ̃|�C���^
			OtherTargetPos = pZone->GetPos(); // ����̈ʒu�擾
			pSummonCommonData = &pZone->GetSummonCommonData(); // �����I�u�W�F�N�g���ʃf�[�^�ւ̃|�C���^�擾
		}

		// �������ꂽ�I�u�W�F�N�g�̎�O�����^�[�Q�b�g��Z�l�̋߂��܂ŗ����猩�؂�J�n
		if (OtherTargetPos.z + fOtherVtxMin_Z < pTarget->GetPos().z + s_fSummonObjectSlowRange)
		{
			m_bStartAnticipation = true;
		}

		pObj = pNext; // ���X�g�����ɐi�߂�
	}

	if (m_bStartAnticipation)
	{
		SetLerpPosToSummonObj(pPlayer);
	}
}

//============================================================================
// �����I�u�W�F�N�g�̈ʒu����`��Ԃ�����
//============================================================================
void CPlayerState_BeforeThrust::SetLerpPosToSummonObj(CPlayer* pPlayer)
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // �擪�I�u�W�F�N�g���擾

	// �^�[�Q�b�g�̓����x
	CTransparent_Info* pTargetTransParentInfo = pPlayer->GetTarget()->GetTransparent_Info();
	pTargetTransParentInfo->SetMaxValue(m_nAnticipationFrame);
	pTargetTransParentInfo->SetValue(m_nAnticipationFrame);
	pTargetTransParentInfo->SetSpeed(-1); // �l�����炷���x
	pTargetTransParentInfo->SetUse(true); // �^�[�Q�b�g�̓����x���g�p����

	// �X���[��Ԃɂ���I�u�W�F�N�g�����X�g���猟������
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // ���X�g�̎��̃I�u�W�F�N�g
		const CObject::TYPE OtherType = pObj->GetType();     // ����̃^�C�v
		CSummonCommonData* pOtherSummonCommonData = nullptr; // �����I�u�W�F�N�g���ʃf�[�^�ւ̃|�C���^
		D3DXVECTOR3 OtherPos = VEC3_INIT; // ����̈ʒu

		// �����}�l�[�W���[���珢�������I�u�W�F�N�g�ȊO�Ȃ烊�X�g�����ɐi�߂�
		if (OtherType != CObject::TYPE::OBSTACLE &&
			OtherType != CObject::TYPE::HOLEOBSTACLE &&
			OtherType != CObject::TYPE::ZONE)
		{
			pObj = pNext;
			continue;
		}

		// �^�C�v���u��Q���v���́u��������Q���v�Ȃ炻�̈ʒu���擾����
		if (OtherType == CObject::TYPE::OBSTACLE ||
			OtherType == CObject::TYPE::HOLEOBSTACLE)
		{
			CObstacle* pObstacle = dynamic_cast<CObstacle*>(pObj); // ��Q���ւ̃|�C���^
			pOtherSummonCommonData = &pObstacle->GetSummonCommonData(); // �����I�u�W�F�N�g���ʃf�[�^�ւ̃|�C���^�擾
			OtherPos = pObstacle->GetPos(); // ����̈ʒu�擾
		}
		// �^�C�v���u�]�[���v�Ȃ炻�̈ʒu���擾����
		else if (OtherType == CObject::TYPE::ZONE)
		{
			CZone* pZone = dynamic_cast<CZone*>(pObj); // �]�[���ւ̃|�C���^
			pOtherSummonCommonData = &pZone->GetSummonCommonData(); // �����I�u�W�F�N�g���ʃf�[�^�ւ̃|�C���^�擾
			OtherPos = pZone->GetPos(); // ����̈ʒu�蓁��
		}

		pOtherSummonCommonData->GetMoveInfo().SetUseMove(false);
		CVec3LerpInfo& LerpPosInfo = pOtherSummonCommonData->GetLerpPosInfo();   // �����I�u�W�F�N�g���ʃf�[�^�ʒu���`��ԏ��擾
		D3DXVECTOR3 OtherMove = pOtherSummonCommonData->GetMoveInfo().GetMove(); // �����I�u�W�F�N�g���ʃf�[�^�ړ��ʎ擾
		D3DXVec3Normalize(&OtherMove, &OtherMove); // �ړ��ʂ𐳋K��

		// �ړ������Ɍ������Ďw�肵�������܂Ő��`��Ԃ����܂�
		LerpPosInfo.SetInitialize(m_nAnticipationFrame, OtherPos, OtherPos + OtherMove * s_fSummonObjLerpLength);

		pObj = pNext; // ���X�g�����ɐi�߂�
	}
}

//============================================================================
// �ړI�n���菈��
//============================================================================
void CPlayerState_BeforeThrust::DecisionGoal(CPlayer* pPlayer)
{
	CCamera * pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game"); // ���݂̃J�����ւ̃|�C���^
	CTarget* pTarget = pPlayer->GetTarget(); // �^�[�Q�b�g�ւ̃|�C���^
	D3DXVECTOR3 TargetPos = pPlayer->GetTarget()->GetPos(); // �^�[�Q�b�g�̈ʒu

	ClearLerpPosToSummonObj(); // �����I�u�W�F�N�g�ʒu���`��Ԃ���������
	StopObstacleAddRot();      // ��Q���̌����̉��Z���~�߂�

    // �J������ڕW�ʒu�փY�[����Ԃɂ���
	pCamera->ChangeState(
		DBG_NEW CCamera_State_TargetZoom(TargetPos, 50.0f, s_fCameraZoom_PosAdjust, s_fCameraZoom_DistanceAdjust, false, pCamera));

	// �v���C���[�̃^�[�Q�b�g�𑀍�s�\�ɂ��A�J�������^�[�Q�b�g��ɂ��Ȃ�
	pTarget->SetUseControl(false);
	pTarget->SetUseAdjustCameraPos(false);
	pTarget->SetUseMove(false);

	// �^�[�Q�b�g�̈ʒu��_���ē˂���Ԃɕς���
	pPlayer->SetState(DBG_NEW CPlayerState_Thrusting(pPlayer, TargetPos));

	// �^�[�Q�b�g�̎c��̓����x���P�O�t���[���őS�Č��炷
	CTransparent_Info* pTargetTransparentInfo = pTarget->GetTransparent_Info();
	pTargetTransparentInfo->SetSpeed(-pTargetTransparentInfo->GetValue() / 10);
}

//============================================================================
// �ړI�n�����߂鑀����s��
//============================================================================
bool CPlayerState_BeforeThrust::ControlDecisionGoal()
{
	// �L�[���͏��ւ̃|�C���^�擾
	CInputKeyboard* pInputKeyboard = CManager::GetManager()->GetKeyboard();
	auto* pPad = CManager::GetManager()->GetPad(); // �p�b�h���͏��

	// �g���K�[�{�^������������ړI�n������
	if (pInputKeyboard->GetTrigger(DIK_J) ||
		pPad->GetTrigger(CInputPad::JOYKEY::A))
	{
#if 0
		// �U��SE�Đ�
		WORD wRand = rand() % 3;
		CSound::LABEL Label = CSound::LABEL::ATK_0;
		switch (wRand)
		{
		case 0:		Label = CSound::LABEL::ATK_0;	break;
		case 1:		Label = CSound::LABEL::ATK_1;	break;
		case 2:		Label = CSound::LABEL::ATK_2;	break;
		default:	Label = CSound::LABEL::ATK_0;	break;
		}
		CSound::GetInstance()->Play(Label);
#endif
		return true;
	}

	// ���؂�t���[�������Ȃ��Ȃ����玩���I�ɖړI�n������
	if (m_nAnticipationFrame == 0)
	{
		return true;
	}

	return false;
}

//============================================================================
// �����I�u�W�F�N�g���`��ԏ�������������
//============================================================================
void CPlayerState_BeforeThrust::ClearLerpPosToSummonObj()
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // �擪�I�u�W�F�N�g���擾

	// �X���[��Ԃɂ���I�u�W�F�N�g�����X�g���猟������
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // ���X�g�̎��̃I�u�W�F�N�g
		const CObject::TYPE OtherType = pObj->GetType();     // ����̃^�C�v
		CSummonCommonData* pOtherSummonCommonData = nullptr; // �����I�u�W�F�N�g���ʃf�[�^�ւ̃|�C���^

		// �����}�l�[�W���[���珢�������I�u�W�F�N�g�ȊO�Ȃ烊�X�g�����ɐi�߂�
		if (OtherType != CObject::TYPE::OBSTACLE &&
			OtherType != CObject::TYPE::HOLEOBSTACLE &&
			OtherType != CObject::TYPE::ZONE)
		{
			pObj = pNext;
			continue;
		}

		// �^�C�v���u��Q���v���́u��������Q���v�Ȃ炻�̈ʒu���擾����
		if (OtherType == CObject::TYPE::OBSTACLE ||
			OtherType == CObject::TYPE::HOLEOBSTACLE)
		{
			CObstacle* pObstacle = dynamic_cast<CObstacle*>(pObj); // ��Q���ւ̃|�C���^
			pOtherSummonCommonData = &pObstacle->GetSummonCommonData(); // �����I�u�W�F�N�g���ʃf�[�^�ւ̃|�C���^�擾
        }
		// �^�C�v���u�]�[���v�Ȃ炻�̈ʒu���擾����
		else if (OtherType == CObject::TYPE::ZONE)
		{
			CZone* pZone = dynamic_cast<CZone*>(pObj); // �]�[���ւ̃|�C���^
			pOtherSummonCommonData = &pZone->GetSummonCommonData(); // �����I�u�W�F�N�g���ʃf�[�^�ւ̃|�C���^�擾
		}

		// �ړ�������
		pOtherSummonCommonData->GetMoveInfo().SetUseMove(true);

		// �ʒu�̐��`��Ԃ�����
		pOtherSummonCommonData->GetLerpPosInfo().SetUse(false);

		pObj = pNext; // ���X�g�����ɐi�߂�
	}

	SummonObjSlow(0);     // �����I�u�W�F�N�g���X���[��Ԃɂ���
	SummonObjClearSlow(); // �����I�u�W�F�N�g�X���[��Ԃ����̊֐��ŏ��X�ɉ���
}

//============================================================================
// ��Q���̉�]���~�߂�
//============================================================================
void CPlayerState_BeforeThrust::StopObstacleAddRot()
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // �擪�I�u�W�F�N�g���擾
    // �X���[��Ԃɂ���I�u�W�F�N�g�����X�g���猟������
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // ���X�g�̎��̃|�C���^
		CObject::TYPE OtherType = pObj->GetType(); // ����̃^�C�v

		// �^�C�v���u��Q���v�u��������Q���v�ȊO�Ȃ烊�X�g�����ɐi�߂�
		if (OtherType != CObject::TYPE::OBSTACLE &&
			OtherType != CObject::TYPE::HOLEOBSTACLE)
		{
			pObj = pNext;
			continue;
		}

		// ��Q���Ƀ_�E���L���X�g
		CObstacle* pObstacle = dynamic_cast<CObstacle*>(pObj);

		// ��]���~�߂�
		pObstacle->SetUseAddRot(false);

		pObj = pNext; // ���X�g�����ɐi�߂�
	}
}

//============================================================================
// �����I�u�W�F�N�g���X���[��Ԃɂ���
//============================================================================
void CPlayerState_BeforeThrust::SummonObjSlow(int nSlowFrame)
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // �擪�I�u�W�F�N�g���擾

// ���X�g����������
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // ���X�g�̎��̃I�u�W�F�N�g���擾

		// �^�C�v���������ꂽ����������U�O�t���[���x������
		if (pObj->GetType() == CObject::TYPE::OBSTACLE ||
			pObj->GetType() == CObject::TYPE::ZONE ||
			pObj->GetType() == CObject::TYPE::HOLEOBSTACLE)
		{
			if (nSlowFrame > 0)
			{
				pObj->GetTimeScaleInfo()->SetTimeScaleChangeFrame(0.02f, nSlowFrame);
			}
			else
			{
				pObj->GetTimeScaleInfo()->SetTimeScale(0.02f);
			}
		}

		pObj = pNext; // ���X�g�����ɐi�߂�
	}
}

//============================================================================
// �����I�u�W�F�N�g�̃X���[��Ԃ�����
//============================================================================
void CPlayerState_BeforeThrust::SummonObjClearSlow()
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // �擪�I�u�W�F�N�g���擾

    // ���X�g���������A�����I�u�W�F�N�g�̃^�C���X�P�[����߂�
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // ���X�g�̎��̃I�u�W�F�N�g���擾

		// ���X�Ɍ��̃^�C���X�P�[���ɖ߂�
		if (pObj->GetType() == CObject::TYPE::OBSTACLE ||
			pObj->GetType() == CObject::TYPE::ZONE ||
			pObj->GetType() == CObject::TYPE::HOLEOBSTACLE)
		{
			pObj->GetTimeScaleInfo()->SetGoalTimeScale(1.0f, 0.1f);
		}

		pObj = pNext; // ���X�g�����ɐi�߂�
	}
}
