//============================================================================
// 
// �Q�[�� [Tutorial.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "tutorial.h"
#include "sound.h"

// �V�[���J�ڗp
#include "manager.h"
#include "object.h"
#include "game.h"

// �J�����擾�p
#include "camera_manager.h"

// �f�o�b�O�p
#include "renderer.h"

// �I�u�W�F�N�g�����p
#include "TutorialPlayer.h" // �`���[�g���A���v���C���[
#include "obstacle.h"     // ��Q��
#include "HoleObstacle.h" // ��������Q��
#include "hole.h"     // ��
#include "zone.h"
#include "number.h"   // ����
#include "block.h"    // �u���b�N
#include "target.h"   // �^�[�Q�b�g
#include "logger.h"   // ���O
#include "particle.h" // �p�[�e�B�N��
#include "bgmodel.h"  // �w�i���f��
#include "StencilMask.h" // �X�e���V���}�X�N
#include "UiText.h" // UI�e�L�X�g

#include "motion_set.h"		// ���[�V�����Z�b�g�p
#include "space.h"			// ��Ԑ��䏈���p
#include "swaying_player.h"	// �h��Đ������p
#include "calculation.h"

// ���̑��X�e�[�g
#include "summon_manager_state.h" // �������
#include "summon_manager_state_choose.h" // �����I�����
#include "summon_manager_state_start.h" // �����J�n���

// === �`���[�g���A����� ===
#include "TutorialState.h"
#include "TutorialState_GamePurpose.h" // �Q�[���ړI

// �J�������
#include "camera_state_targetzoom.h" // �J�����Y�[�����

// ���
#include "move_info.h" // �ړ����ɃA�N�Z�X���ē������~�߂���
#include "transparent_info.h"

// �}�l�[�W���[�֘A
#include "camera.h"  // �J����
#include "manager.h" // �}�l�[�W���[
#include "UiTextManager.h" // UI�e�L�X�g�}�l�[�W���[

// �X�|�i�[
#include "VisionBlockSpawner.h" // ����W�Q������������I�u�W�F�N�g

#include <fstream>
//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �ÓI�����o�ϐ�
//****************************************************

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CTutorial::Update()
{
	// ��Ԃ̍X�V
	if (m_pState)
	{
		m_pState->Update(this);
	}

	// �V�[���N���X�̍X�V����
	CScene::Update();

	/* �� */
	if (CManager::GetKeyboard()->GetTrigger(DIK_X))
	{
		// ���̃t���[���ł̋�ԕ`�������
		CRenderer::GetRenderer()->SetDrawSpace();
	}

	// �����I�ɃQ�[���V�[����
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) ||
		CManager::GetPad()->GetTrigger(CInputPad::JOYKEY::START))
	{
		To_Next();
	}

	m_nCntFrame++; // �t���[�������J�E���g����
}

//============================================================================
// �`�揈��
//============================================================================
void CTutorial::Draw()
{
	// ���N���X�̕`�揈��
	CScene::Draw();
}

//============================================================================
// ���̃V�[����
//============================================================================
void CTutorial::To_Next()
{
	// �S�I�u�W�F�N�g�̎��S�t���O�𔭓�����
	CObject::AllSetRelease();

	// ���U���g�V�[���֕ύX
	CScene_Manager::ChangeScene(CGame::Create());
}

//============================================================================
// ��Ԃ�ύX����
//============================================================================
void CTutorial::SetState(CTutorialState* pState)
{
	// ��Ԃւ̃|�C���^�����݂��Ă�����j�����ĐV������Ԃ�ݒ�
	if (m_pState)
	{
		m_pState->Uninit(this); // �I������
		delete m_pState;
		m_pState = pState;
	}

	// ��Ԃւ̃|�C���^�����݂��Ȃ��̂Őݒ肷��
	m_pState = pState;
	m_pState->Init(this); // ����������
}

