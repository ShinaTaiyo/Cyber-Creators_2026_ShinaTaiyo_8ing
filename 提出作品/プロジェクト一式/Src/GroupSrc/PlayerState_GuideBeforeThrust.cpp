//============================================================================
// 
// ２０２５年７月２３日：チュートリアルを実装する [PlayerState_GuideBeforeThrust.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルード
//****************************************************

// === オブジェクト ===

#include "player.h" // プレイヤー
#include "target.h" 
#include "obstacle.h" // 障害物
#include "HoleObstacle.h" // 穴あき障害物
#include "zone.h" // ゾーン
#include "summon_manager.h" // 召喚マネージャー
#include "object.h"
#include "SummonGroupSelector.h" // 召喚グループ選択
#include "particle.h" // ターゲット位置確認用

// === 状態 ===
#include "PlayerState_GuideBeforeThrust.h" // 突きの前状態
#include "PlayerState_Thrusting.h" // 突き
#include "PlayerState_Returning.h" // 戻る
#include "summon_manager_state.h" // 召喚マネージャー状態
#include "summon_manager_state_start.h" // 召喚マネージャー召喚開始状態
#include "camera_state_targetzoom.h" // カメラズーム状態
#include "camera_state_normal.h" // カメラ通常状態
#include "summon_manager_state_choose.h" // 召喚マネージャー召喚選択状態

// === 情報 ===

#include "transparent_info.h" // 透明度情報
#include "game.h" // ゲームシーン
#include "manager.h" // マネージャー
#include "input.h" // 入力
#include "renderer.h" // 描画
#include "logger.h" // ログ
#include "camera.h" // カメラ
#include "move_info.h" // 移動情報
#include "calculation.h" // 計算

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

//============================================================================
// コンストラクタ
//============================================================================
CPlayerState_GuideBeforeThrust::CPlayerState_GuideBeforeThrust(CPlayer* pPlayer, int nAnticipationFrame) :
	m_nNowMotion{0},
	m_nMotionFrame{0},
	m_GoalPos{VEC3_INIT},
	m_Vec3LerpInfo{},
	m_bNowEndMotionAfterThrust{false},
	CPlayerState_BeforeThrust(pPlayer,nAnticipationFrame)
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CPlayerState_GuideBeforeThrust::~CPlayerState_GuideBeforeThrust()
{
	// 何もなし
}

//============================================================================
// 初期化処理
//============================================================================
void CPlayerState_GuideBeforeThrust::Init(CPlayer* pPlayer)
{
	CTarget* pTarget = pPlayer->GetTarget();
	pTarget->SetUseControl(false);
	pTarget->SetUseMove(true);

	std::fstream ReadingFile; // ファイル読み込み用

	// === モーションジェイソンファイル読み込み ===

	ReadingFile.open("Data\\JSON\\PlayerState_GuideBeforeThrust\\GuideMotion.Json", std::ios::in);

	// ファイルが正常に開けていたら召喚パターンリストを格納
	if (ReadingFile.good())
	{
		ReadingFile >> m_JsonMotion; // ジェイソン格納

		// 配列じゃなかったら処理を終了
		if (!m_JsonMotion.is_array())
		{
			assert(false && "召喚パターンリストが配列型じゃありません");
		}
	}

	ReadingFile.close(); // ファイルを閉じる

	if (!m_JsonMotion[m_nNowMotion].contains("LerpFrame"))
	{
		assert(false && "プレイヤーのガイド突き前状態のモーションの補間フレーム数のキーが存在しない");
	}

	int nLerpFrame = m_JsonMotion[m_nNowMotion]["LerpFrame"];
	vector<float> vGoalPos = m_JsonMotion[m_nNowMotion]["GoalPos"];
	m_Vec3LerpInfo.SetInitialize(nLerpFrame, pPlayer->GetTarget()->GetPos(),
		D3DXVECTOR3(vGoalPos[0], vGoalPos[1], vGoalPos[2]));
}

