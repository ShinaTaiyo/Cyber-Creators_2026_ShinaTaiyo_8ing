//============================================================================
// 
// ２０２５年７月２２日：チュートリアルを実装する [TutorialState_HandSwordDemo.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルード
//****************************************************
#include "TutorialState_HandSwordDemo.h"
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
const string CTutorialState_HandSwordDemo::s_StrExplanation[static_cast<int>(CTutorialState_HandSwordDemo::EXPLANATION::MAX)] =
{
	"このゲームの目的は",
	"説明していくので一緒に慣れていきましょう"
};

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CTutorialState_HandSwordDemo::CTutorialState_HandSwordDemo(CTutorial* pTutorial) :
	CTutorialState(pTutorial)
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CTutorialState_HandSwordDemo::~CTutorialState_HandSwordDemo()
{
	// 何もなし
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CTutorialState_HandSwordDemo::Init(CTutorial* pTutorial)
{
	// 何もなし
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTutorialState_HandSwordDemo::Uninit(CTutorial* pTutorial)
{
	// 何もなし
}

//============================================================================
// 更新処理
//============================================================================
void CTutorialState_HandSwordDemo::Update(CTutorial* pTutorial)
{
	if (CManager::GetKeyboard()->GetTrigger(DIK_M) ||
		CManager::GetPad()->GetTrigger(CInputPad::JOYKEY::A))
	{
		ChangeStrExplanation(pTutorial);
	}
	m_nCntFrame++; // フレーム数カウントアップ
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 説明文字列を変更する
//============================================================================
void CTutorialState_HandSwordDemo::ChangeStrExplanation(CTutorial* pTutorial)
{
	m_nChooseExplanation++; // 説明文字列番号をインクリメントする

	//// 最大超えたら最小に戻す
	//if (m_nChooseExplanation >= static_cast<int>(EXPLANATION::MAX))
	//{
	//	pTutorial->SetState(DBG_NEW CTutorialState_HandSwordDemo)
	//	m_nChooseExplanation = 0; // 説明選択番号を０に戻す
	//}
}
