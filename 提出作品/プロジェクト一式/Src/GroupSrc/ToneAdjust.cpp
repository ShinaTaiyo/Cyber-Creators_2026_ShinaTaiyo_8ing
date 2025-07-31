//==============================================
// 
// �|�X�g�G�t�F�N�g[�F���ω�] <ToneAdjust.cpp>
// Auther �ؓc�А�
// 
//==============================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************

#include "ToneAdjust.h"   // ���w�b�_�[���g��
#include "manager.h"    // �}�l�[�W���[�̎g�p
#include "renderer.h"  // �f�o�C�X�̎g�p
#include "game.h"    // �Q�[�����g�p
#include "GamePlayer.h"   // �Q�[���v���C���[���g�p
#include "SummonGroupSelector.h"    // �T�����O���[�v�}�l�[�W���[�̊֐����g�p
#include "summon_manager_state_start.h"   // �����}�l�[�W���[��ԃN���X���g�p
#include "PlayerState_Thrusting.h"   // �v���C���[����Q���ɓ������������g�p

//====================================
// �ÓI�����o�錾
//====================================
bool CToneAdjust::s_bUseDraw = false; // �`�悷�邩�ǂ���


//================================
// �R���X�g���N�^
//================================
CToneAdjust::CToneAdjust()
{
    m_pEffect = nullptr;

    // �F���̐F
    m_colorR = 1.0f;
    m_colorG = 1.0f;
    m_colorB = 1.0f;

    m_enableEffect = false;  // �G�t�F�N�g�̕\�����
    m_bActive = true;  // ImGui�̐؂�ւ�
    m_bChange = false;
    m_Alpha = 0.0f;  // �����x
    m_nCountFrame = 0;
    m_fadeProgress = 0.0f;  // �t�F�[�h�̐i��
    m_FadeActive = false;  // �t�F�[�h�̐؂�ւ�
    m_FadeDirection = 0.0f;  // �t�F�[�h�̕���

    m_totalTime = 0.0f;  // ���Ԃ�����
    m_bLevelBarEnabled = false; // �_�̋N��
    s_bUseDraw = true; // �`����g�p����
}
//================================
// �f�X�g���N�^
//================================
CToneAdjust::~CToneAdjust()
{
}
//================================
// ����������
//================================
HRESULT CToneAdjust::Init()
{
    // �f�o�C�X�̎擾
    auto pDevice = CRenderer::GetDeviece();

    CObject_2D::Init();

    // �^�C�v�ݒ�
    SetType(CObject::TYPE::TONE);

    // �V�F�[�_�[�t�@�C���̓ǂݍ���
#ifdef _DEBUG
    InitToneAdjust(pDevice, "Src/Shader/fx/ToneAdjust.fx");
#else
    InitToneAdjust(pDevice, "Src/Shader/fxo/ToneAdjust.fxo");
#endif // _DEBUG


    return S_OK;
}
//================================
// �I������
//================================
void CToneAdjust::Uninit()
{
    // �G�t�F�N�g�̔j��
    if (m_pEffect)
    {
        m_pEffect->Release();
        m_pEffect = nullptr;
    }

    CObject_2D::Uninit();
}
//================================
// �X�V����
//================================
void CToneAdjust::Update()
{
    CObject_2D::Update();

    // ImGui�̂��
    ImGui::SetNextWindowSize({ -1, -1 });
    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

    if (ImGui::Begin(u8"�F���ω�"))
    {
        ImGui::Checkbox(u8"Gui���J��", &m_bActive);
        if (!m_bActive) {
            ImGui::Text(u8"�F���̐F����");
            ImGui::SliderFloat("Color R", &m_colorR, 0.0f, 1.0f);
            ImGui::SliderFloat("Color G", &m_colorG, 0.0f, 1.0f);
            ImGui::SliderFloat("Color B", &m_colorB, 0.0f, 1.0f);

            ImGui::Text(u8"�F���̓����x");
            ImGui::SliderFloat(u8"�����x", &m_Alpha, 0.0f, 1.0f);

            ImGui::Checkbox(u8"�G�t�F�N�g�̕\�����", &m_enableEffect);

            ImGui::Text(u8"�t�F�[�h�̐ݒ�");

            ImGui::SliderFloat(u8"�t�F�[�h�̐i��", &m_fadeProgress, 0.0f, 1.0f);
            ImGui::Checkbox(u8"�t�F�[�h�̐؂�ւ�", &m_FadeActive);

        }
        ImGui::End();
    }
    // �t���[���Ԃ̎��Ԏ擾�Ɏg��
    DWORD currentTime = timeGetTime(); // �~���b
    static DWORD prevTime = currentTime;
    float deltaTime = (currentTime - prevTime) / 1000.0f;
    prevTime = currentTime;

    m_totalTime += deltaTime;

    UpdateToneAdjust(m_totalTime);  // �V�F�[�_�[�̍X�V
}
//================================
// �`�揈��
//================================
void CToneAdjust::Draw()
{
    if (s_bUseDraw)
    {
        // �f�o�C�X�̎擾
        auto pDevice = CRenderer::GetDeviece();

        RenderToneAdjust(pDevice);  // �V�F�[�_�[�̕`��
    }
}

