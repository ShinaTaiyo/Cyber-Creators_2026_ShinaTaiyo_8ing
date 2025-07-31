//============================================================================
// 
// �Q�O�Q�T�N�T���R�O���F�V�[�����Ƃ̃v���C���[���쐬 [player.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************

// === �I�u�W�F�N�g ===
#include "player.h" // �v���C���[�i���w�b�_�j
#include "target.h" // �^�[�Q�b�g
#include "summon_manager.h" // �����}�l�[�W���[
#include "VisionBlocker.h"  // ����W�Q�I�u�W�F�N�g
#include "particle.h" // �p�[�e�B�N��
#include "StencilMask.h"

// === ���͗p ===
#include "manager.h"
#include "renderer.h"  // �f�o�C�X���g�p

// === ��� ===
#include "move_info.h" // �ړ�
#include "transparent_info.h" // �����x
#include "calculation.h" // �v�Z

// === ��� ===
#include "PlayerState_Returning.h"    // �v���C���[�߂�
#include "PlayerState_BeforeThrust.h" // �v���C���[�˂��O
#include "PlayerState_Thrusting.h"    // �v���C���[�˂�
#include "summon_manager_state_start.h" // �����}�l�[�W���[�����J�n���

// === ���̑� ===
#include "SummonGroupSelector.h" // �����O���[�v�I��

#define SHADER_ACTIVE (1)  // �V�F�[�_�[��onoff  0��1�ŕύX�ł���

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
// �R���X�g���N�^
//============================================================================
CPlayer::CPlayer() :
	CObject_X{ LAYER::DEFAULT },
	m_Velocity{ VEC3_INIT },
	m_RotTarget{ VEC3_INIT },
	m_pMesh{nullptr},
	m_pToonEffect{nullptr},
	m_pToonMap{nullptr},
	m_RimPowor{0.2f},
	m_RimIntensity{0.7f},
	m_RimThickness{0.45f},
	m_nAirPower{s_nINITAIRPOWER},
	m_nDamageFrame{0},
	m_pMoveInfo{DBG_NEW CMove_Info()},
	m_pTarget{CTarget::Create(VEC3_INIT)},
	m_pState{nullptr},
	m_Type{ CPlayer::TYPE::TUTORIAL },
	m_upSpace(nullptr)
{
	// �^�[�Q�b�g�����T�C�Y�ݒ�
	m_pTarget->SetInitSize(D3DXVECTOR3(30.0f, 30.0f, 0.0f));

	// �^�C�v�ݒ�
	CObject::SetType(CObject::TYPE::PLAYER);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayer::~CPlayer()
{
	// �����Ȃ�
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CPlayer::Init()
{
	// ������Ԃ�ݒ�
	SetState(DBG_NEW CPlayerState_Returning(this));

	// X�I�u�W�F�N�g�N���X�̏����ݒ�
	if (FAILED(CObject_X::Init()))
	{
		assert(false && "�v���C���[�̏����ݒ�Ɏ��s");
	}
#if SHADER_ACTIVE
	auto pDevice = CRenderer::GetDeviece();  // �f�o�C�X�̎擾

#ifdef _DEBUG
	InitToonShader(pDevice, "Src/Shader/fx/ToonShader.fx");
#else
	InitToonShader(pDevice, "Src/Shader/fxo/ToonShader.fxo");
#endif // _DEBUG

#endif

	// ��Ԑ���
	m_upSpace = make_unique<CSpace>();

	/* ���Ƃ��āA�v���C���[���f�� (��) ����ʉ������Ɍ����܂� */
	SetRot({ 0.0f, -D3DX_PI, 0.0f });
	SetRotTarget({ 0.0f, -D3DX_PI, 0.0f });

	// �^�[�Q�b�g�̓����x�̍ő�l�����肷��
	CTransparent_Info* pTransParent_Info = m_pTarget->GetTransparent_Info(); // �����x���
	pTransParent_Info->SetMaxValue(CPlayerState_BeforeThrust::GetNormalAnticipationFrame());
	pTransParent_Info->SetSpeed(0);
	pTransParent_Info->SetUse(true);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CPlayer::Uninit()
{
	// X�I�u�W�F�N�g�N���X�̏I������
	CObject_X::Uninit();

	// �ړ����I������
	if (m_pMoveInfo != nullptr)
	{
		delete m_pMoveInfo;
		m_pMoveInfo = nullptr;
	}

	// �v���C���[��ԏI������
	if (m_pState)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	// �^�[�Q�b�g�I������
	if (m_pTarget != nullptr)
	{
		m_pTarget = nullptr; // �|�C���^���������i����̓I�u�W�F�N�g���X�g�Łj
	}

#if SHADER_ACTIVE
	// ���b�V���̔j��
	if (m_pMesh) {
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
	// �V�F�[�_�[�̔j��
	if (m_pToonEffect) {
		m_pToonEffect->Release();
		m_pToonEffect = nullptr;
	}
	// �g�D�[���}�b�v�̔j��
	if (m_pToonMap) {
		m_pToonMap->Release();
		m_pToonMap = nullptr;
	}
#endif
}

//============================================================================
// �X�V����
//============================================================================
void CPlayer::Update()
{
	// �_���[�W���
	Damage();

	// �v���C���[��ԍX�V
	if (m_pState)
	{
		m_pState->Update(this);
	}

	// �蓁���o�X�V
	m_upSpace->Update();

	// �I�u�W�F�N�gX�̍X�V����
	CObject_X::Update();

	// �p�[�e�B�N���Ƃ̓����蔻��
	CollisionParticle();

	// ����W�Q�I�u�W�F�N�g�𐁂���΂�
	BlowVisionBlocker();

	// �C�̒l�����i���̏����ŋC�̒l���ő�ŏ��͈̔͂Ɏ��߂���Ɏ蓁�ŋC������܂��B�j
	AirPowerLimit();

	// ��]
	Rotation();

#if SHADER_ACTIVE
	// �g�D�[���V�F�[�_�[�̍X�V
	//UpdateToonShader();
#endif
}

//============================================================================
// �`�揈��
//============================================================================
void CPlayer::Draw()
{
#if SHADER_ACTIVE
	UINT numPasses = 0;

	m_pToonEffect->SetTechnique("Outline");
	m_pToonEffect->Begin(&numPasses, 0);
	for (UINT i = 0; i < numPasses; i++)
	{
		m_pToonEffect->BeginPass(i);

		Draw_UsingEffect(m_pToonEffect);
		m_pToonEffect->EndPass();
	}
	m_pToonEffect->End();


	//m_pToonEffect->SetTechnique("RimLightOnly");
	//m_pToonEffect->Begin(&numPasses, 0);
	//for (UINT i = 0; i < numPasses; i++)
	//{
	//	m_pToonEffect->BeginPass(i);

	//	Draw_UsingEffect(m_pToonEffect);
	//	m_pToonEffect->EndPass();
	//}
	//m_pToonEffect->End();

	m_pToonEffect->SetTechnique("ToonShading");
	m_pToonEffect->Begin(&numPasses, 0);

	LPDIRECT3DDEVICE9 pDev = CRenderer::GetRenderer()->GetDeviece();

	// �X�e���V���}�X�N�̉e�����󂯂����Ȃ��ꏊ�ŌĂ�
	CStencilMask::SetOtherStencilAvoid();

	for (UINT i = 0; i < numPasses; i++)
	{
		m_pToonEffect->BeginPass(i);

		// �ʏ�� Draw() �ł͂Ȃ��AEffect�p�̂��Ă�
		Draw_UsingEffect(m_pToonEffect);

		m_pToonEffect->EndPass();
	}
	m_pToonEffect->End();

	// �X�e���V���ݒ�����ɖ߂�
	CStencilMask::SetOtherResetStencilState();
#endif

	//// X�I�u�W�F�N�g�N���X�̕`�揈��
	//CObject_X::Draw();
}

//============================================================================
// ����
//============================================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 InitPos)
{
	// �C���X�^���X�𐶐�
	CPlayer* pPlayer = DBG_NEW CPlayer;

	if (pPlayer == nullptr)
	{ // �������s
		assert(false);
	}

	// �����ݒ�
	pPlayer->Init();

	// �ʒu�̐ݒ�
	pPlayer->SetPos(InitPos);

	// ���f����ݒ�
	pPlayer->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey("Player"));

	// �T�C�Y��ݒ�
	pPlayer->SetSize(pPlayer->GetModel()->Size);

	// �ő咸�_�ݒ�
	pPlayer->SetBaseVtxMax(pPlayer->GetModel()->VtxMax);

	// �ŏ����_�ݒ�
	pPlayer->SetBaseVtxMin(pPlayer->GetModel()->VtxMin);

	return pPlayer;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �蓁���o��������
//============================================================================
void CPlayer::ExecuteUpSpace()
{
	// ���̃t���[���ł̋�ԕ`�������
	CRenderer::GetRenderer()->SetDrawSpace();

	// ��Ԃ̏�ԍX�V���J�n����
	m_upSpace->Start();
}

//============================================================================
// ��]
//============================================================================
void CPlayer::Rotation()
{
	// �������擾
	D3DXVECTOR3 rot = GetRot();

	// �u���[�L��
	float fStopEnergy = 0.1f;

	// ��]���f�Ɖ�]�ʂ̌���
	if (m_RotTarget.y - rot.y > D3DX_PI)
	{
		rot.y += ((m_RotTarget.y - rot.y) * fStopEnergy + (D3DX_PI * 1.8f));
	}
	else if (m_RotTarget.y - rot.y < -D3DX_PI)
	{
		rot.y += ((m_RotTarget.y - rot.y) * fStopEnergy + (D3DX_PI * -1.8f));
	}
	else
	{
		rot.y += ((m_RotTarget.y - rot.y) * fStopEnergy);
	}

	// �������ݒ�
	SetRot(rot);
}

//============================================================================
// �_���[�W����
//============================================================================
void CPlayer::Damage()
{
	// �_���[�W�t���[�����O�ȏ�Ȃ�_���[�W�������J�n
	if (m_nDamageFrame > 0)
	{
		m_nDamageFrame--;

		// �Q�t���[�����ɐF��_�ł�����
		m_nDamageFrame % 2 == 0 ? SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)), SetUseCol(false) : SetUseCol(true);

		// �_���[�W�t���[�����O�ɂȂ����珈�����I���A�F�������g�p���Ȃ�
		if (m_nDamageFrame == 0)
		{
			SetUseCol(false);
		}

		CCamera_Manager* pCManager = CCamera_Manager::GetInstance();
		pCManager->RefRegistry().BindAtKey("Game")->SetVibration(0.01f);
	}
}

//============================================================================
// ����W�Q�I�u�W�F�N�g�𐁂���΂�
//============================================================================
void CPlayer::BlowVisionBlocker()
{
	CObject* pObj = GetTopObject(CObject::LAYER::DEFAULT); // �f�t�H���g���C���[�̐擪���X�g���擾
	const D3DXVECTOR3& Pos = GetPos(); // �ʒu

	// �ړ��ʂ̒��������߂�
	float fMoveVecLength = D3DXVec3Length(&m_Move);

	// �ړ��ʂ̃x�N�g���̒�����1.0f�ȉ��Ȃ珈�����s��Ȃ�
	if (fMoveVecLength < 1.0f)
	{
		return;
	}

	// ����W�Q�I�u�W�F�N�g��������������΂�
	while (pObj != nullptr)
	{
		CObject* pNext = pObj->GetNext();// ���̃I�u�W�F�N�g���擾

		// ��ނ��u����W�Q�I�u�W�F�N�g�v�ȊO�Ȃ珈���𒆒f
		if (pObj->GetType() != CObject::TYPE::VISIONBLOCKER)
		{
			pObj = pNext; // ���X�g�����ɐi�߂�
			continue;
		}

		// ����W�Q�I�u�W�F�N�g�Ƀ_�E���L���X�g
		CVisionBlocker* pVisionBlocker = dynamic_cast<CVisionBlocker*>(pObj);

		// ����W�Q�I�u�W�F�N�g�ւ̃|�C���^�����݂��Ȃ��Ȃ珈���𒆒f
		if (!pVisionBlocker)
		{
			pObj = pNext; // ���X�g�����ɐi�߂�
			continue;
		}

		const D3DXVECTOR3& VisionBlockerPos = pVisionBlocker->GetPos();

		// ���������߂�
		float fLength = Calculation::Length::PointToPoint(Pos, VisionBlockerPos);

		// ��苗���ȉ��Ő�����΂�
		if (fLength < s_fVisionBlockerBlowLength)
		{
			D3DXVECTOR3 Dir = VisionBlockerPos - Pos;
			D3DXVec3Normalize(&Dir, &Dir);
			pVisionBlocker->SetBlowMove(Dir * s_fVisionBlockerBlowSpeed);
			pVisionBlocker->SetBlowInertia(s_fVisionBlockerBlowInertia);
		}

		pObj = pNext;//���X�g�����ɐi�߂�
	}

}

//============================================================================
// �C���ő�l�𒴂��Ȃ��悤�ɂ���
//============================================================================
void CPlayer::AirPowerLimit()
{
	m_nAirPower = Calculation::Value::LimitRange(m_nAirPower, 0, s_nMaxAirPower);
}

//============================================================================
// �p�[�e�B�N���Ƃ̓����蔻��
//============================================================================
void CPlayer::CollisionParticle()
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);  // �f�t�H���g���C���[�̐擪���X�g���擾

// === ���菈���J�n ===

// �v���C���[�Ɣ��肷��I�u�W�F�N�g�̌���
	while (pObj != nullptr)
	{
		CObject* pNext = pObj->GetNext();// ���̃I�u�W�F�N�g���擾

		if (pObj->GetType() != CObject::TYPE::PARTICLE)
		{
			pObj = pNext;
			continue;
		}

		CParticle* pParticle = dynamic_cast<CParticle*>(pObj);

		if (pParticle == nullptr)
		{
			pObj = pNext;
			continue;
		}

		// === ����Ɏg�p�������錾 ===

		D3DXVECTOR3 Vec = pParticle->GetPos() - GetPos(); // �x�N�g��
		float fVecLength = D3DXVec3Length(&Vec);          // �x�N�g���̒���
		float fPlayerRadius = GetSize().x;            // �v���C���[�̔��a
		float fParticleRadius = pParticle->GetSize().x;   // �p�[�e�B�N���̔��a

		// === ����J�n ===

		// ���̓����蔻����s���A����������p�[�e�B�N����j��
		// ���P�F���݂��̔��a�����������Z����Γ������Ă���
		// ���Q�F�v���C���[�ɂ���������̂������邽�߁A�o���U�O�t���[�����傫����Γ�����
		if (fVecLength < fPlayerRadius + fParticleRadius && pParticle->GetCntFrame() > 60)
		{
			pParticle->SetRelease();
		}

		pObj = pNext;
	}
}

