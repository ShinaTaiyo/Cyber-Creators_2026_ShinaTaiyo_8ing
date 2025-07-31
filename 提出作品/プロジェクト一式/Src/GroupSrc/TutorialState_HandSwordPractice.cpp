//============================================================================
// 
// �Q�O�Q�T�N�V���Q�Q���F�`���[�g���A������������ [TutorialState_HandSwordPractice.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h
//****************************************************
#include "TutorialState_HandSwordPractice.h"
#include "summon_manager_state_start.h"
#include "PlayerState_Returning.h"
#include "summon_manager_state_choose.h"
#include "PlayerState_HandSwordPractice.h"
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
CTutorialState_HandSwordPractice::CTutorialState_HandSwordPractice(CTutorial* pTutorial) :
	m_nCommentNum{0},
	m_nCommentDispFrame{0},
	m_bExecuteHandSword{false},
	CTutorialState{ pTutorial }
{
	// �����Ȃ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTutorialState_HandSwordPractice::~CTutorialState_HandSwordPractice()
{
	// �����Ȃ�
}

//============================================================================
// ����������
//============================================================================
HRESULT CTutorialState_HandSwordPractice::Init(CTutorial* pTutorial)
{
	std::fstream ReadingFile; // �t�@�C���ǂݍ��ݗp

	// === �˂����K�W�F�C�\���t�@�C���ǂݍ��� ===

	ReadingFile.open("Data\\JSON\\Tutorial\\HandSwordPractice.Json", std::ios::in);

	// �t�@�C��������ɊJ���Ă����珢���p�^�[�����X�g���i�[
	if (ReadingFile.good())
	{
		ReadingFile >> m_Json; // �W�F�C�\���i�[

		// �z�񂶂�Ȃ������珈�����I��
		if (!m_Json.is_array())
		{
			assert(false && "�˂����K��Ԃ̃W�F�C�\�����I�u�W�F�N�g�^���Ⴀ��܂���");
		}
	}

	ReadingFile.close(); // �t�@�C�������

	// �����}�l�[�W���[�ւ̃|�C���^���擾��,�I��������Ԃɕς���
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));
	if (pSummonManager)
	{
		pSummonManager->SetChengeState(DBG_NEW CSummon_Manager_State_Choose(true));
	}

	// �v���C���[�ւ̃|�C���^�����݂���Ȃ�v���C���[��߂��Ԃɂ���
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER));
	if (pPlayer)
	{
		pPlayer->SetState(DBG_NEW CPlayerState_Returning(pPlayer));
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CTutorialState_HandSwordPractice::Uninit(CTutorial* pTutorial)
{
	// �����Ȃ�
}

//============================================================================
// �X�V����
//============================================================================
void CTutorialState_HandSwordPractice::Update(CTutorial* pTutorial)
{
	// �e�L�X�g���Ȃ��Ȃ珈�����I��
	if (!m_Json[m_nCommentNum].contains("Text"))
	{
		return;
	}

	// ����������\��
	string StrComment = m_Json[m_nCommentNum]["Text"];
	CRenderer::GetRenderer()->GetUiTextManager().Add(
		DBG_NEW CUIText(Calculation::Convert::Utf8ToSjis(StrComment), WSCREEN_WIDTH / 2, 100, 50,1,
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));

	// ������ς���
	ChangeStrExplanation(pTutorial);
}

//============================================================================
// ������ς���
//============================================================================
void CTutorialState_HandSwordPractice::ChangeStrExplanation(CTutorial* pTutorial)
{
	if (!m_Json[m_nCommentNum].contains("DispFrame") ||
		!m_Json[m_nCommentNum].contains("Stop"))
	{
		return;
	}

	int nDispFrame = m_Json[m_nCommentNum]["DispFrame"]; // �\���t���[����
	m_bExecuteHandSword = m_Json[m_nCommentNum]["Stop"]; // �~�߂邩�ǂ���

	// �v���C���[�ւ̃|�C���^�����݂���Ȃ�v���C���[��߂��Ԃɂ���
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER));

	CRenderer* pRenderer = CRenderer::GetRenderer(); // �`����ւ̃|�C���^���擾
	CUITextManager& pUiTextManager = pRenderer->GetUiTextManager(); // UI�e�L�X�g�}�l�[�W���[���擾

	// �蓁�𔭓����Ȃ��Ȃ�
	if (!m_bExecuteHandSword)
	{
		// �\���t���[�����𒴂����玟�̃R�����g�ɐi��
		if (m_nCommentDispFrame >= nDispFrame)
		{
			m_nCommentNum++; // ����������ԍ����C���N�������g����

		    // �����}�l�[�W���[�ւ̃|�C���^���擾��,�I��������Ԃɕς���
			CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));
			if (!pSummonManager)
			{
				assert(false && "�蓁���K��Ԃŏ����}�l�[�W���[�ւ̃|�C���^������܂���");
				return;
			}

			CSummon_Manager_State_Choose* pChoose =
				dynamic_cast<CSummon_Manager_State_Choose*>(pSummonManager->GetState());

			switch (m_nCommentNum)
			{
				// ���̃R�����g�ɂȂ����u�Ԃɏ����O���[�v���Ă�
			case static_cast<int>(COMMENT::KONOMAMAZYA):
				pChoose->SetSummonGroup(static_cast<int>(CSummon_Manager::SUMMONGROUP::PATTERN_2));
				// �v���C���[�̏�Ԃ��u�蓁���K�v�ɂ���
				if (pPlayer)
				{
					pPlayer->SetState(DBG_NEW CPlayerState_HandSwordPractice(pPlayer, 150));
				}
				break;
				case static_cast<int>(COMMENT::EXECUTE):

					// �������ꂽ�I�u�W�F�N�g���|�[�Y��Ԃɂ���
					CSummon_Manager::StopSummonObj();
				break;
			default:
				break;
			}
			m_nCommentDispFrame = 0; // �R�����g�t���[������������
		}
		// �R�����g�\���t���[�������J�E���g�A�b�v����
		m_nCommentDispFrame++;
	}
	// �蓁�𔭓�����Ȃ�
	else
	{
		// �˂����J��o�����߂̕�����\��
		pUiTextManager.Add(
			DBG_NEW CUIText("���蓁���J��o����\n�L�[�{�[�h�FX\n�R���g���[���[�FX�{�^��", WSCREEN_WIDTH / 2, WSCREEN_HEIGHT / 2 + 250,
				50, 1, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));

		// �v���C���[�̎蓁���K���I������玟�̃R�����g��
		if (CPlayerState_HandSwordPractice::GetEndPractice())
		{
			m_nCommentNum++;
			if (m_nCommentNum >= static_cast<int>(COMMENT::MAX))
			{
				m_nCommentNum = 0; // �����I��ԍ����O�ɖ߂�
			}
		}
	}

	// �S�ẴR�����g�����ꂽ��V�[���J�ڂ���
	if (m_nCommentNum >= static_cast<int>(COMMENT::MAX))
	{
		m_nCommentNum = 0;
		pTutorial->To_Next();
		return; // �V�[�����ς��̂ŏ������I��
	}
}
