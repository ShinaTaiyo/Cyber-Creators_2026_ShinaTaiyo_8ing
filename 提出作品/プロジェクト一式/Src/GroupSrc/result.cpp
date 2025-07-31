//============================================================================
// 
// ���U���g [result.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "result.h"
#include "sound.h"

// �J�ڐ�̃V�[�����
#include "title.h"

// ���͎擾�p
#include "manager.h"

// �t�H�O�ݒ�p
#include "renderer.h"

// �J�����擾�p
#include "camera_manager.h"

// �I�u�W�F�N�g�Ǘ��p
#include "texture_manager.h"
#include "object_HUD.h"
#include "object_X.h"
#include "object_parts.h"
#include "motion_set.h"

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
// �X�V����
//============================================================================
void CResult::Update()
{
	// ���N���X�̍X�V����
	CScene::Update();

	// �t�F�[�Y�̍X�V����
	UpdatePhase();

	// �^�C�g����ʂ�
	auto* pPad = CManager::GetPad();
	if ((CManager::GetManager()->GetKeyboard()->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputPad::JOYKEY::A) ||
		pPad->GetTrigger(CInputPad::JOYKEY::START)) && m_nPhase == 7)
	{
		To_Next();
	}
}

//============================================================================
// �`�揈��
//============================================================================
void CResult::Draw()
{
	// ���N���X�̕`�揈��
	CScene::Draw();
}

//============================================================================
// ���̃V�[����
//============================================================================
void CResult::To_Next()
{
	// �G�̉���\��
	if (m_pEnemy)
	{
		m_pEnemy->SetRelease();
	}

	// �^�����̉���\��
	if (m_pFakeField)
	{
		m_pFakeField->SetRelease();
	}

	// �R���f���̉���\��
	if (m_pMountain)
	{
		m_pMountain->SetRelease();
	}

	// �S�I�u�W�F�N�g�̎��S�t���O�𔭓�����
	CObject::AllSetRelease();

	// �^�C�g���V�[���֕ύX
	CScene_Manager::ChangeScene(CTitle::Create());
}

//============================================================================
// ���U���g�𐶐�
//============================================================================
CResult* CResult::Create()
{
	// ���U���g�̃C���X�^���X�𐶐�
	CResult* pScene = DBG_NEW CResult();

	// �������s
	if (pScene == nullptr)
	{
		throw std::runtime_error("Failed to create result scene");
	}

	// ����������
	if (FAILED(pScene->Init()))
	{
		throw std::runtime_error("Failed to initialize result scene");
	}

	// �C���X�^���X�̃|�C���^��Ԃ�
	return pScene;
}

//============================================================================
// �R���f�������p��
//============================================================================
void CResult::SetMountain(CObject_X* p)
{
	if (!p)
	{
		return;
	}

	// �|�C���^�������p��
	m_pMountain = p;

	// �R(��)���[�V�����Z�b�g����
	if (!m_pMountBreakable)
	{
		const JSON& ParamMountain = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\MOTIONSET\\Mount_Breakable.json");
		m_pMountBreakable = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, ParamMountain);

		// �����l�̈ꕔ���A�����p�����R���f���̂��̂ŏ㏑��
		m_pMountBreakable->SetNowMotion(1);
		m_pMountBreakable->SetPos(m_pMountain->GetPos());
		m_pMountBreakable->GetParentParts()->SetScaleOffset(m_pMountain->GetScale());
	}

	// �R���f���̕\�����Ƃ���
	m_pMountain->SetScale(VEC3_INIT);
}

