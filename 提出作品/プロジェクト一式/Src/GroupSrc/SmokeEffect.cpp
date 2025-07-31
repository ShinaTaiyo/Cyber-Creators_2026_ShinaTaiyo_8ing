//==============================================
// 
// �|�X�g�G�t�F�N�g[��] <SmokeEffect.cpp>
// Auther �ؓc�А�
// 
//==============================================

#include "SmokeEffect.h"   // ���w�b�_�[�g�p
#include "manager.h"    // �}�l�[�W���[���g�p
#include "renderer.h"   // �f�o�C�X�擾
#include "game.h"       // �Q�[���̃|�C���^���g�p
#include "SummonGroupSelector.h"  // �����O���[�v�Ǘ��N���X���g�p
#include "summon_manager_state_start.h"  // �����}�l�[�W���[��ԃN���X�̎d�l

//====================================
// �ÓI�����o�錾
//====================================
bool CSmokeEffect::s_bUseDraw = false; // �`�悷�邩�ǂ���

//====================================
// �R���X�g���N�^
//====================================
CSmokeEffect::CSmokeEffect()
{
	m_pEffect = nullptr;   // �G�t�F�N�g�̏�����
	m_noiseTex = nullptr;  // �e�N�X�`���̏�����
	m_quadVB = nullptr;    // �o�b�t�@�̏�����
    m_time = 0.0f;    // �^�C���̏�����
	m_Opacity = 1.0f; // �Z�x�̏�����
	m_enableEffect = false;  // �G�t�F�N�g�̕\�����
	m_bActive = true;
	s_bUseDraw = true; // �`����g�p����
}
//====================================
// �f�X�g���N�^
//====================================
CSmokeEffect::~CSmokeEffect()
{
}
//====================================
// �����ݒ�
//====================================
HRESULT CSmokeEffect::Init()
{
	// �f�o�C�X�̎擾
	auto pDevice = CRenderer::GetDeviece();

	if (FAILED(CObject_2D::Init()))
	{
		return E_FAIL;
	}

	// �V�F�[�_�[�̓ǂݍ���
#ifdef _DEBUG
	InitSmokeEffect(pDevice, "Src/Shader/fx/SmokeEffect.fx");
#else
	InitSmokeEffect(pDevice, "Src/Shader/fxo/SmokeEffect.fxo");
#endif // _DEBUG


	return S_OK;
}
//====================================
// �I������
//====================================
void CSmokeEffect::Uninit()
{
	// �G�t�F�N�g�̔j��
	if (m_pEffect) {
		m_pEffect->Release();
		m_pEffect = nullptr;
	}
	// �e�N�X�`���̔j��
	if (m_noiseTex) {

		m_noiseTex->Release();
		m_noiseTex = nullptr;
	}
	// �o�b�t�@�̔j��
	if (m_quadVB) {
		m_quadVB->Release();
		m_quadVB = nullptr;
	}

}
//====================================
// �X�V����
//====================================
void CSmokeEffect::Update()
{
#ifdef _DEBUG
	// ImGui�̂��
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin(u8"���̐ݒ�"))
	{
		ImGui::Checkbox(u8"�V�F�[�_�[��\��", &m_enableEffect);

		ImGui::Text(u8"���̔Z���̐ݒ�");

		ImGui::SliderFloat(u8"�Z��", &m_Opacity, 1.0f, 5.5f);

		ImGui::End();
	}
#endif

	UpdateSmokeEffect(0.05f);  // �V�F�[�_�[�̍X�V
}
//====================================
// �`�揈��
//====================================
void CSmokeEffect::Draw()
{
	if (s_bUseDraw)
	{
		// �f�o�C�X�̎擾
		auto pDevice = CRenderer::GetDeviece();

		RenderSmokeEffect(pDevice);
	}
}
//====================================
// ��������
//====================================
CSmokeEffect* CSmokeEffect::Create()
{
	CSmokeEffect* pSmoke = DBG_NEW CSmokeEffect();
	pSmoke->Init();
	return pSmoke;
}

//====================================
// �`�悷�邩�ǂ����ݒ�
//====================================
void CSmokeEffect::SetUseDraw(bool bUse)
{
	s_bUseDraw = bUse;
}