//================================
// ��������
//================================
CToneAdjust* CToneAdjust::Create()
{
    CToneAdjust* pToneAdjust = DBG_NEW CToneAdjust();
    pToneAdjust->Init();

    return pToneAdjust;
}
//====================================
// �`�悷�邩�ǂ����ݒ�
//====================================
void CToneAdjust::SetUseDraw(bool bUse)
{
    s_bUseDraw = bUse;
}
//================================
// �V�F�[�_�[�̏�����
//================================
bool CToneAdjust::InitToneAdjust(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
    m_fxFilePath = fxFile; // �p�X�ۑ�
    LPD3DXBUFFER pErr = nullptr;

    // �G�t�F�N�g�ǂݍ���
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
    if (FAILED(hr))
    {// �V�F�[�_�[���ǂݍ��܂�Ȃ�������
        if (pErr)
        {// �G���[�\�����o��
            MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Compile Error", MB_OK);
            pErr->Release();
            pErr = nullptr;
        }
        return false;
    }

    return true;
}

//================================
// �V�F�[�_�[�̔j��
//================================
void CToneAdjust::UninitToneAdjust()
{
    // ���ɂȂ�
}
//=====================================
// �V�F�[�_�[�̍X�V
//=====================================
void CToneAdjust::UpdateToneAdjust(float deltaTime)
{
    // �F�̐ݒ�
    D3DXVECTOR4 baseColor = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);
    D3DXVECTOR4 fadeColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

    // �����}�l�[�W���ւ̃|�C���^���擾
    CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));

    // �T�����}�l�[�W���[�X�e�[�g�X�^�[�g�̃|�C���^�����擾
    CSummon_Manager_State_Start* pStart = dynamic_cast<CSummon_Manager_State_Start*>(pSummonManager->GetState());

    // �v���C���[�ւ̃|�C���^���擾
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER));
    // �v���C���[�˂���Ԃւ̃|�C���^���擾
    CPlayerState_Thrusting* pThrusting = dynamic_cast<CPlayerState_Thrusting*>(pPlayer->GetState());

    if (pStart)
    {
        if (pStart->GetStartPhase())
        {// ���݂��t�F�[�Y����������
            m_enableEffect = true;  // �F���N��

            // �˂���Ԃœ����������A�����������̂���Q���Ȃ烍�O���o��
            if (pThrusting)
            {
                if (pThrusting->GetNowCollision() && pThrusting->GetCollisionObstacle())
                {// ��Q���ɓ���������

                    m_nCountFrame = 30;  // �����t���[�����Z�b�g
                    m_bChange = true;   // �F��ς��邽�߂�bool
                }
                else
                {
                }
            }

            float gradProgress = 0.0f;

            // ��Q���ɓ���������(���s��)
            if (m_bChange) {  // true�̎�
                --m_nCountFrame;  // �J�E���g�����炷
                m_FadeActive = true;
                m_bLevelBarEnabled = true;

                // �t�F�[�h���A�N�e�B�u��Ԃ�������
                if (m_FadeActive) {
                    m_FadeDirection = 1.0f;  // �t�F�[�h�C�����
                    // �t�F�[�h�C������
                    if (m_fadeProgress < 1.0f) {
                        m_fadeProgress += deltaTime * FADE_SPEED; // 2.0�{�̃X�s�[�h�ŃC��
                        m_fadeProgress = min(m_fadeProgress, 1.0f);
                    }
                }
                // �F��ς���
                m_colorR = 0.3f;
                m_colorG = 0.0f;
                m_colorB = 0.5f;
                m_Alpha = 0.8f;

                // �J�E���g���Ȃ��Ȃ�����
                if (m_nCountFrame <= 0) {
                    m_bChange = false;  // ��ԕω������ɖ߂�

                    m_nCountFrame = 0;  // �J�E���g���߂�
                }
            }
            else {  // false�̎�
                m_FadeActive = false;

                // �G�t�F�N�g��off�̎�
                if (!m_FadeActive) {
                    m_FadeDirection = -1.0f;  // �t�F�[�h�A�E�g���
                    // �t�F�[�h�A�E�g����
                    if (m_fadeProgress > 0.0f) {
                        m_fadeProgress -= deltaTime * FADE_SPEED_DOWN; // 2.0�{�̃X�s�[�h�ŃA�E�g

                        m_fadeProgress = max(m_fadeProgress, 0.0f);
                    }
                }
                // �F�Ɠ����x�����ɖ߂�
                m_colorR = 1.0f;
                m_colorG = 1.0f;
                m_colorB = 1.0f;
                m_bLevelBarEnabled = false;
            }
        }
        else{
            m_colorR = 1.0f;
            m_colorG = 1.0f;
            m_colorB = 1.0f;

            m_Alpha = 0.0f;
            m_enableEffect = false;  // �F�����~�߂�
        }
    }

    // �p�����[�^�[��ݒ�
    m_pEffect->SetVector("g_BaseColor", &baseColor);  // �x�[�X�̐F
    m_pEffect->SetFloat("g_FadeProgress", m_fadeProgress);   // �t�F�[�h���N��
    m_pEffect->SetFloat("g_Alpha", m_Alpha); // ������
    m_pEffect->SetBool("g_FadeEnabled", m_FadeActive);
    m_pEffect->SetVector("g_FadeStartColor", &fadeColor);
    m_pEffect->SetFloat("g_Time", deltaTime);
    m_pEffect->SetBool("g_LevelBarEnabled", m_bLevelBarEnabled);

}