//============================================================================
// �C�̍ő�l���擾
//============================================================================
const int& CPlayer::GetMaxAirPower()
{
	return s_nMaxAirPower;
}

//============================================================================
// �ړ��ʎ擾
//============================================================================
const D3DXVECTOR3& CPlayer::GetMove() const
{
	return m_Move;
}
//============================================================================
// �^�[�Q�b�g�ւ̃|�C���^�擾
//============================================================================
CTarget* CPlayer::GetTarget() const
{
	return m_pTarget;
}


//============================================================================
// �C���擾����
//============================================================================
const int& CPlayer::GetAirPower() const
{
	return m_nAirPower;
}

//============================================================================
// �C��ݒ肷��
//============================================================================
void CPlayer::SetAirPower(int nPower)
{
	m_nAirPower = nPower;
}

//============================================================================
// �v���C���[��Ԏ擾
//============================================================================
CPlayerState* CPlayer::GetState() const
{
	return m_pState;
}

//============================================================================
// �v���C���[��Ԑݒ�
//============================================================================
void CPlayer::SetState(CPlayerState* State)
{
	// �|�C���^�����݂���Ȃ�j������
	if (m_pState != nullptr)
	{
		m_pState->Uninit(this); // �I������
		delete m_pState;
	}

	// �V������Ԃ�ݒ�
	m_pState = State;
	m_pState->Init(this);
}

