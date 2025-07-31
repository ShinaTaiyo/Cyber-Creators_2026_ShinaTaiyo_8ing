//================================
//
//�u�C�v�̒~��(EffectAccumulation.cpp)
// Auther kida ibuki
// 
//================================
#include "renderer.h"  // �f�o�C�X���g�p���邽��
#include "EffectAccumulation.h"   // ���w�b�_�[
#include "manager.h"   // �}�l�[�W���[�̎d�l
#include "game.h"      // �Q�[���̃|�C���^���g�p
#include "GamePlayer.h"   // �C�̍ő�p���[���擾����̂Ɏg�p
#include "summon_manager_state_start.h"  // �����}�l�[�W���[��ԃN���X�̃|�C���^���g�p


//======================================
// �R���X�g���N�^
//======================================
CEffectAccumulation::CEffectAccumulation()
{
	m_sceneRenderTarget = nullptr;   // �����_���[�^�[�Q�b�g
	m_fireTexture = nullptr;   // �e�N�X�`��
	m_fireEffect = nullptr;   // �G�t�F�N�g
    m_fireTime = 0.0f;  // �A�j���[�V�����̑��x
    m_sceneSurface = nullptr;   // �T�[�t�F�X
    m_fullscreenVB = nullptr;  // �t���X�N���[���N�A�b�h
    // �e�N�X�`���̐F
    m_colorR = 1.0f;
    m_colorG = 1.0f;
    m_colorB = 1.0f;

    // �~�Ɛ��̋���
    m_circlePowor = 0.3f;
    m_sharpnessPowor = 0.1f;
    m_totalDeformPower = 1.2f;

    m_circleSize = 2.0f;  // �G�t�F�N�g�̃T�C�Y(Update���ŉ��Z�����珙�X�ɑ�����B�L�[���������Ƃ��Ȃ�)2.0~6.0�����z����

    m_enableEffect = true;  // �G�t�F�N�g�̕\�����
    m_bActive = true;
    m_bSwitch = true;

    m_pSuummonManager = nullptr;

    // �C�̃p�����[�^�[�p�ϐ��̏�����
    baseValue = 1.0f;
    pressCount = 0;
    powerPerPress = 0.05f;   // �������Ƃ̑�����
    maxPower = 2.0f;


}
//======================================
// �f�X�g���N�^
//======================================
CEffectAccumulation::~CEffectAccumulation()
{
}
//======================================
// ����������
//======================================
HRESULT CEffectAccumulation::Init()
{
    SetType(CObject::TYPE::SPIRIT);  // �^�C�v�̐ݒ�

    // �f�o�C�X�̎擾
    auto pDevice = CRenderer::GetDeviece();

    if (FAILED(CObject_2D::Init()))
    {// 2D�I�u�W�F�N�g�̏�����
        return E_FAIL;
    }

    // �V�F�[�_�[�̓ǂݍ���
#ifdef _DEBUG
    InitPostEffect(pDevice, "Src/Shader/fx/EffectAccumulation.fx");  // �V�F�[�_�[�̏����� & �Ăяo��
#else
    InitPostEffect(pDevice, "Src/Shader/fxo/EffectAccumulation.fxo");  // �V�F�[�_�[�̏����� & �Ăяo��
#endif // !_DEBUG

	return S_OK;
}
//======================================
// �I������
//======================================
void CEffectAccumulation::Uninit()
{
    ShutdownPostEffect();   // �V�F�[�_�[�̕ϐ��j��

    CObject_2D::Uninit();  // 2D�I�u�W�F�N�g�̔j��

    // �����}�l�[�W���[�I��
    if (m_pSuummonManager != nullptr)
    {
        delete m_pSuummonManager;    // ���I�������̉��
        m_pSuummonManager = nullptr; // �|�C���^�̏�����
    }

}
//======================================
// �X�V����
//======================================
void CEffectAccumulation::Update()
{
    //CObject_2D::Update();

#ifdef _DEBUG
    // ImGui�̂��
    ImGui::SetNextWindowSize({ -1, -1 });
    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
    if (ImGui::Begin(u8"�C�̃V�F�[�_�["))
    {
        ImGui::Checkbox(u8"Gui���J��", &m_bActive);
        if (!m_bActive) {

            ImGui::Text(u8"�F�̒���");
            ImGui::SliderFloat("Color R", &m_colorR, 0.0f, 1.0f);
            ImGui::SliderFloat("Color G", &m_colorG, 0.0f, 1.0f);
            ImGui::SliderFloat("Color B", &m_colorB, 0.0f, 1.0f);

            ImGui::Text(u8"�C�̋���");
            ImGui::SliderFloat(u8"�~�̋���", &m_circlePowor, 0.0f, 0.5f);
            ImGui::SliderFloat(u8"���̋���", &m_sharpnessPowor, 0.0f, 0.5f);
            ImGui::SliderFloat(u8"�S�̂̋���", &m_totalDeformPower, 0.0f, 2.0f);

            ImGui::Text(u8"�C�̑傫��");
            ImGui::SliderFloat(u8"�~�̑傫��", &m_circleSize, 2.0f, 10.0f);
            ImGui::Checkbox(u8"�\�����", &m_enableEffect);
        }
        ImGui::End();
    }
#endif

    UpdatePostEffect(0.05f);  // �V�F�[�_�[�̍X�V(����:�A�j���[�V�����̑��x)
}
//======================================
// �`�揈��
//======================================
void CEffectAccumulation::Draw()
{
    // �f�o�C�X�̎擾
    auto pDevice = CRenderer::GetDeviece();

    RenderEffect(pDevice);  // �V�F�[�_�[�̕`��
}
//======================================
// ��������
//======================================
CEffectAccumulation* CEffectAccumulation::Create()
{
    CEffectAccumulation* pEffect = DBG_NEW CEffectAccumulation();
	pEffect->Init();
	return pEffect;
}
//======================================
// �V�F�[�_�[�̏����� & �Ăяo��
//======================================
bool CEffectAccumulation::InitPostEffect(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
    m_fxFilePath = fxFile; // �p�X�ۑ�

    // �e�N�X�`���̍쐬
    device->CreateTexture(
        TEX_SIZE_X, TEX_SIZE_Y, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
        &m_sceneRenderTarget, nullptr
    );
    m_sceneRenderTarget->GetSurfaceLevel(0, &m_sceneSurface);  // �T�[�t�F�X���x���̎擾

    // ���e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(device, "Data/TEXTURE/fireNoice.png", &m_fireTexture);

    LPD3DXBUFFER pErr = nullptr;

#ifdef _DEBUG
    // �f�o�b�N��
    HRESULT hr = D3DXCreateEffectFromFileA(
        device, fxFile, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_fireEffect, &pErr);
#else	
    // �����[�X����fxo��ǂݍ���
    HRESULT hr = D3DXCreateEffectFromFileA(
    	device,
        fxFile, // .fx �ł͂Ȃ� .fxo ���w��
    	nullptr,
    	nullptr,
    	D3DXFX_NOT_CLONEABLE,  // �o�C�i������ǂݍ��ނƂ��̃t���O(�Ӗ��F���̃G�t�F�N�g�͕����s�ł���)
    	nullptr,
    	&m_fireEffect,
    	&pErr);

#endif	
    if (FAILED(hr)){
        if (pErr){
            // fx�t�@�C�����R���p�C���G���[��������G���[�\�����o��
            MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Compile Error", MB_OK);
            pErr->Release();
        }
        return false;
    }

    // �t���X�N���[���l�p�`�̍쐬
    device->CreateVertexBuffer(4 * sizeof(ScreenVertex), 0, FVF_SCREENVERTEX, D3DPOOL_MANAGED, &m_fullscreenVB, nullptr);

    // �e�N�X�`���̃T�C�Y���w�� + �e�N�X�`�����W��
    ScreenVertex* vertices;
    m_fullscreenVB->Lock(0, 0, (void**)&vertices, 0);
    vertices[0] = { { 0.0f,       0.0f,    0.0f, 1.0f }, { 0.0f, 0.0f } };
    vertices[1] = { { TEX_F_SIZE_X, 0.0f ,   0.0f, 1.0f }, { 1.0f, 0.0f } };
    vertices[2] = { { 0.0f,       TEX_F_SIZE_Y,  0.0f, 1.0f }, { 0.0f, 1.0f } };
    vertices[3] = { { TEX_F_SIZE_X, TEX_F_SIZE_Y,  0.0f, 1.0f }, { 1.0f, 1.0f } };
    m_fullscreenVB->Unlock();

    return true;
}
//======================================
// �V�F�[�_�[�Ɋւ���ϐ��̔j��
//======================================
void CEffectAccumulation::ShutdownPostEffect()
{
    // �V�[���̃����_���[�^�[�Q�b�g�̔j��
    if (m_sceneRenderTarget)
    {
        m_sceneRenderTarget->Release();
        m_sceneRenderTarget = nullptr;
    }
    // �e�N�X�`���̔j��
    if (m_fireTexture)
    {
       m_fireTexture->Release();
       m_fireTexture = nullptr;
    }
    // �G�t�F�N�g�̔j��
    if (m_fireEffect) 
    {
        m_fireEffect->Release();
        m_fireEffect = nullptr;
    }
    // �|���S���̃o�b�t�@�̔j��
    if (m_fullscreenVB)
    {
        m_fullscreenVB->Release();
        m_fullscreenVB = nullptr;
    }
    // �T�[�t�F�X�̔j��
    if (m_sceneSurface)
    {
        m_sceneSurface->Release();
        m_sceneSurface = nullptr;
    }

}
//======================================
// �V�F�[�_�[�̍X�V
//======================================
void CEffectAccumulation::UpdatePostEffect(float deltaTime)
{
    m_fireTime += deltaTime;   // �A�j���[�V�����̑��x
    
    
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER)); // �Q�[���̃v���C���[�̃|�C���^���擾

    if (pPlayer == nullptr)
    {// null�`�F�b�N
        return;
    }
    int nMaxAirPowor = pPlayer->GetMaxAirPower();  // �C�̍ő�p���[�擾
    int nAirPowor = pPlayer->GetAirPower();  // �C�̌��݂̃p���[�擾
    
    float rawRatio = static_cast<float>(nAirPowor) / static_cast<float>(nMaxAirPowor);  // ���������߂�
    rawRatio = min(rawRatio, 1.0f);  // �����1.0�ȉ��ɐ���

    // ���`���
    float ratio = START_RATIO + (1.0f - START_RATIO) * rawRatio;

    m_circleSize = MAX_SIZE * ratio;  // �C�𑝂₷

    // �����}�l�[�W���[�ւ̃|�C���^�擾
    CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));

    // �T�����}�l�[�W���[�X�e�[�g�X�^�[�g�̃|�C���^�����擾
    CSummon_Manager_State_Start* pStart = dynamic_cast<CSummon_Manager_State_Start*>(pSummonManager->GetState());

    if (pStart)
    {
        if (pStart->GetPhaseJudgmentFrame())
        {
            if (pStart->GetSuccessPhase())
            {// ����
                // �J�E���g�𑝂₷
                ++pressCount;

                // �����l
                currentWave.time = 0.0f;
                currentWave.duration = 0.5f;

                // �g�̃s�[�N���x���Z�o
                float peakPower = baseValue + powerPerPress * (pressCount - 1);
                peakPower = min(peakPower, maxPower);  // �ő�l��ݒ�

                // �g�̋����̔��f
                currentWave.power = peakPower - baseValue;

                // ���ԃX�P�[�����J�E���g�̉񐔕������A�ő�2.0�ɐ���
                currentWave.timeScale = min(0.4f + pressCount * 0.025f, 2.0f);
                currentWave.active = true;   // �L����

            }
            else
            {
                // ���s
            }
        }
    }


    // �L�������ǂ���
    if (currentWave.active) {
        // �C�̃^�C���ɔ{�������Z
        currentWave.time += 0.01f * currentWave.timeScale;

        float t = currentWave.time / currentWave.duration; // ���������߂� 
        if (t >= 1.0f)
        {
            t = 1.0f;  // ���̈ʒu�ɖ߂�
            currentWave.active = false;
        }

        // ��������
        float strength = GetWaveStrength(t);

        m_totalDeformPower = baseValue + currentWave.power * strength;  // ����𑝂₷

    }
    if (m_circleSize >= VALUE) {
        // �l�����l�ȏ�ɂȂ����狭��𑝂₷
        m_circlePowor = MAX_POWOR;
        m_sharpnessPowor = MAX_POWOR;

        // �F���w��(���F�Ԃ��ۂ�)
        m_colorR = 0.862f;
        m_colorG = 0.078f;
        m_colorB = 0.235f;
    }
    else {// ���l�ȊO��������ʏ�̋���

        // �F���w��(���F���ۂ��F)
        m_colorR = 0.031f;
        m_colorG = 0.968f;
        m_colorB = 0.968f;

        // �ʏ�̋���
        m_circlePowor = NORMAL_POWOR_CIRCLE;
        m_sharpnessPowor = NORMAL_POWOR_SHARPNESS;
    }

    D3DXVECTOR4 color = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);  // �F�̐ݒ�

    // �G�t�F�N�g�p�����[�^�[�̐ݒ�
    m_fireEffect->SetFloat("circlePowor", m_circlePowor);   // �~�̋����imgui�ŕύX��
    m_fireEffect->SetFloat("sharpnessPowor", m_sharpnessPowor);   // ���̋���
    m_fireEffect->SetFloat("totalDeformPower", m_totalDeformPower);  // �S�̂̋���𑝂₷

    m_fireEffect->SetVector("flameColor", &color);  // �e�N�X�`���̐F
    m_fireEffect->SetFloat("size", m_circleSize);  // �|�X�g�G�t�F�N�g�̃T�C�Y(������ς����)

}
//======================================
// �V�F�[�_�[�̕`��
//======================================
void CEffectAccumulation::RenderEffect(LPDIRECT3DDEVICE9 device)
{
    if (!m_enableEffect)
    {// �G�t�F�N�g�̕\�����
        return;  // false�̎��͕`�悵�Ȃ�
    }
    // �o�b�N�o�b�t�@�擾
    LPDIRECT3DSURFACE9 backBuffer = nullptr;
    device->GetRenderTarget(0, &backBuffer);

    device->SetRenderTarget(0, m_sceneSurface);  // �V�[���T�[�t�F�X���Z�b�g

    // ��ʂ̃N���A
    device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

    CObject_2D::Draw();  // 2D�`��

    // �A���t�@�u�����h�ݒ�
    device->SetRenderState(D3DRS_ZENABLE, FALSE); // �[�x�l�ŕ`�揇��ς��Ȃ��悤�ɂ��A���S�ɕ`�悳�ꂽ���Ԃɂ���
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); //  �A���t�@�u�����h�����Z������
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // �`�悷��F�̃u�����h�W�����u�\�[�X�̃A���t�@�l�v�ɐݒ�
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // �u�����h�W�����P�[�A���t�@�l��

    // �e�N�X�`���̃A���t�@���K��
    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); // �����P�̃A���t�@�l�����̂܂܎g���܂�
    device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);   // �e�N�X�`���摜�̃A���t�@�l�������P�Ƃ��܂��B

    // �o�b�N�o�b�t�@�̐ݒ�
    device->SetRenderTarget(0, backBuffer);


    D3DXVECTOR4 color = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);
    // �G�t�F�N�g�p�����[�^�[�̐ݒ�
    m_fireEffect->SetTexture("screenTex", m_sceneRenderTarget);  // �V�[���̃����_���[�^�[�Q�b�g
    m_fireEffect->SetTexture("fireTex", m_fireTexture);   // �e�N�X�`��
    m_fireEffect->SetFloat("time", m_fireTime);    // �A�j���[�V�����^�C��
    m_fireEffect->SetFloat("circlePowor", m_circlePowor);   // �~�̋���
    m_fireEffect->SetFloat("sharpnessPowor", m_sharpnessPowor);  // ���̋���
    m_fireEffect->SetVector("flameColor", &color);   // �e�N�X�`���̐F
    m_fireEffect->SetFloat("size", m_circleSize);    // �G�t�F�N�g�̃T�C�Y
    m_fireEffect->SetFloat("totalDeformPower", m_totalDeformPower);  // �S�̂̋���𑝂₷


    m_fireEffect->SetTechnique("TechMain");  // �e�N�j�b�N�錾
    UINT passCount = 0;
    m_fireEffect->Begin(&passCount, 0);
    for (UINT i = 0; i < passCount; ++i){
        m_fireEffect->BeginPass(i);

        // �|���S���`��
        device->SetFVF(FVF_SCREENVERTEX);
        device->SetStreamSource(0, m_fullscreenVB, 0, sizeof(ScreenVertex));
        device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

        m_fireEffect->EndPass();
    }
    m_fireEffect->End();

    // �o�b�N�o�b�t�@�̃��Z�b�g
    if (backBuffer){
        backBuffer->Release();
        backBuffer = nullptr;
    }

    // �ݒ�����ɖ߂�
    device->SetRenderState(D3DRS_ZENABLE, TRUE);
    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}
//==========================================
// �g�̋����̎擾
//==========================================
float CEffectAccumulation::GetWaveStrength(float t)
{
    if (t < 0.5f) {
        float up = t * 0.00002f;
        return EaseInSmooth(up);
    }
    else {
        float down = (t - 0.5f) * 2.0f;
        return 1.0f - down;
    }
}
//==========================================
// �C�[�W���O�֐�
//==========================================
float CEffectAccumulation::EaseInSmooth(float t)
{
    return t * t * (2.0f - t);
}