//============================================================================
// 
// protected�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CResult::CResult() :
	CScene(),
	m_nPheseComboCntKeep(-1),
	m_pAtk(nullptr),
	m_pTY(nullptr),
	m_pEnemy(nullptr),
	m_pHuman(nullptr),
	m_pFakeField(nullptr),
	m_pMountain(nullptr),
	m_pMountBreakable(nullptr),
	m_nPhase(0),
	m_nFrameCounter(0)
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CResult::~CResult()
{}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CResult::Init()
{
	/* �Q�[���p�̓V���𐶐� */
	const JSON& ParamSkyDome = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Game_SkyDome.json");
	CObject_X::Create(ParamSkyDome);

	// �����_���[�̃|�C���^
	CRenderer* pRenderer = CRenderer::GetRenderer();

	//// �t�H�O�̎n�_���Z�b�g
	//pRenderer->SetFogStart(450.0f);

	//// �t�H�O�̏I�_���Z�b�g
	//pRenderer->SetFogEnd(700.0f);

	//// �t�H�O�̃J���[���Z�b�g
	//pRenderer->SetFogCol({ 1.0f, 1.0f, 1.0f ,1.0f });

	// ����̃J�����Ńg�����W�V������ݒ�
	CCamera_Manager::GetInstance()->SetTransition("Result");

#if 0
	// ���̃J�����ɁARoll����]���������
	Compose(CCamera_Manager::GetInstance()->RefSelectCamera().RefRotatorRoll(),
		[]() -> up<CRotatorRoll>
		{
			up<CRotatorRoll> up = make_unique<CRotatorRoll>();

			up->SetFlags(RotatorRollFlags::Rolling);
			up->SetSpeed(0.0025f);
			up->SetAngle(D3DX_PI * 0.35f);

			return up;
		});
#endif

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CResult::Uninit()
{}

//============================================================================
// �t�F�[�Y�̍X�V����
//============================================================================
void CResult::UpdatePhase()
{
	if (m_nPhase == 0)
	{
		// �G���[�V������_�����ɕύX
		if (m_pEnemy->GetNowMotion() != 0)
		{
			m_pEnemy->SetNowMotion(0);
		}
		else
		{
			// ��l�����[�V�����Z�b�g�𐶐�
			if (!m_pHuman)
			{
				m_pHuman = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\MOTIONSET\\Karate_Human.json"));
				m_pHuman->SetNowMotion(1);
				m_pHuman->GetParentParts()->SetScaleOffset({ 0.35f, 0.35f, 0.35f });
				m_pHuman->SetRot({ 0.0f, D3DX_PI, 0.0f });
				m_pHuman->SetPos({ 0.0f, 0.0f, -600.0f });
			}

			//�t�F�[�Y���X�V
			++m_nPhase;
		}
	}
	else if (m_nPhase == 1)
	{
		// �J����������

		// �G�������������
		Vec3 EnemyRot = m_pEnemy->GetRot();
		ExponentialDecay(EnemyRot.y, 0.0f, 0.1f);

		// ��l���������Ă���
		Vec3 HumanPos = m_pHuman->GetPos();
		if (HumanPos.z < 1500.0f)
		{
			HumanPos.z += 10.0f;
		}
		else
		{
			// �ړ��������s��
			HumanPos.z = 1500.0f;

			// �t�F�[�Y��i�s
			++m_nPhase;
		}
		m_pHuman->SetPos(HumanPos);
	}
	else if (m_nPhase == 2)
	{
		// ��l�����[�V�����𗭂߃��[�V�����ɕύX
		if (m_pHuman->GetNowMotion() != 11)
		{
			m_pHuman->SetNowMotion(11);

			// ���[�V�����ύX�̃^�C�~���O�ŃJ�����̖ڕW�l���ύX
			CCamera& refCam = CCamera_Manager::GetInstance()->RefSelectCamera();
			refCam.SetPosTarget({ 0.0f, 0.0f, 1500.0f });
			refCam.SetDistanceTarget(500.0f);

			// �U���Ñ���\��
			const JSON& AtkParam = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\HUD\\Atk.json");
			m_pAtk = CObject_HUD::Create(AtkParam);
			m_pAtk->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("Atk"));
		}
		else
		{
			// �������ԃJ�E���g
			++m_nFrameCounter;

			// �U���Ñ���k�킷
			Vec3 posAtk = m_pAtk->GetPos();
			posAtk +=
			{
				hws::GetRandomValue<float>() * 0.01f,
				hws::GetRandomValue<float>() * 0.01f,
				hws::GetRandomValue<float>() * 0.01f
			};
			m_pAtk->SetPos(posAtk);

			// �C�x���g�ɍ��킹�ă{�^�����́A�܂���30�b���u��
			if (m_nFrameCounter > 1800 ||
				CManager::GetManager()->GetKeyboard()->GetTrigger(DIK_J) ||
				CManager::GetManager()->GetPad()->GetTrigger(CInputPad::JOYKEY::A))
			{
				// �J�E���g�����Z�b�g���t�F�[�Y��i�s
				m_nFrameCounter = 0;
				++m_nPhase;

				/* �� - �˂����[�V�����ɑ΂��ăv���C���[�̈ʒu���C�� */
				Vec3 posHuman = m_pHuman->GetPos();
				posHuman.y += -60.0f;
				m_pHuman->SetPos(posHuman);

				// ��l����˂����[�V�����ɕύX
				m_pHuman->SetNowMotion(8);
				
				// �R(��)�𑹉󃂁[�V�����ɕύX
				m_pMountBreakable->SetNowMotion(0);

				// �h�J��
				CSound::GetInstance()->Play(CSound::LABEL::DOKA);

				// �U���Ñ��������Ȃ�����
				m_pAtk->SetSizeTarget(VEC3_INIT);
				m_pAtk->SetRotTarget({ 0.0f, 0.0f, -10.0f });
				m_pAtk->SetColTarget(XCOL_INIT);
			}
		}
	}
	else if (m_nPhase == 3)
	{
		SetCamImpact("Charge_0");
	}
	else if (m_nPhase == 4)
	{
		SetCamImpact("Charge_1");
	}
	else if (m_nPhase == 5)
	{
		SetCamImpact("Charge_2");
	}
	else if (m_nPhase == 6)
	{
		// ���ӂ�\��
		const JSON& TYParam = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\HUD\\TY.json");
		m_pTY = CObject_HUD::Create(TYParam);
		m_pTY->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("TY"));

		// �t�F�[�Y�i�s
		++m_nPhase;
	}
}

//============================================================================
// ���߉���ɍ��킹���J�����̃Z�b�g
//============================================================================
void CResult::SetCamImpact(const char* key)
{
	// 1�b�J�E���g
	++m_nFrameCounter;

	if (m_nFrameCounter > 60)
	{
		// �J�E���^�[�����Z�b�g���t�F�[�Y��i�s
		m_nFrameCounter = 0;
		++m_nPhase;

		// ��l����˂����[�V�����ɕύX
		m_pHuman->SetNowMotion(8);

		// �R(��)�𑹉󃂁[�V�����ɕύX
		m_pMountBreakable->SetNowMotion(0);

		// �h�J��
		CSound::GetInstance()->Play(CSound::LABEL::DOKA);

		// �g�p�J�����𗭂ߎg�p���p�̃��m�ɕύX
		CCamera_Manager::GetInstance()->SetSelectKey(key);
	}
}