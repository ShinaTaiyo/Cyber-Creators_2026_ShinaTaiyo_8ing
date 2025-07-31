//============================================================================
// 
// �Q�O�Q�T�N�V���Q�Q���F�`���[�g���A������������ [TutorialState_HandSwordDemo.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h
//****************************************************
#include "TutorialState_HandSwordDemo.h"
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
const string CTutorialState_HandSwordDemo::s_StrExplanation[static_cast<int>(CTutorialState_HandSwordDemo::EXPLANATION::MAX)] =
{
	"���̃Q�[���̖ړI��",
	"�������Ă����̂ňꏏ�Ɋ���Ă����܂��傤"
};

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CTutorialState_HandSwordDemo::CTutorialState_HandSwordDemo(CTutorial* pTutorial) :
	CTutorialState(pTutorial)
{
	// �����Ȃ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTutorialState_HandSwordDemo::~CTutorialState_HandSwordDemo()
{
	// �����Ȃ�
}

//============================================================================
// ����������
//============================================================================
HRESULT CTutorialState_HandSwordDemo::Init(CTutorial* pTutorial)
{
	// �����Ȃ�
	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CTutorialState_HandSwordDemo::Uninit(CTutorial* pTutorial)
{
	// �����Ȃ�
}

//============================================================================
// �X�V����
//============================================================================
void CTutorialState_HandSwordDemo::Update(CTutorial* pTutorial)
{
	if (CManager::GetKeyboard()->GetTrigger(DIK_M) ||
		CManager::GetPad()->GetTrigger(CInputPad::JOYKEY::A))
	{
		ChangeStrExplanation(pTutorial);
	}
	m_nCntFrame++; // �t���[�����J�E���g�A�b�v
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �����������ύX����
//============================================================================
void CTutorialState_HandSwordDemo::ChangeStrExplanation(CTutorial* pTutorial)
{
	m_nChooseExplanation++; // ����������ԍ����C���N�������g����

	//// �ő咴������ŏ��ɖ߂�
	//if (m_nChooseExplanation >= static_cast<int>(EXPLANATION::MAX))
	//{
	//	pTutorial->SetState(DBG_NEW CTutorialState_HandSwordDemo)
	//	m_nChooseExplanation = 0; // �����I��ԍ����O�ɖ߂�
	//}
}
