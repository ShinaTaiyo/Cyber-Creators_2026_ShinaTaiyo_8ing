//============================================================================
// 
// ２０２５年７月２２日：チュートリアルを実装する [TutorialState_GamePurpose.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルード
//****************************************************
#include "TutorialState_GamePurpose.h"
#include "TutorialState_ThrustGuide_Success.h"
#include "tutorial.h"
#include "Pch.h"
#include "calculation.h"
#include "UiText.h"
#include "manager.h"
#include "renderer.h"
#include "Button.h"
#include "input.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;
using namespace std;

//****************************************************
// 静的メンバ変数
//****************************************************
const string CTutorialState_GamePurpose::s_StrExplanation[static_cast<int>(CTutorialState_GamePurpose::EXPLANATION::MAX)] =
{
	u8"このゲームの目的は、相手の攻撃を見切ることです",
	u8"説明していくので一緒に慣れていきましょう"
};

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CTutorialState_GamePurpose::CTutorialState_GamePurpose(CTutorial* pTutorial) : 
	m_nChooseExplanation{0},
	m_pButton{nullptr},
	CTutorialState(pTutorial)
{
	m_pButton = CButton::Create(D3DXVECTOR3(FSCREEN_WIDTH - 100.0f,FSCREEN_HEIGHT / 2,0.0f));
	m_pButton->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
}

//============================================================================
// デストラクタ
//============================================================================
CTutorialState_GamePurpose::~CTutorialState_GamePurpose()
{
	// ボタンの死亡フラグを設定
	if (m_pButton)
	{
		m_pButton->SetRelease();
		m_pButton = nullptr;
	}
}

//============================================================================
// 更新処理
//============================================================================
void CTutorialState_GamePurpose::Update(CTutorial* pTutorial)
{
	CUIText* pUiText =
		DBG_NEW CUIText(Calculation::Convert::Utf8ToSjis(s_StrExplanation[m_nChooseExplanation]), 640, 600, 40,1, 
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
	
	CRenderer::GetRenderer()->GetUiTextManager().Add(pUiText);

	CRenderer::GetRenderer()->GetUiTextManager().Add(
		DBG_NEW CUIText("読み進める\nキーボード：[M]\nコントローラー：Aボタン", 640, 490, 40,1, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f))
	);

	CRenderer::GetRenderer()->GetUiTextManager().Add(
		DBG_NEW CUIText("チュートリアルを飛ばす\nキーボード：[ENTER]\nコントローラー：STARTボタン", 640, 100, 70,1, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f))
	);

	if (m_pButton)
	{
		// ボタンUI表示位置を説明テキストの横に設定
		m_pButton->SetPos(D3DXVECTOR3(
			static_cast<float>(pUiText->GetPosX()) + static_cast<float>(pUiText->GetTextWidth() / 2) + m_pButton->GetSize().x,
			static_cast<float>(pUiText->GetPosY()),
			0.0f));
	}

	// 説明を進める
	if (CManager::GetKeyboard()->GetTrigger(DIK_M) ||
		CManager::GetPad()->GetTrigger(CInputPad::JOYKEY::A))
	{
		ChangeStrExplanation(pTutorial);
	}
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 初期化処理
//============================================================================
HRESULT CTutorialState_GamePurpose::Init(CTutorial* pTutorial)
{
	// 何もなし
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTutorialState_GamePurpose::Uninit(CTutorial* pTutorial)
{
	// 何もなし
}

//============================================================================
// 説明文字列を変更する
//============================================================================
void CTutorialState_GamePurpose::ChangeStrExplanation(CTutorial* pTutorial)
{
	m_nChooseExplanation++; // 説明文字列番号をインクリメントする

	// 最大超えたら全ての説明を見終わったので、突きガイド成功状態にする
	if (m_nChooseExplanation >= static_cast<int>(EXPLANATION::MAX))
	{
		pTutorial->SetState(DBG_NEW CTutorialState_ThrustGuide_Success(pTutorial));
		m_nChooseExplanation = 0; // 説明選択番号を０に戻す
	}
}
