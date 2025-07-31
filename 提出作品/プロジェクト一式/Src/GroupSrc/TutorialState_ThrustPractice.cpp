//============================================================================
// 
// �Q�O�Q�T�N�V���Q�Q���F�`���[�g���A������������ [TutorialState_ThrustPractice.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h
//****************************************************
#include "TutorialState_ThrustPractice.h"
#include "PlayerState_GuideBeforeThrust.h"
#include "summon_manager_state_start.h"
#include "PlayerState_Returning.h"
#include "TutorialState_HandSwordPractice.h"
#include "summon_manager_state_choose.h"
#include "tutorial.h"
#include "Pch.h"
#include "calculation.h"
#include "UiText.h"
#include "manager.h"
#include "player.h"
#include "renderer.h"
#include "input.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;
using namespace std;

//****************************************************
// �ÓI�����o�ϐ�
//****************************************************

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CTutorialState_ThrustPractice::CTutorialState_ThrustPractice(CTutorial* pTutorial) :
	m_nSuccessNum{ 0 },
	m_bStartPhaseOld{false},
	m_bStartPhase{false},
	m_nNextSceneCntDown{s_nInitNextSceneCntDown},
	CTutorialState{ pTutorial }
{
	// �����Ȃ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTutorialState_ThrustPractice::~CTutorialState_ThrustPractice()
{
	// �����Ȃ�
}

//============================================================================
// ����������
//============================================================================
HRESULT CTutorialState_ThrustPractice::Init(CTutorial* pTutorial)
{
	std::fstream ReadingFile; // �t�@�C���ǂݍ��ݗp

    // === �˂����K�W�F�C�\���t�@�C���ǂݍ��� ===

	ReadingFile.open("Data\\JSON\\Tutorial\\ThrustPractice.Json", std::ios::in);

	// �t�@�C��������ɊJ���Ă����珢���p�^�[�����X�g���i�[
	if (ReadingFile.good())
	{
		ReadingFile >> m_Json; // �W�F�C�\���i�[

		// �z�񂶂�Ȃ������珈�����I��
		if (!m_Json.is_object())
		{
			assert(false && "�˂����K��Ԃ̃W�F�C�\�����I�u�W�F�N�g�^���Ⴀ��܂���");
		}
	}

	ReadingFile.close(); // �t�@�C�������

	// �����}�l�[�W���[�ւ̃|�C���^���擾���A�����J�n��Ԃɕς���
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));
	pSummonManager->SetChengeState(DBG_NEW CSummon_Manager_State_Start(999));

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER));

	// �v���C���[�ւ̃|�C���^�����݂���Ȃ�v���C���[��߂��Ԃɂ���
	if (pPlayer)
	{
		pPlayer->SetState(DBG_NEW CPlayerState_Returning(pPlayer));
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CTutorialState_ThrustPractice::Uninit(CTutorial* pTutorial)
{
	// �����Ȃ�
}

//============================================================================
// �X�V����
//============================================================================
void CTutorialState_ThrustPractice::Update(CTutorial* pTutorial)
{
	// ���݂̖ړI��\������
	CRenderer::GetRenderer()->GetUiTextManager().Add(
		DBG_NEW CUIText("�˂��̗��K�����Ă݂悤�I", WSCREEN_WIDTH / 2, 100,
			50,1, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));

	// �����J�n��Ԃւ̃|�C���^���擾����
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));
	CSummon_Manager_State_Start* pStart = dynamic_cast<CSummon_Manager_State_Start*>(pSummonManager->GetState());
	CRenderer* pRenderer = CRenderer::GetRenderer(); // �`����ւ̃|�C���^���擾
	CUITextManager & pUiTextManager = pRenderer->GetUiTextManager(); // UI�e�L�X�g�}�l�[�W���[���擾

	if (pStart)
	{
		if (pStart->GetPhaseJudgmentFrame())
		{
			if (pStart->GetSuccessPhase())
			{
				pUiTextManager.Add(
					DBG_NEW CUIText("�������ł��I", WSCREEN_WIDTH / 2, WSCREEN_HEIGHT / 2 - 150,
						100, 100, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));
				m_nSuccessNum++; // �����񐔃C���N�������g
			}
			else
			{
				pUiTextManager.Add(
					DBG_NEW CUIText("�������܂��E�E�E", WSCREEN_WIDTH / 2, WSCREEN_HEIGHT / 2 - 150,
						100, 100, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));
			}
		}
	}

	// �˂����J��o�����߂̕�����\��
	pUiTextManager.Add(
		DBG_NEW CUIText("���˂����J��o����\n�L�[�{�[�h�FJ\n�R���g���[���[�FA�{�^��", WSCREEN_WIDTH / 2, WSCREEN_HEIGHT / 2 + 250,
			50, 1, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));

	// ������UI�e�L�X�g�\��
	pUiTextManager.Add(
		DBG_NEW CUIText(
			Calculation::Convert::Utf8ToSjis(u8"�˂������񐔁F" + to_string(m_nSuccessNum) + "/" + to_string(s_nGoalSuccessNum)), 
			WSCREEN_WIDTH / 2 + 400, 150,
			50, 1, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));

	// �ڕW�����񐔂ɒB������J�E���g�_�E�����A�Q�[���V�[���ֈڍs
	if (m_nSuccessNum == s_nGoalSuccessNum)
	{
		m_nNextSceneCntDown--;

		// �˂��̃`���[�g���A����ԂɈڍs���܂�
		if (m_nNextSceneCntDown <= 0)
		{
			pTutorial->SetState(DBG_NEW CTutorialState_HandSwordPractice(pTutorial));
		}
	}
}

//============================================================================
// 
// private�����o
//
//============================================================================
