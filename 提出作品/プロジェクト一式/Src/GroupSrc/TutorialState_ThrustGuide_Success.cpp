//============================================================================
// 
// ２０２５年７月２２日：チュートリアルを実装する [TutorialState_ThrustGuide_Success.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルード
//****************************************************
#include "TutorialState_ThrustGuide_Success.h"
#include "PlayerState_GuideBeforeThrust.h"
#include "summon_manager_state_choose.h"
#include "TutorialState_ThrustPractice.h"
#include "tutorial.h"
#include "Pch.h"
#include "calculation.h"
#include "UiText.h"
#include "manager.h"
#include "player.h"
#include <string>
#include <locale>
#include <codecvt>
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

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CTutorialState_ThrustGuide_Success::CTutorialState_ThrustGuide_Success(CTutorial* pTutorial) :
	m_nChooseExplanation{ 0 },
	m_nCntFrame{0},
	CTutorialState{pTutorial}
{
	std::fstream ReadingFile; // ファイル読み込み用

	// 召喚マネージャーへのポインタ取得
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));

	// === 説明ジェイソンファイル読み込み ===

	ReadingFile.open("Data\\JSON\\Tutorial\\ThrustGuide_Success.Json", std::ios::in); // 突きガイド成功のジェイソンファイルを開く

	// ファイルが正常に開けていたら召喚パターンリストを格納
	if (ReadingFile.good())
	{
		ReadingFile >> m_JsonExplanation; // ジェイソン格納

		// 配列じゃなかったら処理を終了
		if (!m_JsonExplanation.is_array())
		{
			assert(false && "召喚パターンリストが配列型じゃありません");
		}
	}

	ReadingFile.close(); // ファイルを閉じる

}

//============================================================================
// デストラクタ
//============================================================================
CTutorialState_ThrustGuide_Success::~CTutorialState_ThrustGuide_Success()
{
	// 何もなし
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CTutorialState_ThrustGuide_Success::Init(CTutorial* pTutorial)
{
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTutorialState_ThrustGuide_Success::Uninit(CTutorial* pTutorial)
{
	// 何もなし
}

//============================================================================
// 更新処理
//============================================================================
void CTutorialState_ThrustGuide_Success::Update(CTutorial* pTutorial)
{
	// UI用テキストを生成し追加する
	if (m_JsonExplanation[m_nChooseExplanation].contains("Explanation"))
	{
		if (m_JsonExplanation[m_nChooseExplanation]["Explanation"].is_string())
		{
			string Str = m_JsonExplanation[m_nChooseExplanation]["Explanation"];
			CRenderer::GetRenderer()->GetUiTextManager().Add(
				DBG_NEW CUIText(Calculation::Convert::Utf8ToSjis(Str), 640, 600, 40,1, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
					D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f)));
		}
	}

	// 一定フレームごとに説明を更新する
	if (m_JsonExplanation[m_nChooseExplanation].contains("DispFrame"))
	{
		if (m_nCntFrame == m_JsonExplanation[m_nChooseExplanation]["DispFrame"] && m_nCntFrame > 0)
		{
			// trueならシーンが変わるので処理を中断する
			if (ChangeStrExplanation(pTutorial))
			{
				return;
			}
			m_nCntFrame = 0;
		}
	}
	m_nCntFrame++;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 説明文字列を変更する
//============================================================================
bool CTutorialState_ThrustGuide_Success::ChangeStrExplanation(CTutorial* pTutorial)
{
	m_nChooseExplanation++; // 説明文字列番号をインクリメントする

	if (m_nChooseExplanation == 1)
	{
		CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));
		CSummon_Manager_State_Choose* pChoose = dynamic_cast<CSummon_Manager_State_Choose*>(pSummonManager->GetState());
		pChoose->SetSummonGroup(1);
	}

	// 最大超えたら最小に戻す
	if (m_nChooseExplanation >= static_cast<int>(EXPLANATION::MAX))
	{
		m_nChooseExplanation = 0; // 説明選択番号を０に戻す
		//pTutorial->To_Next(); // シーンを次へ
		pTutorial->SetState(DBG_NEW CTutorialState_ThrustPractice(pTutorial));
		return true;
	}

	return false;
}