//============================================================================
// 終了処理
//============================================================================
void CPlayerState_GuideBeforeThrust::Uninit(CPlayer* pPlayer)
{
	// 何もなし
}

//============================================================================
// 更新処理
//============================================================================
void CPlayerState_GuideBeforeThrust::Update(CPlayer* pPlayer)
{
	// 線形補間処理
	LerpProcess(pPlayer);

	// モーションを次に進める（状態が変わったらtrueが返され、処理を終了する）
	if (NextMotion(pPlayer))
	{
		return;
	}

	// 基底クラス更新
	CPlayerState_BeforeThrust::Update(pPlayer);

	// モーションフレームをカウントアップ
	m_nMotionFrame++;
}

//============================================================================
// 線形補間処理
//============================================================================
void CPlayerState_GuideBeforeThrust::LerpProcess(CPlayer* pPlayer)
{
	CTarget* pTarget = pPlayer->GetTarget();
	D3DXVECTOR3 TargetPos = pTarget->GetPos();
	m_Vec3LerpInfo.Update(TargetPos, pTarget);
	pTarget->SetPos(TargetPos);

	CRenderer::SetDebugString("目標位置：X = " + to_string(m_GoalPos.x) +
		"Y = " + to_string(m_GoalPos.y) + "Z = " + to_string(m_GoalPos.z));
}

//============================================================================
// 目的地を決める操作
//============================================================================
bool CPlayerState_GuideBeforeThrust::ControlDecisionGoal()
{
	return false;
}

//============================================================================
// 手刀処理
//============================================================================
bool CPlayerState_GuideBeforeThrust::HandSword(CPlayer* pPlayer)
{
	// この状態では発動しない
	return false;
}

//============================================================================
// モーションを次に進める
//============================================================================
bool CPlayerState_GuideBeforeThrust::NextMotion(CPlayer* pPlayer)
{
	// 今回使うキーがどれか一つでもなかったら例外処理
	if (!m_JsonMotion[m_nNowMotion].contains("LerpFrame") ||
		!m_JsonMotion[m_nNowMotion].contains("GoalPos") ||
		!m_JsonMotion[m_nNowMotion].contains("ExecuteThrust"))
	{
		assert(false && "プレイヤーのガイド突き前状態のモーションの使用するキーで存在しないものがある！");
	}

	int nLerpFrame = m_JsonMotion[m_nNowMotion]["LerpFrame"];

	// 線形補間フレーム数に達したらモーションを次に進める
	if (m_nMotionFrame >= nLerpFrame)
	{
		// 今回のモーションが終わったら突きを発動するならやる
		if (m_bNowEndMotionAfterThrust)
		{
			DecisionGoal(pPlayer);
			return true;
		}

		m_nNowMotion++;
		int nSize = m_JsonMotion.size();
		if (m_nNowMotion == nSize)
		{
			m_nNowMotion = 0;
		}
		CLogger::GetInstance()->AddDuration(u8"読み込んだモーション：" + to_string(m_nNowMotion),5);
		m_nMotionFrame = 0; // モーションフレームリセット

		// 線形補間フレーム数
		nLerpFrame = m_JsonMotion[m_nNowMotion]["LerpFrame"];

		// 現在のモーションが終わったら突きを発動するかどうか
		m_bNowEndMotionAfterThrust = m_JsonMotion[m_nNowMotion]["ExecuteThrust"];

		// 目的地
		vector<float> vGoalPos = m_JsonMotion[m_nNowMotion]["GoalPos"];
		m_GoalPos = D3DXVECTOR3(vGoalPos[0], vGoalPos[1], vGoalPos[2]);

		// 読み込んだ情報を使って線形補間情報を変更
		m_Vec3LerpInfo.SetInitialize(nLerpFrame,pPlayer->GetTarget()->GetPos(),m_GoalPos);
	}

	return false;
}
