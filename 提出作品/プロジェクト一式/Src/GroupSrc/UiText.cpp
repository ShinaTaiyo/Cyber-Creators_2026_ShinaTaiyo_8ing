//============================================================================
// 
// ２０２５年７月２３日：UI用テキストを描画する [UIText.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************

#include "UIText.h"
#include "manager.h"
#include "object_2D.h"
#include "renderer.h"
#include "calculation.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
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
        m_nFontSize,       // フォントサイズ（高さ）
        0,                 // 幅（0 = 自動）
        FW_NORMAL,         // 太さ
        1,                 // ミップレベル
        FALSE,             // イタリック
        SHIFTJIS_CHARSET,  // 文字セット（日本語ならSHIFTJIS_CHARSET）
        OUT_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        "メイリオ",       // フォント名
        &m_pFont
    );

    // === テキストの矩形サイズ計算 ===

    RECT rc = { 0, 0, 0, 0 };
    m_pFont->DrawTextA(
        nullptr,
        m_Str.c_str(),
        -1,
        &rc,
        DT_CALCRECT, // これを指定すると、テキストは描画せずにテキストの矩形サイズを取得する 
        m_Col);

    m_nTextWidth = rc.right - rc.left;
    m_nTextHeight = rc.bottom - rc.top;

    // テキスト背景サイズと位置設定
    m_pTextBG->Init(); // 初期化処理
    m_pTextBG->SetCol(BgColor); // 色合いを設定
    m_pTextBG->SetSize(D3DXVECTOR3(static_cast<float>(m_nTextWidth / 2), static_cast<float>(m_nTextHeight / 2), 0.0f));
    m_pTextBG->SetPos(D3DXVECTOR3(static_cast<float>(m_nPosX), static_cast<float>(m_nPosY), 0.0f));
}

//============================================================================
// デストラクタ
//============================================================================
CUIText::~CUIText()
{
    // フォントの破棄
    if (m_pFont != nullptr)
    {
        m_pFont->Release();
        m_pFont = nullptr;
    }

    // テキスト背景の死亡フラグを発動する
    if (m_pTextBG)
    {
        m_pTextBG->SetRelease();
        m_pTextBG = nullptr;
    }
}

//============================================================================
// 更新処理
//============================================================================
void CUIText::Update()
{
    // 表示フレーム数を減らす
    m_nDispFrame--;
}

//============================================================================
// 描画処理
//===========================================================================
void CUIText::Draw()
{
    // テキストを描画する範囲を決める
    RECT rect = {
        m_nPosX - m_nTextWidth / 2,
        m_nPosY - m_nTextHeight / 2,
        m_nPosX + m_nTextWidth / 2,
        m_nPosY + m_nTextHeight / 2
    };

    // テキスト描画開始
    m_pFont->DrawTextA(
        nullptr,
        m_Str.c_str(),
        -1,
        &rect, // 描画位置
        DT_CENTER | DT_VCENTER, // 中央寄せ
        m_Col // 色合い
    );
}

//============================================================================
// 位置X取得
//===========================================================================
const int& CUIText::GetPosX() const
{
    return m_nPosX;
}

//============================================================================
// 位置Y取得
//===========================================================================
const int& CUIText::GetPosY() const
{
    return m_nPosY;
}

//============================================================================
// 表示フレーム数取得
//===========================================================================
const int& CUIText::GetDispFrame() const
{
    return m_nDispFrame;
}

//============================================================================
// テキストの横幅取得
//===========================================================================
const int& CUIText::GetTextWidth() const
{
    return m_nTextWidth;
}

//============================================================================
// テキストの高さ取得
//===========================================================================
const int& CUIText::GetTextHeight() const
{
    return m_nTextHeight;
}
