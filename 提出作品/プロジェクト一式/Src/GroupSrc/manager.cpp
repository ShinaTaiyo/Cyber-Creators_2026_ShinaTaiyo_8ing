//============================================================================
// 
// �}�l�[�W���[ [manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "manager.h"

// �e��V���O���g���N���X�p
#include "camera_manager.h"
//#include "font_manager.h"
#include "texture_manager.h"
#include "logger.h"

// === �V�F�[�_�[ ===
#include "EffectAccumulation.h"
#include "ToneAdjust.h"
#include "SmokeEffect.h"

/* ���V���O���g�� */
#include "renderer.h"
#include "scene.h"
#include "sound.h"
#include "X_manager.h"

//****************************************************
// �v���v���Z�b�T�f�B���N�e�B�u
//****************************************************
#define ENABLE_STENCIL_BUFFER 1	// �X�e���V���o�b�t�@�̗L����
#define IMGUI_USE_JAPANESE 1	// ImGui�̓��{��L����

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
CManager* CManager::m_pManager = nullptr;	// �}�l�[�W���[�̖{��

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CManager::Update()
{
	// Gui�̍X�V���J�n
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// �����_���[�̍X�V
	CRenderer::GetRenderer()->Update();

	// ���C�g�̍X�V
	m_pLight->Update();

	// �J�����̍X�V
	CCamera_Manager::GetInstance()->Update();

#ifdef _DEBUG
	// �t���[�J�����̍X�V
	CCamera::UpdateFreeCam();
#endif // _DEBUG

	// �V�[���̍X�V
	CScene_Manager::GetInstance()->GetScene()->Update();

	// �L�[�{�[�h�̍X�V
	m_pKeyboard->Update();

	// �}�E�X�̍X�V
	m_pMouse->Update();

	// �p�b�h�̍X�V
	m_pPad->Update();

	// ���O�o��
	CLogger::GetInstance()->DisplayLog();

	// Gui�̍X�V���I��
	ImGui::EndFrame();

	// �S�I�u�W�F�N�g��X�V����
	CObject::LateUpdateAll();
}

//============================================================================
// �`�揈��
//============================================================================
void CManager::Draw()
{
	// �����_���[�̕`��
	CRenderer::GetRenderer()->Draw();
}

//============================================================================
// ���C�g���擾
//============================================================================
CLight* CManager::GetLight() const
{
	return m_pLight;
}

