//============================================================================
// 
// ２０２５年５月１５日：召喚グループを [summon_mmanager_state_Choose.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "summon_manager_state_Choose.h" // 元ヘッダ
#include "summon_manager.h"
#include "calculation.h"
#include "player.h"
#include "PlayerState_GuideBeforeThrust.h" // プレイヤー状態：「突き前ガイド」
//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;
using namespace std;

//============================================================================
//
// publicメンバ
//
//============================================================================

//====================================================
// コンストラクタ
//====================================================
CSummon_Manager_State_Choose::CSummon_Manager_State_Choose(bool bGuide) :
	m_nChooseGroup{0},
	m_bGuide{ bGuide },
	CSummon_Manager_State()
{
	// 次のウインドウの位置をサイズを決める
	ImGui::SetNextWindowSize(ImVec2(350, 150));
	ImGui::SetNextWindowPos(ImVec2(0, 150));
}

//====================================================
// デストラクタ
//====================================================
CSummon_Manager_State_Choose::~CSummon_Manager_State_Choose()
{
	// 何もなし
}

//====================================================
// 更新処理
//====================================================
void CSummon_Manager_State_Choose::Update(CSummon_Manager* pManager)
{
#ifdef _DEBUG
	// ガイドなしの時は召喚グループを選べるようにする
	if (!m_bGuide)
	{
		bool bSummon = false; // 召喚するかどうか
		bool bChoose = false; // 選択したかどうか
		ImGui::Begin(u8"召喚グループ番号を選択する");

		// ボタンで召喚したいグループを選びます
		if (ImGui::Button("-"))
		{
			m_nChooseGroup--;
			bChoose = true;
		}
		// ボタンを横並びに
		ImGui::SameLine();
		if (ImGui::Button("+"))
		{
			m_nChooseGroup++;
			bChoose = true;
		}

		// 召喚グループ選択時に配列の範囲を超えたら値を調整する
		if (bChoose)
		{
			m_nChooseGroup = Calculation::Value::LimitRange(m_nChooseGroup, 0, static_cast<int>(CSummon_Manager::SUMMONGROUP::MAX) - 1);
		}

		// 選択グループを表示する
		ImGui::Text(u8"選択グループ番号：%d", m_nChooseGroup);

		// チェックボックスでフラグのオンオフ
		ImGui::Checkbox(u8"チェックボックスを押した瞬間に選択グループが召喚されます", &bSummon);

		// 召喚したかどうか表示
		if (bSummon)
		{
			ImGui::Text(u8"選択グループを召喚！");
			SetSummonGroup(m_nChooseGroup);
		}
		else
		{
			ImGui::Text(u8"選択グループを召喚しない");
		}

		ImGui::End();
	}
#endif // _DEBUG

	// 基底クラス更新
	CSummon_Manager_State::Update(pManager);
}

//====================================================
// ガイドするかどうか取得
//====================================================
const bool& CSummon_Manager_State_Choose::GetGuide() const
{
	return m_bGuide;
}

//====================================================
// ガイドするかどうか設定
//====================================================
void CSummon_Manager_State_Choose::SetGuide(bool bGuide)
{
	m_bGuide = bGuide;
}

//====================================================
// 見切りフレーム数を設定する
//====================================================
void CSummon_Manager_State_Choose::SetAnticipationFrame(const JSON& SummonPattern)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER)); // プレイヤーへのポインタ
	int nAnticipationFrame = 120; // 見切りフレーム数

    // 「見切りフレーム数」のキーが存在するなら見切りフレーム数を設定する
	if (SummonPattern.contains("AnticipationFrame"))
	{
		nAnticipationFrame = SummonPattern["AnticipationFrame"];

	}

	// プレイヤーをガイド突き前状態にする
	if (m_bGuide)
	{
		pPlayer->SetState(DBG_NEW CPlayerState_GuideBeforeThrust(pPlayer, nAnticipationFrame));
	}
	// プレイヤーを突き前状態にする
	else
	{
		pPlayer->SetState(DBG_NEW CPlayerState_BeforeThrust(pPlayer, nAnticipationFrame));
	}
}
