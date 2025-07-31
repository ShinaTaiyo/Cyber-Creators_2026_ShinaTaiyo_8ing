//============================================================================
// 
// �Q�O�Q�T�N�V���Q�R���FUI�p�e�L�X�g��`�悷�� [UIText.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************

#include "UIText.h"
#include "manager.h"
#include "object_2D.h"
#include "renderer.h"
#include "calculation.h"

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
CUIText::CUIText(std::string Str, int PosX, int PosY, int nFontSize,int nDispFrame, D3DXCOLOR Col, D3DXCOLOR BgColor) :
    m_Str{ Str },
    m_nPosX{ PosX },
    m_nPosY{ PosY },
    m_nFontSize{nFontSize},
    m_nDispFrame{nDispFrame},
    m_pFont{nullptr},
    m_Col{Col},
    m_nTextWidth{0},
    m_pTextBG{DBG_NEW CObject_2D()},
    m_nTextHeight{0}
{
    D3DXCreateFont(
        CRenderer::GetDeviece(),
        m_nFontSize,       // �t�H���g�T�C�Y�i�����j
        0,                 // ���i0 = �����j
        FW_NORMAL,         // ����
        1,                 // �~�b�v���x��
        FALSE,             // �C�^���b�N
        SHIFTJIS_CHARSET,  // �����Z�b�g�i���{��Ȃ�SHIFTJIS_CHARSET�j
        OUT_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        "���C���I",       // �t�H���g��
        &m_pFont
    );

    // === �e�L�X�g�̋�`�T�C�Y�v�Z ===

    RECT rc = { 0, 0, 0, 0 };
    m_pFont->DrawTextA(
        nullptr,
        m_Str.c_str(),
        -1,
        &rc,
        DT_CALCRECT, // ������w�肷��ƁA�e�L�X�g�͕`�悹���Ƀe�L�X�g�̋�`�T�C�Y���擾���� 
        m_Col);

    m_nTextWidth = rc.right - rc.left;
    m_nTextHeight = rc.bottom - rc.top;

    // �e�L�X�g�w�i�T�C�Y�ƈʒu�ݒ�
    m_pTextBG->Init(); // ����������
    m_pTextBG->SetCol(BgColor); // �F������ݒ�
    m_pTextBG->SetSize(D3DXVECTOR3(static_cast<float>(m_nTextWidth / 2), static_cast<float>(m_nTextHeight / 2), 0.0f));
    m_pTextBG->SetPos(D3DXVECTOR3(static_cast<float>(m_nPosX), static_cast<float>(m_nPosY), 0.0f));
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CUIText::~CUIText()
{
    // �t�H���g�̔j��
    if (m_pFont != nullptr)
    {
        m_pFont->Release();
        m_pFont = nullptr;
    }

    // �e�L�X�g�w�i�̎��S�t���O�𔭓�����
    if (m_pTextBG)
    {
        m_pTextBG->SetRelease();
        m_pTextBG = nullptr;
    }
}

//============================================================================
// �X�V����
//============================================================================
void CUIText::Update()
{
    // �\���t���[���������炷
    m_nDispFrame--;
}

//============================================================================
// �`�揈��
//===========================================================================
void CUIText::Draw()
{
    // �e�L�X�g��`�悷��͈͂����߂�
    RECT rect = {
        m_nPosX - m_nTextWidth / 2,
        m_nPosY - m_nTextHeight / 2,
        m_nPosX + m_nTextWidth / 2,
        m_nPosY + m_nTextHeight / 2
    };

    // �e�L�X�g�`��J�n
    m_pFont->DrawTextA(
        nullptr,
        m_Str.c_str(),
        -1,
        &rect, // �`��ʒu
        DT_CENTER | DT_VCENTER, // ������
        m_Col // �F����
    );
}

//============================================================================
// �ʒuX�擾
//===========================================================================
const int& CUIText::GetPosX() const
{
    return m_nPosX;
}

//============================================================================
// �ʒuY�擾
//===========================================================================
const int& CUIText::GetPosY() const
{
    return m_nPosY;
}

//============================================================================
// �\���t���[�����擾
//===========================================================================
const int& CUIText::GetDispFrame() const
{
    return m_nDispFrame;
}

//============================================================================
// �e�L�X�g�̉����擾
//===========================================================================
const int& CUIText::GetTextWidth() const
{
    return m_nTextWidth;
}

//============================================================================
// �e�L�X�g�̍����擾
//===========================================================================
const int& CUIText::GetTextHeight() const
{
    return m_nTextHeight;
}