//====================================
// �V�F�[�_�[�̏�����
//====================================
bool CSmokeEffect::InitSmokeEffect(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
	m_fxFilePath = fxFile; // �p�X�ۑ�

	LPD3DXBUFFER pErr = nullptr;
#ifdef _DEBUG
	// �f�o�b�N��
	HRESULT hr = D3DXCreateEffectFromFileA(
		device, fxFile, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_pEffect, &pErr);
#else	
	// �����[�X����fxo��ǂݍ���
	HRESULT hr = D3DXCreateEffectFromFileA(
		device,
		fxFile, // .fx �ł͂Ȃ� .fxo ���w��
		nullptr,
		nullptr,
		D3DXFX_NOT_CLONEABLE,  // �o�C�i������ǂݍ��ނƂ��̃t���O(�Ӗ��F���̃G�t�F�N�g�͕����s�ł���)
		nullptr,
		&m_pEffect,
		&pErr);
#endif	

	// �ǂݍ��݃G���[���������ꍇ�G���[�\�����o��
	if (FAILED(hr)){
		if (pErr){
			MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Compile Error", MB_OK);
			pErr->Release();
			pErr = nullptr;
		}
		return false;
	}

    // �m�C�Y�e�N�X�`���ǂݍ���
    D3DXCreateTextureFromFile(device, "Data/TEXTURE/fireNoice.png", &m_noiseTex);

    // �X�N���[���N�A�b�h�쐬
    InitScreenQuad(device); 

	return true;
}
//====================================
// �V�F�[�_�[�̍X�V
//====================================
void CSmokeEffect::UpdateSmokeEffect(float deltaTime)
{
	m_time += deltaTime;  // �A�j���[�V�����̑��x��ϐ��ɓ����

	// �����}�l�[�W���[�ւ̃|�C���^�擾
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));

	// �T�����}�l�[�W���[�X�e�[�g�X�^�[�g�̃|�C���^�����擾
	CSummon_Manager_State_Start* pStart = dynamic_cast<CSummon_Manager_State_Start*>(pSummonManager->GetState());

	if (pStart)
	{
		// �T�����O���[�v�Z���N�^�[�̃|�C���^���擾
		CSummonGroupSelector* pSelecter = dynamic_cast<CSummonGroupSelector*>(pStart->GetSummonGroupSelector());

		if (pStart->GetStartPhase()) {  // �t�F�[�Y���̎�
			m_enableEffect = true;  // �����N��

			// �����O���[�v�I���ւ̃|�C���^�����݂��Ă���Ȃ疶�̔Z���̐ݒ���s��
			if (pSelecter)
			{
				// ��Փx�ɂ���Ė��̔Z�����ς��
				if (pSelecter->GetDifficultyRank() == CSummon_Manager::DIFFICULTY::EASY) {
					m_Opacity = 2.0f;
				}
				else if (pSelecter->GetDifficultyRank() == CSummon_Manager::DIFFICULTY::NORMAL) {
					m_Opacity = 3.5f;
				}
				else if (pSelecter->GetDifficultyRank() == CSummon_Manager::DIFFICULTY::HARD) {
					m_Opacity = 5.5f;
				}
				else {

				}
			}
		}
		else {  // �t�F�[�Y���ł͂Ȃ��Ƃ�
			m_enableEffect = false;
		}
	}
	// �����J�n��Ԃւ̃|�C���^�����݂��Ă��Ȃ��̂Ŗ��̔Z�����Œ�ɂ���
	else{
		m_Opacity = 0.0f;
	}

	// �p�����[�^�[�̐ݒ�
	m_pEffect->SetFloat("g_Time", m_time);  // �A�j���[�V�����^�C��
	m_pEffect->SetFloat("g_Opacity", m_Opacity);  // �Z��


}
//====================================
// �V�F�[�_�[�̕`��
//====================================
void CSmokeEffect::RenderSmokeEffect(LPDIRECT3DDEVICE9 device)
{
	if (!m_enableEffect)
	{// �G�t�F�N�g�̕\�����
		return;  // false�̎��͕`�悵�Ȃ�
	}

    CObject_2D::Draw(); // �w�i�`��

	// �e�u�����h�K��
	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); 

	// �G�t�F�N�g�̐ݒ�
    m_pEffect->SetTechnique("SmokeEffect");
    m_pEffect->SetTexture("g_NoiseTex", m_noiseTex);   // �e�N�X�`��
    m_pEffect->SetFloat("g_Time", m_time);   // �A�j���[�V�����^�C��
    m_pEffect->SetFloat("g_Scale", 1.5f);    // �X�P�[��
    m_pEffect->SetFloat("g_Speed", 0.1f);    // ���x
    m_pEffect->SetFloat("g_Opacity", m_Opacity);  // �Z��
    m_pEffect->SetFloatArray("g_SmokeColor", D3DXVECTOR3(1.0f, 1.0f, 1.0f), 3);  // �F

    m_pEffect->Begin(nullptr, 0);
    m_pEffect->BeginPass(0);

    // �X�N���[���N�A�b�h�`��
    device->SetStreamSource(0, m_quadVB, 0, sizeof(Vertex));
    device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

    m_pEffect->EndPass();
    m_pEffect->End();

	// �u�����h��߂�
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    device->SetRenderState(D3DRS_ZENABLE, TRUE);

}

//====================================
// �|���S���̏�����
//====================================
void CSmokeEffect::InitScreenQuad(LPDIRECT3DDEVICE9 device)
{
	device->CreateVertexBuffer(4 * sizeof(Vertex), 0, SMOKE_FVF,
		D3DPOOL_MANAGED, &m_quadVB, nullptr);

	Vertex* v;
	m_quadVB->Lock(0, 0, (void**)&v, 0);

	v[0] = { 0,                (float)SCR_HEIGHT, 0, 1.0f, 0.0f, 1.0f }; // ����
	v[1] = { 0,                0,                 0, 1.0f, 0.0f, 0.0f }; // ����
	v[2] = { (float)SCR_WIDTH, (float)SCR_HEIGHT, 0, 1.0f, 1.0f, 1.0f }; // �E��
	v[3] = { (float)SCR_WIDTH, 0,                 0, 1.0f, 1.0f, 0.0f }; // �E��

	m_quadVB->Unlock();
}
