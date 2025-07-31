//============================================================================
// 
// �^�C�g�� [title.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "title.h"
#include "sound.h"

// �J�ڐ�̃V�[�����
#include "prologue.h"
#include "TestScene.h"

// ���͎擾�p
#include "manager.h"

// �J�����擾�p
#include "camera_manager.h"

// �e�N�X�`���擾�p
#include "texture_manager.h"

// �I�u�W�F�N�g�Ǘ��p
#include "object_HUD.h"
#include "object_X.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �ÓI�����o�ϐ�
//****************************************************
bool CTitle::s_bNextSceneIsTest = false; // ���̃V�[�����e�X�g�V�[���ɂ��邩�ǂ���

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CTitle::Update()
{
	// ���N���X�̍X�V����
	CScene::Update();

	// �t���ϔ���
	GenerateLeaf();

#ifdef _DEBUG
	// ���̃E�C���h�E�̈ʒu���T�C�Y�����߂�
	ImGui::SetNextWindowSize(ImVec2(300, 150));
	ImGui::SetNextWindowPos(ImVec2(900, 150));

	// �K�C�h�Ȃ��̎��͏����O���[�v��I�ׂ�悤�ɂ���
	ImGui::Begin(u8"���̃V�[�����e�X�g�V�[���ɂ���H");

	// �`�F�b�N�{�b�N�X�Ńt���O�̃I���I�t
	ImGui::Checkbox(u8"���܂����H", &s_bNextSceneIsTest);

	ImGui::End();
#endif // _DEBUG

#ifdef _DEBUG
	// �퓬BGM�փg�����W�V����
	CSound::GetInstance()->Transition(CSound::LABEL::BGM_1, CSound::LABEL::BGM_0);
#endif

	// �v�����[�O�V�[����
	auto* pPad = CManager::GetPad();
	if (CManager::GetManager()->GetKeyboard()->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputPad::JOYKEY::A) ||
		pPad->GetTrigger(CInputPad::JOYKEY::START))
	{
		To_Next();
	}
}

//============================================================================
// �`�揈��
//============================================================================
void CTitle::Draw()
{
	// ���N���X�̕`�揈��
	CScene::Draw();
}

//============================================================================
// ���̃V�[����
//============================================================================
void CTitle::To_Next()
{
	// ���S�������Ȃ��ʒu��
	m_pLogo->SetPosTarget({ 640.0f, -300.0f, 0.0f });;

	// �B�ꑐ�������Ȃ��ʒu��
	for (unsigned char wCntLoop = 0; wCntLoop < 2; ++wCntLoop)
	{
		Vec3 tmpPosTarget = m_apHideGrass[wCntLoop]->RefVectorInterpolator()->GetPosTarget();
		tmpPosTarget.y = 30.0f;
		m_apHideGrass[wCntLoop]->RefVectorInterpolator()->SetPosTarget(tmpPosTarget);
	}

	// �S�Ă̗t���ς����R�ɏ���
	for (auto& it : m_vpLeaf)
	{
		it->RefVectorInterpolator()->SetScaleTarget(VEC3_INIT);
	}

	// �S�I�u�W�F�N�g�����
	//CObject::ReleaseAll();

	// �v�����[�O�V�[���֕ύX
	if (s_bNextSceneIsTest == false)
	{
		CScene_Manager::ChangeScene(CPrologue::Create());
	}
	// �e�X�g�V�[���֕ύX
	else
	{
		// �S�I�u�W�F�N�g�����
		CObject::ReleaseAll();

		CScene_Manager::ChangeScene(CTestScene::Create());
	}
}

