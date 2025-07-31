//============================================================================
// 
// �v�����[�O [prologue.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "prologue.h"
#include "sound.h"

// �J�����}�l�[�W���[�擾�p
#include "camera_manager.h"
#include "bezier_manager.h"

// �V�[���J�ڗp
#include "manager.h"
#include "object.h"
#include "tutorial.h"

// �I�u�W�F�N�g�����p
#include "bezier_manager.h"
#include "texture_manager.h"
#include "object_HUD.h"
#include "object_X.h"
#include "object_parts.h"
#include "motion_set.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �������O��Ԃ��`
//****************************************************
namespace
{
	// �ėp�ʒu
	float g_fFloatTest = 0.0f;
	Vec3 g_PosTest = VEC3_INIT;

	// �f�o�b�O����p
	void PrologueTest()
	{
		//MIS::MyImGuiShortcut_BeginWindow(u8"�v�����[�O�f�o�b�O", { -1.0f, -1.0f });
		//ImGui::Text(u8"����������������������������������������������������������������������������������");
		//ImGui::DragFloat(u8"�Ȃ񂩕�������", &g_fFloatTest, 0.1f);
		//MIS::MyImGuiShortcut_DragVector(u8"�ʒu", g_PosTest, 0.1f);
		//MIS::MyImGuiShortcut_ResetVector(u8"�ʒu", g_PosTest);
		//ImGui::End();
	}
}

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CPrologue::Update()
{
	// �V�[���N���X�̍X�V����
	CScene::Update();

	// �e�L�X�g�̍X�V����
	UpdateText();

#if 1
	// �t�F�[�Y�̍X�V����
	UpdatePhase();
#endif

	// �`���[�g���A����ʂ�
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
void CPrologue::Draw()
{
	// ���N���X�̕`�揈��
	CScene::Draw();
}

//============================================================================
// ���̃V�[����
//============================================================================
void CPrologue::To_Next()
{
	// �S�I�u�W�F�N�g�����
	CObject::ReleaseAll();

	// �`���[�g���A���V�[���֕ύX
	CScene_Manager::ChangeScene(CTutorial::Create());

	// �x�W�F�Ȑ��}�l�[�W���[�̖����I�Ȕj��
	CBezier_Manager::GetInstance()->ExplicitRelease();
}

//============================================================================
// �v�����[�O�𐶐�
//============================================================================
CPrologue* CPrologue::Create()
{
	// �v�����[�O�𐶐�
	CPrologue* pNew = DBG_NEW CPrologue();

	if (pNew == nullptr)
	{
		assert(false && "�v�����[�O�̐����Ɏ��s");
	}

	// �v�����[�O�̏����ݒ�
	if (FAILED(pNew->Init()))
	{
		assert(false && "�v�����[�O�̏����ݒ�Ɏ��s");
	}

	return pNew;
}

//============================================================================
// 
// protected�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CPrologue::CPrologue() :
	CScene(),
	m_nPhase(0),
	m_nFrameCounter(0),
	m_pText(nullptr),
	m_pEnemy(nullptr),
	m_pHuman(nullptr),
	m_pGold(nullptr),
	m_vpLeaves{},
	m_fAlphaRate(0.025f)
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPrologue::~CPrologue()
{
	// �O�̂��߃N���[���A�b�v
	Uninit();
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CPrologue::Init()
{
	// �x�W�F�Ȑ��}�l�[�W���[�̖����I�Ȑ���
	CBezier_Manager::GetInstance();

	// �����E�F�C�𐶐�
	CObject_X* tmpRunway = CObject_X::Create(hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Export_Runway.json"));
	tmpRunway->Update();

	// �ł������𐶐�
	CObject_X* tmpBigTree = CObject_X::Create(hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\BigTree.json"));
	tmpBigTree->Update();

	// ���SHUD�𐶐�
	if (!m_pText)
	{
		const JSON& ParamHUD = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\HUD\\Skip.json");
		m_pText = CObject_HUD::Create(ParamHUD);

		// ���S�Ƀe�N�X�`����ݒ�
		const std::string& Key = "Skip";
		m_pText->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey(Key));
	}

	// ��l�����[�V�����Z�b�g�𐶐�
	if (!m_pHuman)
	{
		m_pHuman = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\MOTIONSET\\Karate_Human.json"));
		m_pHuman->SetNowMotion(0);
		m_pHuman->SetPos({ 0.0f, -20.0f, 0.0f });
		m_pHuman->SetRot({ 0.0f, 0.0f, 0.0f });
	}

	// ���C���S�b�g�𐶐�
	if (!m_pGold)
	{
		const JSON& GoldParam = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Gold.json");
		m_pGold = CObject_X::Create(GoldParam);

		// ��ʒ������ɔz�u
		m_pGold->SetPos({ 0.0f, 150.0f, 30.0f });

		// �`��O�ɍX�V
		m_pGold->Update();

		// �h�J��
		CSound::GetInstance()->Play(CSound::LABEL::DOKA);
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CPrologue::Uninit()
{}

//============================================================================
// �e�L�X�g�X�V
//============================================================================
void CPrologue::UpdateText()
{
	float fAlpha = m_pText->GetAlpha();

	fAlpha += m_fAlphaRate;

	if (fabsf(fAlpha) > 1.0f)
	{
		m_fAlphaRate *= -1.0f;
	}

	m_pText->SetAlpha(fAlpha);
}

//============================================================================
// �t�F�[�Y�X�V
//============================================================================
void CPrologue::UpdatePhase()
{
	if (m_nPhase == 0)
	{
		Vec3 GoldPos = m_pGold->GetPos();

		// �S�[���h�������Ă���
		if (GoldPos.y > 0.0f)
		{
			GoldPos.y += -5.0f;
			m_pGold->SetPos(GoldPos);
		}
		else
		{
			// ���~����
			GoldPos.y += 3.0f;
			m_pGold->SetPos(GoldPos);

			// �������������玟�̃t�F�[�Y��
			++m_nPhase;
		}
	}
	else if (m_nPhase == 1)
	{
		// �J�E���g���g�p
		++m_nFrameCounter;
	
		// 0.5�b�o�߂܂ŃJ�E���g
		if (m_nFrameCounter <= 30)
		{
			// ���̊ԁA�U��
			CCamera_Manager::GetInstance()->RefSelectCamera().SetVibration(0.005f);
		}
		else
		{
			// ���Z�b�g�����̃t�F�[�Y��
			m_nFrameCounter = 0;
			++m_nPhase;

			// �G���[�V�����Z�b�g�𐶐�
			if (!m_pEnemy)
			{
				m_pEnemy = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\MOTIONSET\\Karate_Enemy.json"));
				m_pEnemy->SetNowMotion(1);
				m_pEnemy->SetPos({ -150.0f, 13.0f, 0.0f });
				m_pEnemy->SetRot({ 0.0f, D3DX_PI * -0.5f, 0.0f });
			}
		}
	}
	else if (m_nPhase == 2)
	{
		Vec3 EnemyPos = m_pEnemy->GetPos();

		if (EnemyPos.x < 0.0f)
		{
			// �G�𒆉��܂ňړ�������
			EnemyPos.x += 1.5f;
			m_pEnemy->SetPos(EnemyPos);
		}
		else
		{
			// ����̕���������
			m_pEnemy->SetRot({0.0f, D3DX_PI, 0.0f});

			// �͂ݏo���Ȃ��悤���W�Œ�
			EnemyPos.x += 0.0f;
			m_pEnemy->SetPos(EnemyPos);

			// �����ɓ��B�Ń��[�V�����ύX
			m_pEnemy->SetNowMotion(4);

			// ���̃t�F�[�Y��
			++m_nPhase;
		}
	}
	else if (m_nPhase == 3)
	{
		// ���[�V��������~������
		if (m_pEnemy->GetStopState())
		{
			// ���̃t�F�[�Y��
			++m_nPhase;

			// ������i�s�����ɖ߂�
			m_pEnemy->SetRot({0.0f, D3DX_PI * -0.5f, 0.0f});

			// �G�̃��[�V������D�_�_�b�V���ɕύX
			m_pEnemy->SetNowMotion(5);

			// ��������ł�����
			m_pGold->SetScale(VEC3_INIT);
		}
	}
	else if (m_nPhase == 4)
	{
		Vec3 EnemyPos = m_pEnemy->GetPos();

		if (EnemyPos.x < 150.0f)
		{
			// �G���E�[�܂ňړ�������
			EnemyPos.x += 1.5f;
			m_pEnemy->SetPos(EnemyPos);
		}
		else
		{
			// �E�֑��蔲������t�F�[�Y�i�s
			++m_nPhase;

			// �G�������E�F�C��Ɉړ�
			m_pEnemy->SetPos({ -250.0f, 13.0f, -500.0f });

			// �g�p����J������ύX
			CCamera_Manager::GetInstance()->SetSelectKey("Runway");
		}
	}
	else if (m_nPhase == 5)
	{
		// �J�E���g���g�p
		++m_nFrameCounter;

		// 2�b�o�߂܂ŃJ�E���g
		if (m_nFrameCounter <= 120)
		{
			Vec3 EnemyPos = m_pEnemy->GetPos();

			if (EnemyPos.x < 250.0f)
			{
				// �����E�F�C��œG���ړ�
				EnemyPos.x += 1.5f;
				m_pEnemy->SetPos(EnemyPos);
			}

			// �J������G�̈ʒu�֓���
			CCamera_Manager::GetInstance()->RefSelectCamera().SetPos(EnemyPos);
			CCamera_Manager::GetInstance()->RefSelectCamera().SetPosTarget(EnemyPos);
		}
		else
		{
			// ���Z�b�g�����̃t�F�[�Y��
			m_nFrameCounter = 0;
			++m_nPhase;

			// �g�p����J������ύX
			CCamera_Manager::GetInstance()->SetSelectKey("HumanAppeal");

			// �x�W�F�}�l�[�W���[���Đ���
			CBezier_Manager::ExplicitRelease();
			CBezier_Manager::SetReloadPath("Data\\JSON\\INITIALIZE\\Bezier_HumanAppeal.json");
			CBezier_Manager::GetInstance();
		}
	}
	else if (m_nPhase == 6)
	{
		// �x�W�F�}�l�[�W���[�̍X�V�󋵂ɉ�����
		if (!CBezier_Manager::GetInstance()->Update())
		{
			/* �X�V�� */

			// �O�Ղ̍��W���擾
			const Vec3& TrajectoryPos = CBezier_Manager::GetInstance()->GetPosTracePointOnCurrentBeizier();

			/* HumanAppeal�J�����e�X�g */
			CCamera_Manager::GetInstance()->RefSelectCamera().SetPosTarget(TrajectoryPos);
		}
		else
		{
			/* �X�V���������� */

			// ��l���̈ʒu���ł������̓����ɕύX
			m_pHuman->SetPos({ 0.0f, 420.0f, 0.0f });

			// �t�F�[�Y��i�s
			++m_nPhase;
		}
	}
	else if (m_nPhase == 7)
	{
		Vec3 HumanPos = m_pHuman->GetPos();

		// �ł��������甇���o�Ă���
		if (HumanPos.y < 432.5f)
		{
			HumanPos.y += 0.5f;
			m_pHuman->SetPos(HumanPos);
		}
		else
		{
			// ���\�L���o��
			const JSON& TmpParam = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\HUD\\Tmp.json");
			CObject_HUD* pTmp = CObject_HUD::Create(TmpParam);
			pTmp->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("Tmp"));

			// �㏸����
			HumanPos.y = 432.5f;
			m_pHuman->SetPos(HumanPos);

			// ��l���ɃW�����v���[�V����
			m_pHuman->SetNowMotion(6);

			// ��ʂɗt�𐶐�
			SetLeaves();

			// �t�F�[�Y��i�s
			++m_nPhase;
		}
	}
	else if (m_nPhase == 8)
	{
		// ��ʂ̗t���ύX�V
		UpdateLeaves();
	}
	else if (m_nPhase == -1)
	{
		// �\������
		float fAlpha = m_pText->GetAlpha();
		fAlpha += -0.05f;
		m_pText->SetAlpha(fAlpha);

		// �`���[�g���A���V�[����
		if (fAlpha < 0.0f)
		{
			To_Next();
		}
	}
}

//============================================================================
// �t���ς̃Z�b�g
//============================================================================
void CPrologue::SetLeaves()
{
	for (unsigned char ucCnt = 0; ucCnt < 20; ++ucCnt)
	{
		// �I�u�W�F�N�g�𐶐���
		CObject_X* tmpPtr = CObject_X::Create(hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Generate_Leaf.json"));

		// �k�ڂ�ݒ�
		tmpPtr->SetScale(VEC3_INIT);
		tmpPtr->RefVectorInterpolator()->SetScaleTarget({ 0.25f, 0.25f, 0.25f });

		// ������K���Ɍ���
		Vec3 tmpRot = VEC3_INIT;
		float fRand = hws::GetRandomValue<float>() * 0.1f;
		tmpRot += { fRand, fRand, fRand };
		tmpPtr->SetRot(tmpRot);
		
		// �`��O�ɍX�V�K��
		tmpPtr->Update();

		// �R���e�i�ɒǉ�
		m_vpLeaves.push_back(tmpPtr);
	}
}

//============================================================================
// �t���ς̍X�V����
//============================================================================
void CPrologue::UpdateLeaves()
{
	for (auto it : m_vpLeaves)
	{
		// �k�ڂ��擾
		Vec3 tmpScale = it->GetScale();

		// �N���N��
		Vec3 tmpRot = it->GetRot();
		float fAdderRot = 0.05f;
		tmpRot.x += fAdderRot;
		tmpRot.y += fAdderRot;
		tmpRot.z += fAdderRot;
		it->SetRot(tmpRot);

		// �ʒu�ύX
		Vec3 tmpPos = it->GetPos();
		if (tmpScale.x < 0.24f)
		{
			// �����
			tmpPos +=
			{
				-3.0f + (3.0f * cosf(tmpRot.y)),
				-3.0f + (3.0f * cosf(tmpPos.x * 0.1f)),
				0.0f
			};
		}
		else
		{
			// ��l���Ƌt�����ɐ������
			Vec3 Dir = m_pHuman->GetPos() - tmpPos;
			if (Dir.y < 0.0f) Dir.y *= -1.0f;
			tmpPos += Dir;
		}
		it->RefVectorInterpolator()->SetPosTarget(tmpPos);
	}
}