//============================================================================
// 
// �Q�[�� [TestScene.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "TestScene.h"
#include "sound.h"

// �V�[���J�ڗp
#include "manager.h"
#include "object.h"
#include "title.h"
#include "game.h"
#include "TestScene.h" // �`���[�g���A���v���C���[

// �J�����擾�p
#include "camera_manager.h"

// �f�o�b�O�p
#include "renderer.h"

// �I�u�W�F�N�g�����p

#include "player.h"
#include "motion_set.h"		// ���[�V�����Z�b�g�p
#include "space.h"			// ��Ԑ��䏈���p
#include "swaying_player.h"	// �h��Đ������p
#include "calculation.h"
#include "StencilMask.h"

// ���̑��X�e�[�g
#include "summon_manager_state.h" // �������
#include "summon_manager_state_choose.h" // �����I�����
#include "summon_manager_state_start.h" // �����J�n���

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
bool CTestScene::s_bTest = false; // �e�X�g�z�u�𗘗p���邩�ǂ���

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CTestScene::Update()
{
	// �V�[���N���X�̍X�V����
	CScene::Update();

	/* �� */
	if (CManager::GetKeyboard()->GetTrigger(DIK_X))
	{
		// ���̃t���[���ł̋�ԕ`�������
		CRenderer::GetRenderer()->SetDrawSpace();
	}

#ifdef _DEBUG	// �p�����[�^�m�F�p
	// �����o��
	if (CManager::GetKeyboard()->GetTrigger(DIK_SPACE))
	{
		CSound::GetInstance()->Play(CSound::LABEL::TEXT);
	}

#endif // _DEBUG
	// ���U���g��ʂ�
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
	{
		To_Next();
	}
}

//============================================================================
// �`�揈��
//============================================================================
void CTestScene::Draw()
{
	// ���N���X�̕`�揈��
	CScene::Draw();
}

//============================================================================
// ���̃V�[����
//============================================================================
void CTestScene::To_Next()
{
	// �S�I�u�W�F�N�g�̎��S�t���O�𔭓�����
	CObject::AllSetRelease();

	// �^�C�g���V�[���֕ύX
	CScene_Manager::ChangeScene(CTitle::Create());
}

//============================================================================
// �Q�[���𐶐�
//============================================================================
CTestScene* CTestScene::Create()
{
	// �Q�[���𐶐�
	CTestScene* pNew = DBG_NEW CTestScene();

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
// �e�X�g�z�u�����邩�ǂ���
//============================================================================
bool& CTestScene::GetTest()
{
	return s_bTest;
}

//============================================================================
// �I�u�W�F�N�g�̓������~�߂邩�ǂ����擾
//============================================================================
//const bool& CTestScene::GetStopObject()
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
CTestScene::CTestScene() :
	CScene{},
	m_bEnd{ false },
	m_pToneAdjust{ nullptr },
	m_pEffectAccumulation{ nullptr },
	m_pSmokeEffect{ nullptr }
{
	// �����Ȃ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTestScene::~CTestScene()
{
	// �����Ȃ�
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CTestScene::Init()
{
	CSummon_Manager* pSummonManager = CSummon_Manager::Create(); // �����}�l�[�W���[����
	pSummonManager->SetChengeState(DBG_NEW CSummon_Manager_State_Choose(false)); // �����}�l�[�W���[�̃X�e�[�g���u�����I����ԁv�ɂ���
	
	// ����̃J������I���i�Q�[���V�[���ƂقƂ�Ǔ����Ȃ̂ŃQ�[���̃J�������g�p����j
	CCamera_Manager::GetInstance()->SetTransition("Game");

	// �Q�[���̃J�����ւ̃|�C���^���擾
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game");
	pCamera->SetUseDecayState(false);
	pCamera->SetUpAdjust(0.0f); // ���Փx�������Ȃ���
	m_pToneAdjust = CToneAdjust::Create();
	m_pSmokeEffect = CSmokeEffect::Create();

	CPlayer::Create(D3DXVECTOR3(30.0f, -30.0f, -330.0f)); // �`���[�g���A���v���C���[����

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
void CTestScene::Uninit()
{
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game"); // �J�����ւ̃|�C���^
	pCamera->OverwriteView(nullptr); // �f�t�H���g�����ɖ߂�
}