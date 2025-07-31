//============================================================================
// 
// ２０２５年７月１０日：召喚関係のバグの改善とプレイヤー条件分岐をステートパターンにする [PlayerState_Returning.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルード
//****************************************************
#include "PlayerState_Returning.h"
#include "camera.h"
#include "GamePlayer.h"
#include "calculation.h"
#include "camera_manager.h"
#include "camera_state_targetzoom.h"
#include "target.h"
#include "game.h"
#include "summon_manager_state_start.h"
#include "transparent_info.h"
#include "PlayerState_BeforeThrust.h"
#include "renderer.h" // 描画
#include "logger.h" // ログ

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
CPlayerState_Returning::CPlayerState_Returning(CPlayer* pPlayer) :
	CPlayerState(pPlayer),
	m_GoalPos{VEC3_INIT},
	m_bIsReturnComplete{false}
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CPlayerState_Returning::~CPlayerState_Returning()
{
	// 何もなし
}

//============================================================================
// 初期化処理
//============================================================================
void CPlayerState_Returning::Init(CPlayer* pPlayer)
{
	CCamera* pCamera = &CCamera_Manager::GetInstance()->RefSelectCamera();
	pCamera->ChangeState(DBG_NEW CCamera_State_TargetZoom(VEC3_INIT, CCamera::GetInitDistance(), 0.2f, 0.2f, false, pCamera));
	CTarget* pTarget = pPlayer->GetTarget(); // ターゲットへのポインタ
	pTarget->SetPosTarget(VEC3_INIT); // 中心に戻す
	pTarget->SetUseMove(true); // 移動出来るようにする
	pTarget->SetUseControl(false); // まだ操作させない

	// ターゲットの透明度を６０フレームで元に戻す
	CTransparent_Info* pTargetTransparentInfo = pTarget->GetTransparent_Info();
	pTargetTransparentInfo->SetSpeed(pTargetTransparentInfo->GetMaxValue() / 60);

	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	// 召喚マネージャーのオブジェクトはこの状態になった瞬間に消す
	while (pObj)
	{
		CObject* pNext = pObj->GetNext();

		if (pObj->GetType() == CObject::TYPE::OBSTACLE ||
			pObj->GetType() == CObject::TYPE::HOLEOBSTACLE ||
			pObj->GetType() == CObject::TYPE::ZONE)
		{
			pObj->SetRelease();
		}

		pObj = pNext;
	}

	// プレイヤー状態をログで表示
	CLogger::GetInstance()->AddDuration(u8"プレイヤーが「戻る状態」になりました！", 1);
}

//============================================================================
// 終了処理
//============================================================================
void CPlayerState_Returning::Uninit(CPlayer* pPlayer)
{
	// 何もなし
}

//============================================================================
// 更新処理
//============================================================================
void CPlayerState_Returning::Update(CPlayer* pPlayer)
{
	// 目的地を決定
	CCamera* pCamera = &CCamera_Manager::GetInstance()->RefSelectCamera(); // 現在のカメラへのポインタ
	Calculation::Pos::ScreenToWorld(&m_GoalPos, FSCREEN_WIDTH + 100.0f, FSCREEN_HEIGHT - 100.0f, 0.75f, 1280, 720,
		pCamera->GetMtxView(), pCamera->GetMtxProj());

	const D3DXVECTOR3& SelfPos = pPlayer->GetPos(); // 自身の位置
	D3DXVECTOR3 VecSelfToGoal = m_GoalPos - SelfPos; // 自身から目的地へのベクトル

	// 指数減衰で目的地へ移動する
	pPlayer->SetPos(SelfPos + VecSelfToGoal * s_fPlayerMoveAdjust);

	VecSelfToGoal = m_GoalPos - SelfPos; // 自身から目的地へのベクトル更新

	float fVecLength = D3DXVec3Length(&VecSelfToGoal); // 自身から目的地へのベクトルの長さ

	// 目的地への距離が許容距離よりも近くなったら元の位置へ戻る完了フラグをtrueにする
	if (fVecLength < s_fPlayerGoalToleranceDistance)
	{
		m_bIsReturnComplete = true;
	}

	// デバッグ表示
	CRenderer::SetDebugString("プレイヤーの状態：戻る");
}

//============================================================================
// 元の位置へ戻るのが完了したかどうか取得
//============================================================================
const bool& CPlayerState_Returning::GetIsReturnComplete() const
{
	return m_bIsReturnComplete;
}
