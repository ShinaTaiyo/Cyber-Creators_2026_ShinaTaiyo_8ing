//============================================================================
// 
// �Q�O�Q�T�N�V���Q�Q���F�`���[�g���A������������ [TutorialState_GamePurpose.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h
//****************************************************
#include "TutorialState_GamePurpose.h"
#include "TutorialState_ThrustGuide_Success.h"
#include "tutorial.h"
#include "Pch.h"
#include "calculation.h"
#include "UiText.h"
#include "manager.h"
#include "renderer.h"
#include "Button.h"
#include "input.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;
using namespace std;

//****************************************************
// �ÓI�����o�ϐ�
//****************************************************
const string CTutorialState_GamePurpose::s_StrExplanation[static_cast<int>(CTutorialState_GamePurpose::EXPLANATION::MAX)] =
{
	u8"���̃Q�[���̖ړI�́A����̍U�������؂邱�Ƃł�",
	u8"�������Ă����̂ňꏏ�Ɋ���Ă����܂��傤"
};

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CTutorialState_GamePurpose::CTutorialState_GamePurpose(CTutorial* pTutorial) : 
	m_nChooseExplanation{0},
	m_pButton{nullptr},
	CTutorialState(pTutorial)
{
	m_pButton = CButton::Create(D3DXVECTOR3(FSCREEN_WIDTH - 100.0f,FSCREEN_HEIGHT / 2,0.0f));
	m_pButton->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTutorialState_GamePurpose::~CTutorialState_GamePurpose()
{
	// �{�^���̎��S�t���O��ݒ�
	if (m_pButton)
	{
		m_pButton->SetRelease();
		m_pButton = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CTutorialState_GamePurpose::Update(CTutorial* pTutorial)
{
	CUIText* pUiText =
		DBG_NEW CUIText(Calculation::Convert::Utf8ToSjis(s_StrExplanation[m_nChooseExplanation]), 640, 600, 40,1, 
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
	
	CRenderer::GetRenderer()->GetUiTextManager().Add(pUiText);

	CRenderer::GetRenderer()->GetUiTextManager().Add(
		DBG_NEW CUIText("�ǂݐi�߂�\n�L�[�{�[�h�F[M]\n�R���g���[���[�FA�{�^��", 640, 490, 40,1, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f))
	);

	CRenderer::GetRenderer()->GetUiTextManager().Add(
		DBG_NEW CUIText("�`���[�g���A�����΂�\n�L�[�{�[�h�F[ENTER]\n�R���g���[���[�FSTART�{�^��", 640, 100, 70,1, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f))
	);

	if (m_pButton)
	{
		// �{�^��UI�\���ʒu������e�L�X�g�̉��ɐݒ�
		m_pButton->SetPos(D3DXVECTOR3(
			static_cast<float>(pUiText->GetPosX()) + static_cast<float>(pUiText->GetTextWidth() / 2) + m_pButton->GetSize().x,
			static_cast<float>(pUiText->GetPosY()),
			0.0f));
	}

	// ������i�߂�
	if (CManager::GetKeyboard()->GetTrigger(DIK_M) ||
		CManager::GetPad()->GetTrigger(CInputPad::JOYKEY::A))
	{
		ChangeStrExplanation(pTutorial);
	}
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ����������
//============================================================================
HRESULT CTutorialState_GamePurpose::Init(CTutorial* pTutorial)
{
	// �����Ȃ�
	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CTutorialState_GamePurpose::Uninit(CTutorial* pTutorial)
{
	// �����Ȃ�
}

//============================================================================
// �����������ύX����
//============================================================================
void CTutorialState_GamePurpose::ChangeStrExplanation(CTutorial* pTutorial)
{
	m_nChooseExplanation++; // ����������ԍ����C���N�������g����

	// �ő咴������S�Ă̐��������I������̂ŁA�˂��K�C�h������Ԃɂ���
	if (m_nChooseExplanation >= static_cast<int>(EXPLANATION::MAX))
	{
		pTutorial->SetState(DBG_NEW CTutorialState_ThrustGuide_Success(pTutorial));
		m_nChooseExplanation = 0; // �����I��ԍ����O�ɖ߂�
	}
}