//================================
// �V�F�[�_�[�̕`��
//================================
void CToneAdjust::RenderToneAdjust(LPDIRECT3DDEVICE9 device)
{
    //if (!m_enableEffect)
    //{// �G�t�F�N�g�̕\�����
    //    return;  // false�̎��͕`�悵�Ȃ�
    //}

    // --- �u�����h�L���� ---
    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    // --- �F�p�����[�^�ݒ� ---
    D3DXVECTOR4 baseColor = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);
    D3DXVECTOR4 fadeColor = D3DXVECTOR4(0.031f, 0.968f, 0.968f, 1.0f);
    D3DXVECTOR4 tint(1.0f, 1.0f, 1.0f, 1.0f);      // ���ύX
    m_pEffect->SetVector("g_BaseColor", &baseColor);  // �x�[�X�̐F
    m_pEffect->SetFloat("g_FadeProgress", m_fadeProgress);   // �t�F�[�h���N��
    m_pEffect->SetVector("g_Tint", &tint);    // �F��
    m_pEffect->SetFloat("g_Brightness", 0.5f);  // ���邳
    m_pEffect->SetFloat("g_Contrast", 1.0f);  // �R���g���N�^
    m_pEffect->SetFloat("g_Alpha", m_Alpha); // ������
    m_pEffect->SetBool("g_FadeEnabled", m_FadeActive);
    m_pEffect->SetVector("g_FadeStartColor", &fadeColor); // �t�F�[�h�̐F
    m_pEffect->SetBool("g_LevelBarEnabled", m_bLevelBarEnabled);  // �_�̋N��

    m_pEffect->SetTechnique("ToneAdjust");  // �e�N�j�b�N��`
    // --- �`�� ---
    UINT numPasses = 0;
    m_pEffect->Begin(&numPasses, 0);
    for (UINT i = 0; i < numPasses; ++i)
    {
        m_pEffect->BeginPass(i);
        CreateFullScreenQuad(device);   // �S��ʂɃ|���S����`��
        m_pEffect->EndPass();
    }
    m_pEffect->End();

    // �u�����h�����ɖ߂�
    device->SetRenderState(D3DRS_ZENABLE, TRUE);
    device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

//================================
// �S��ʃ|���S���̕`��
//================================
void CToneAdjust::CreateFullScreenQuad(LPDIRECT3DDEVICE9 device)
{
    // �S��ʃN�A�b�h�i���ォ��E���j
    SCREEN_VERTEX quad[] = {
        { 0.0f,     0.0f,     0.0f, 1.0f, 0.0f, 0.0f },
        { 1280.0f,  0.0f,     0.0f, 1.0f, 1.0f, 0.0f },
        { 0.0f,     720.0f,   0.0f, 1.0f, 0.0f, 1.0f },
        { 1280.0f,  720.0f,   0.0f, 1.0f, 1.0f, 1.0f },
    };

    device->SetFVF(FVF_SCREEN_VERTEX);
    device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quad, sizeof(SCREEN_VERTEX));

}