//============================================================================
// �^�C�v�擾
//============================================================================
const CPlayer::TYPE& CPlayer::GetType() const
{
	return m_Type;
}

//============================================================================
// �^�C�v�ݒ�
//============================================================================
void CPlayer::SetType(TYPE Type)
{
	m_Type = Type;
}

//============================================================================
// �_���[�W���o
//============================================================================
void CPlayer::SetDamage(int nFrame)
{
	if (m_nDamageFrame == 0)
	{
		m_nDamageFrame = nFrame; // �_���[�W��Ԃɂ���t���[������ݒ�
		SetUseCol(true); // �F�������g�p����

		// �����}�l�[�W���[�ւ̃|�C���^�擾
		CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER)); 

		CSummon_Manager_State_Start* pStart = dynamic_cast<CSummon_Manager_State_Start*>(pSummonManager->GetState());

		// �X�e�[�g�Ȃ̂łȂ���������܂��B��O���������ɏ����𒆒f
		if (!pStart)
		{
			return;
		}

		CSummonGroupSelector* pGroupSelector = dynamic_cast<CSummonGroupSelector*>(pStart->GetSummonGroupSelector());

		// �_���[�W���󂯂��̂œ�Փx�X�R�A��啝�ɉ����܂��B
		pGroupSelector->SetDifficultyScore(pGroupSelector->GetDifficultyScore() - 4);
		pStart->SetSuccessPhase(false); // ����̃t�F�[�Y�͎��s��Ԃɂ��܂��B
	}
}