//============================================================================
// �Q�[���𐶐�
//============================================================================
CTutorial* CTutorial::Create()
{
	// �Q�[���𐶐�
	CTutorial* pNew = DBG_NEW CTutorial();

	if (pNew == nullptr)
	{
		assert(false && "�Q�[���̐����Ɏ��s");
	}

	// �Q�[���̏����ݒ�
	if (FAILED(pNew->Init()))
	{
		assert(false && "�Q�[���̏����ݒ�Ɏ��s");
	}

	return pNew;
}

//============================================================================
// �I�u�W�F�N�g�̓������~�߂邩�ǂ����擾
//============================================================================
//const bool& CTutorial::GetStopObject()
//{
//	return s_bSTOPOBJECT;
//}
//
//============================================================================
// 
// protected�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CTutorial::CTutorial() :
	CScene{},
	m_bEnd{ false },
	m_nCntFrame{ 0 },
	m_bEditSummon{ true },
	m_pToneAdjust{ nullptr },
	m_pEffectAccumulation{ nullptr },
	m_pSmokeEffect{ nullptr },
	m_pState{nullptr}
{
	// �����Ȃ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTutorial::~CTutorial()
{
	// �����Ȃ�
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CTutorial::Init()
{
	CSummon_Manager* pSummonManager = CSummon_Manager::Create(); // �����}�l�[�W���[����
	// �e�X�g�z�u�����Ȃ��Ȃ�`���[�g���A����Ԃ𐶐�
	m_pState = DBG_NEW CTutorialState_GamePurpose(this);
	m_pState->Init(this); // ����������

	pSummonManager->SetChengeState(DBG_NEW CSummon_Manager_State_Choose(true)); // �����}�l�[�W���[�̃X�e�[�g���u�����I����ԁv�ɂ���

	// ����̃J������I���i�Q�[���V�[���ƂقƂ�Ǔ����Ȃ̂ŃQ�[���̃J�������g�p����j
	CCamera_Manager::GetInstance()->SetTransition("Game");

	// �Q�[���̃J�����ւ̃|�C���^���擾
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game");
	pCamera->SetUseDecayState(false);
	pCamera->OverwriteView(nullptr);
	pCamera->SetUpAdjust(0.0f); // ���Փx�������Ȃ���
	m_pToneAdjust = CToneAdjust::Create();
	m_pSmokeEffect = CSmokeEffect::Create();

	CTutorialPlayer::Create(D3DXVECTOR3(30.0f, -30.0f, -330.0f)); // �`���[�g���A���v���C���[����

	// �X�e���V���o�b�t�@�p�̃}�X�N�𐶐�
	CStencilMask::Create(D3DXVECTOR3(FSCREEN_WIDTH / 2, FSCREEN_HEIGHT / 2, 0.0f));

	// === �ϐ� ===

	m_pEffectAccumulation = CEffectAccumulation::Create();

	CStencilMask::SetUseStencil(false); // �X�e���V���o�b�t�@�͈�U�g�p���܂���

	// �V���𐶐�
	const JSON& ParamSkyDome = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Title_SkyDome.json");
	CObject_X::Create(ParamSkyDome);

	// �����_���[�̃|�C���^
	CRenderer* pRenderer = CRenderer::GetRenderer();

	// �t�H�O�̎n�_���Z�b�g
	pRenderer->SetFogStart(30000.0f);

	// �t�H�O�̏I�_���Z�b�g
	pRenderer->SetFogEnd(60000.0f);

	// �t�H�O�̃J���[���Z�b�g
	pRenderer->SetFogCol({ 0.0f, 0.0f, 0.0f ,0.0f });

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CTutorial::Uninit()
{	
	// ��Ԃ̏I��
	if (m_pState)
	{
		m_pState->Uninit(this); // �I������
		delete m_pState;
		m_pState = nullptr;
	}

	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game"); // �J�����ւ̃|�C���^
	pCamera->OverwriteView(nullptr); // �f�t�H���g�����ɖ߂�
}