//============================================================================
// �^�C�g���𐶐�
//============================================================================
CTitle* CTitle::Create()
{
	// �^�C�g���̃C���X�^���X�𐶐�
	CTitle* pScene = DBG_NEW CTitle();

	// �������s
	if (pScene == nullptr)
	{
		throw std::runtime_error("Failed to create title scene");
	}

	// ����������
	if (FAILED(pScene->Init()))
	{
		throw std::runtime_error("Failed to initialize title scene");
	}

	// �C���X�^���X�̃|�C���^��Ԃ�
	return pScene;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CTitle::CTitle() :
	CScene(),
	m_pLogo(nullptr),
	m_apHideGrass{ nullptr, nullptr },
	m_vpLeaf{},
	m_nFrameCounter(0)
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTitle::~CTitle()
{}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CTitle::Init()
{
	s_bNextSceneIsTest = false; // ���̃V�[�����e�X�g�V�[���ɂ��邩�ǂ�����������

	// ���SHUD�𐶐��A�e�N�X�`���̂��߂Ƀ|�C���^�R�s�[
	const JSON& ParamHUD = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\HUD\\Logo.json");
	m_pLogo = CObject_HUD::Create(ParamHUD);

	// ���S�Ƀe�N�X�`����ݒ�
	const std::string& Key = "Logo";
	m_pLogo->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey(Key));

	// ���i�̔z�u�Z�b�g��ǂݍ���
	const JSON& LayoutSetForTitle = hws::OpenJsonFile("Data\\JSON\\EXPORT\\Layout_Side.json");

	// �ǂݍ��߂��Ƃ�����
	if (LayoutSetForTitle != nullptr)
	{
		// �I�u�W�F�N�g�𐶐�
		for (unsigned short wCntLoop = 0; wCntLoop < static_cast<short int>(LayoutSetForTitle["NumElement"]); ++wCntLoop)
		{
			CObject_X::Create(LayoutSetForTitle["Set"][wCntLoop]);
		}
	}
	else
	{
		throw std::runtime_error("Failed to Import 'Layout_Side.json'");
	}

	// �B�ꑐ�𐶐�
	m_apHideGrass[0] = CObject_X::Create(hws::OpenJsonFile("Data\\JSON\\ObJECT\\X\\Export_HideGrass_0.json"));
	m_apHideGrass[1] = CObject_X::Create(hws::OpenJsonFile("Data\\JSON\\ObJECT\\X\\Export_HideGrass_1.json"));

	// ����̃J������ݒ�
	CCamera_Manager::GetInstance()->SetSelectKey("Title");

#ifndef _DEBUG
	// ��������BGM��������
	CSound::GetInstance()->Stop(CSound::LABEL::BGM_1);
	CSound::GetInstance()->Play(CSound::LABEL::BGM_0);
#endif

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CTitle::Uninit()
{}

//============================================================================
// �t���ϔ���
//============================================================================
void CTitle::GenerateLeaf()
{
	++m_nFrameCounter;

	if (m_nFrameCounter >= 20)
	{
		m_nFrameCounter = 0;

		// �I�u�W�F�N�g�𐶐���
		CObject_X* tmpPtr = CObject_X::Create(hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Generate_Leaf.json"));
		
		// ������K���Ɍ���
		Vec3 tmpRot = VEC3_INIT;
		float fRand = hws::GetRandomValue<float>() * 0.1f;
		tmpRot += { fRand, fRand, fRand };
		tmpPtr->SetRot(tmpRot);

		// �ʒu��K���Ɍ���
		Vec3 tmpPos = 
		{
			hws::GetRandomValue<float>(),
			100.0f + hws::GetRandomValue<float>() * 0.1f,
			50.0f + hws::GetRandomValue<float>() * 0.25f
		};
		tmpPtr->SetPos(tmpPos);
		tmpPtr->RefVectorInterpolator()->SetPosTarget(tmpPos);

		// �`��O�ɍX�V�K��
		tmpPtr->Update();

		// �R���e�i�ɒǉ�
		m_vpLeaf.push_back(tmpPtr);
	}

	// �S�Ă̈ʒu�𓮂���
	for (auto& it : m_vpLeaf)
	{
		// �N���N��
		Vec3 tmpRot = it->GetRot();
		float fAdderRot = 0.05f;
		tmpRot.x += fAdderRot;
		tmpRot.y += fAdderRot;
		tmpRot.z += fAdderRot;
		it->SetRot(tmpRot);

		// �����
		Vec3 tmpPos = it->GetPos();
		tmpPos += 
		{
			-3.0f + (3.0f * cosf(tmpRot.y)),
			-3.0f + (3.0f * cosf(tmpPos.x * 0.1f)),
			0.0f
		};
		it->RefVectorInterpolator()->SetPosTarget(tmpPos);
	}

	// ����
	for (auto it = m_vpLeaf.begin(); it != m_vpLeaf.end(); )
	{
		if ((*it)->GetPos().y < -10.0f)
		{
			(*it)->SetRelease();
			it = m_vpLeaf.erase(it);
			continue;
		}

		++it;
	}
}