//============================================================================
// �V�F�[�_�[�̏�����
//============================================================================
bool CPlayer::InitToonShader(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
	m_fxFilePath = fxFile; // �p�X�ۑ�

	LPD3DXBUFFER pErr = nullptr;
#ifdef _DEBUG
	// �f�o�b�N��
	HRESULT hr = D3DXCreateEffectFromFileA(
		device, fxFile, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_pToonEffect, &pErr);
#else	
	// �����[�X����fxo��ǂݍ���
	HRESULT hr = D3DXCreateEffectFromFileA(
		device,
		fxFile, // .fx �ł͂Ȃ� .fxo ���w��
		nullptr,
		nullptr,
		D3DXFX_NOT_CLONEABLE,  // �o�C�i������ǂݍ��ނƂ��̃t���O(�Ӗ��F���̃G�t�F�N�g�͕����s�ł���)
		nullptr,
		&m_pToonEffect,
		&pErr);
#endif	

	if (FAILED(hr))
	{
		if (pErr)
		{
			MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Compile Error", MB_OK);
			pErr->Release();
		}
		return false;
	}

	// �g�D�[���}�b�v�ǂݍ���
	if (FAILED(D3DXCreateTextureFromFileA(device, "Data/TEXTURE/ToonMap.png", &m_pToonMap)))
	{
		MessageBoxA(nullptr, "ToonMap.png �̓ǂݍ��݂Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}

	return true;
}

//============================================================================
// �V�F�[�_�[�̕`��
//============================================================================
void CPlayer::Draw_UsingEffect(LPD3DXEFFECT pEffect)
{
	auto pDevice = CRenderer::GetDeviece();  // �f�o�C�X�̎擾
	auto model = GetModel();   // ���f���̎擾
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game");  // �Q�[���V�[���̃J�����̎擾
	D3DXVECTOR3 cameraPos = pCamera->GetPos();  // �J�����̈ʒu���擾
	auto pLight = CManager::GetManager()->GetLight();  // ���C�g�̃|�C���^�擾
	D3DXVECTOR3 lightDir = pLight->GetLightDir();   // ���C�g�̈ʒu���擾
	bool bUseCol = GetUseCol();  // �F���ς������

	// ���f���̃��[���h�}�g���b�N�X���g���� WVP �쐬
	D3DXMATRIX world, view, proj, wvp, vp;
	world = GetMtxWorld();
	D3DXMATRIX matWorldNoScale, matWorldInvTrans;

	pDevice->GetTransform(D3DTS_VIEW, &view);
	pDevice->GetTransform(D3DTS_PROJECTION, &proj);
	wvp = world * view * proj;
	vp = view * proj;
	m_pToonEffect->SetMatrix("WorldViewProj", &wvp);
	//m_pToonEffect->SetMatrix("gView", &view);
	//m_pToonEffect->SetMatrix("gProj", &proj);

	// ���C�g�𐳋K��
	D3DXVec3Normalize(&lightDir, &lightDir);
	m_pToonEffect->SetVector("LightDirection", (D3DXVECTOR4*)&lightDir);
	m_pToonEffect->SetVector("gCameraPos", (D3DXVECTOR4*)&cameraPos);
	m_pToonEffect->SetFloat("RimPower", m_RimPowor);      // ����
	m_pToonEffect->SetFloat("RimIntensity", m_RimIntensity);  // �s��
	m_pToonEffect->SetFloat("RimThickness", m_RimThickness);  // ����
	m_pToonEffect->SetFloat("OutlineThickness", 1.05f);   // �A�E�g���C���̑���
	//	// �G�t�F�N�g�ݒ�p�X�P�[���E��]�E���s�ړ�
	//D3DXVECTOR3 scale, translation;
	//D3DXQUATERNION rotation;

	//// ���f���s��iWorld�j
	//D3DXMatrixIdentity(&world); // ��]�E�ړ������� 

	//D3DXVECTOR3 eye(0, 2, -5), at(0, 0, 0), up(0, 1, 0);

	//// �r���[�E�v���W�F�N�V�����s��
	//D3DXMatrixLookAtLH(&view,
	//	&eye,  // Eye
	//	&at,   // LookAt
	//	&up);  // Up

	//// �ˉe�s��
	//D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4, 640.0f / 480.0f, 1.0f, 100.0f);

	//D3DXVECTOR3 Scaling = D3DXVECTOR3(1, 1, 1);  // �X�P�[��

	//D3DXMatrixDecompose(&scale, &rotation, &translation, &world);
	//D3DXMatrixTransformation(&matWorldNoScale, nullptr, nullptr, &Scaling, nullptr, &rotation, &translation);

	//// �@���ϊ��p�FWorld�̃X�P�[�����O�����O���ċt�]�u�s����쐬
	//D3DXMatrixInverse(&matWorldInvTrans, nullptr, &world);
	//D3DXMatrixTranspose(&matWorldInvTrans, &matWorldInvTrans);

	//pEffect->SetMatrix("gWorldInverseTranspose", &matWorldInvTrans);  // ���O�����X�P�[�����O���G�t�F�N�g�ɓ����


	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

	for (WORD wCntMat = 0; wCntMat < static_cast<WORD>(model->dwNumMat); wCntMat++)
	{
		// �K�p����F�ibUseCol��true�Ȃ�GetCol()�Afalse�Ȃ猳�̐F�j
		D3DXCOLOR diffuseColor = bUseCol ? GetCol() : model->apColMat[wCntMat];

		// �G�t�F�N�g�ɐݒ�
		pEffect->SetVector("MaterialDiffuse", (D3DXVECTOR4*)&diffuseColor);

		// �ꎞ�I�Ƀ}�e���A�����쐬���ĐF�����ύX
		D3DMATERIAL9 tempMat = pMat[wCntMat].MatD3D;
		tempMat.Diffuse = diffuseColor;
		pDevice->SetMaterial(&tempMat);  // ���̃}�e���A�����󂳂Ȃ�

		// �e�N�X�`���ݒ�
		if (model->apTex[wCntMat]) {
			pEffect->SetBool("UseTexture", TRUE);
			pDevice->SetTexture(0, model->apTex[wCntMat]);
			pEffect->SetTexture("DiffuseTexture", model->apTex[wCntMat]);
		}
		else {
			pEffect->SetBool("UseTexture", FALSE);
		}

		// ���b�V���̕`��
		model->pMesh->DrawSubset(wCntMat);
	}
}