//============================================================================
// ����
//============================================================================
HRESULT CManager::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pManager != nullptr)
	{
		assert(false && "�}�l�[�W���[�͊��ɍ쐬����Ă��邩�A�_���O�����O���Ă��܂�");
	}

	// �}�l�[�W���[�𐶐�
	m_pManager = DBG_NEW CManager();

	if (m_pManager == nullptr)
	{
		assert(false && "�}�l�[�W���[�̐����Ɏ��s");
	}

	// �}�l�[�W���[�̏����ݒ�
	if (FAILED(m_pManager->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// ���
//============================================================================
void CManager::Release()
{
	// �}�l�[�W���[�̔j��
	if (m_pManager != nullptr)
	{
		// �}�l�[�W���[�̏I������
		m_pManager->Uninit();

		// �����������
		delete m_pManager;

		// �|�C���^��������
		m_pManager = nullptr;
	}
}

//============================================================================
// �}�l�[�W���[���擾
//============================================================================
CManager* CManager::GetManager()
{
	return m_pManager;
}

//============================================================================
// �L�[�{�[�h���擾
//============================================================================
CInputKeyboard* CManager::GetKeyboard()
{
	return m_pManager->m_pKeyboard;
}

//============================================================================
// �}�E�X���擾
//============================================================================
CInputMouse* CManager::GetMouse()
{
	return m_pManager->m_pMouse;
}

//============================================================================
// �p�b�h���擾
//============================================================================
CInputPad* CManager::GetPad()
{
	return m_pManager->m_pPad;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CManager::CManager() :
	m_pKeyboard{ nullptr },
	m_pMouse{ nullptr },
	m_pPad{ nullptr },
	m_pLight{ nullptr }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CManager::~CManager()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �����_���[�̐���
	if (FAILED(CRenderer::Create(hWnd, TRUE)))
	{
		return E_FAIL;
	}

	// �T�E���h�����ݒ�
	if (FAILED(CSound::GetInstance()->Init(hWnd)))
	{
		return E_FAIL;
	}

	// �J�����}�l�[�W���[�̖����I�Ȑ����ƁA�����J�����̑I��
	CCamera_Manager::GetInstance()->SetSelectKey("Title");

	// �V�[���}�l�[�W���[�̐��� (�J�����E�T�E���h������)
	if (FAILED(CScene_Manager::Create()))
	{
		return E_FAIL;
	}

	// �t�H���g�}�l�[�W���[�̖����I�Ȑ���
	//CFont_Manager::GetInstance();

	// �e�N�X�`���}�l�[�W���[�̖����I�Ȑ���
	CTexture_Manager::GetInstance();

	// X���f���}�l�[�W���[�̖����I�Ȑ���
	CX_Manager::GetInstance();

	// ���K�[�̖����I�Ȑ���
	CLogger::GetInstance();

	// ���C�g�̐���
	m_pLight = DBG_NEW CLight();

	if (m_pLight == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// ���C�g�̏�����
	m_pLight->Init();

	// �L�[�{�[�h�̐���
	m_pKeyboard = DBG_NEW CInputKeyboard();

	if (m_pKeyboard == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// �L�[�{�[�h�̏�����
	m_pKeyboard->Init(hInstance, hWnd);

	// �}�E�X�̐���
	m_pMouse = DBG_NEW CInputMouse();

	if (m_pMouse == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// �}�E�X�̏�����
	m_pMouse->Init(hInstance, hWnd);

	// �p�b�h�̐���
	m_pPad = DBG_NEW CInputPad();

	if (m_pPad == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// �p�b�h�̏�����
	m_pPad->Init(hInstance, hWnd);

	// ImGui�̏����ݒ�
	InitImGui(hWnd);

	return S_OK;
}

//============================================================================
// ImGui�̏����ݒ�
//============================================================================
void CManager::InitImGui(HWND hWnd)
{
	// ImGui�̃R���e�L�X�g���쐬
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// ImGui�̓��o�͐ݒ�
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// ImGui�̕\���X�^�C����ݒ�
	ImGui::StyleColorsDark();

	// �o�b�N�G���h�̏����ݒ�
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(CRenderer::GetDeviece());

	// ImGui�̃t�H���g�ݒ�
#if IMGUI_USE_JAPANESE	// ImGui�̓��{��L����
	io.Fonts->AddFontFromFileTTF("Data\\FONT\\meiryo.ttc", 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//ImGui_ImplDX9_InvalidateDeviceObjects();
	//ImGui_ImplDX9_CreateDeviceObjects();
#else
	//io.Fonts->AddFontFromFileTTF("Data\\FONT\\meiryo.ttc", 20.0f, nullptr);
#endif // IMGUI_USE_JAPANESE
}

//============================================================================
// �I������
//============================================================================
void CManager::Uninit()
{
	// ImGui�̏I������
	UninitImGui();

	// �p�b�h�̔j��
	if (m_pPad != nullptr)
	{
		m_pPad->Uninit();	// �I������
		delete m_pPad;		// �����������
		m_pPad = nullptr;	// �|�C���^��������
	}

	// �}�E�X�̔j��
	if (m_pMouse != nullptr)
	{
		m_pMouse->Uninit();	// �I������
		delete m_pMouse;	// �����������
		m_pMouse = nullptr;	// �|�C���^��������
	}

	// �L�[�{�[�h�̔j��
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();	// �I������
		delete m_pKeyboard;		// �����������
		m_pKeyboard = nullptr;	// �|�C���^��������
	}

	// ���C�g�̔j��
	if (m_pLight != nullptr)
	{
		delete m_pLight;		// �����������
		m_pLight = nullptr;		// �|�C���^��������
	}

	// �V�[���}�l�[�W���[�̔j��
	CScene_Manager::Release();

	// �T�E���h�̔j��
	CSound::GetInstance()->Release();

	// �����_���[�̔j��
	CRenderer::Release();
}

//============================================================================
// ImGui�̏I������
//============================================================================
void CManager::UninitImGui()
{
	// ImGUi�̏I������
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}