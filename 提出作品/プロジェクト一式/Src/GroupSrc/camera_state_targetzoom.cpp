//============================================================================
// 
// ２０２５年５月２０日：カメラ状態クラスを作る [Camera_State_TargetZoom_targetzoom.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "camera_state_targetzoom.h" // 元ヘッダ
#include "camera_state_normal.h" // カメラ通常状態
#include "camera.h"    // カメラ  
#include "pch.h"       // 様々なインクルード情報を使う
#include "renderer.h"  // デバッグなど
#include "calculation.h" // 計算
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

//====================================================
// コンストラクタ
//====================================================
CCamera_State_TargetZoom::CCamera_State_TargetZoom(D3DXVECTOR3 GoalPos, // 目的地
	float fGoalDistance,
	float fPosAdjust,
	float fDistanceAdjust,
	bool bAfterTurn,  
	CCamera* pCamera) : 
	m_GoalPos{ GoalPos },
	m_fGoalDistance{fGoalDistance},
	m_fPosAdjust{fPosAdjust},
	m_fDistanceAdjust{fDistanceAdjust},
	m_bAfterTurn{bAfterTurn}
{
	// カメラ状態をログで表示
	CLogger::GetInstance()->AddDuration(u8"カメラが「ズーム状態」になりました！", 1);
}

//====================================================
// デストラクタ
//====================================================
CCamera_State_TargetZoom::~CCamera_State_TargetZoom()
{
	// 何もなし
}

//====================================================
// 更新処理
//====================================================
void CCamera_State_TargetZoom::Update(CCamera* pCamera)
{
	// === 処理に使用する情報を宣言 ===

	const D3DXVECTOR3&
		Pos = pCamera->GetPos(), // 位置
		Rot = pCamera->GetRot(); // 向き

	D3DXVECTOR3
		GoalVec = m_GoalPos - Pos; // 目的地へのベクトル

	const float&
		fDistance = pCamera->GetDistance(), // 視点と注視点の距離
		fDistanceDiff = m_fGoalDistance - fDistance; // 目標の距離との差分

	pCamera->SetDistance(fDistance + fDistanceDiff * m_fDistanceAdjust); // 目的の距離まで縮め続ける

	// 向きを元に戻す
	D3DXVECTOR3
		VecDiffToZero = VEC3_INIT - Rot;
	pCamera->SetRot(D3DXVECTOR3(Rot.x + VecDiffToZero.x * 0.1f, Rot.y + VecDiffToZero.y * 0.1f, 0.0f));
    
	// 徐々に目的地に寄せていきます
	pCamera->SetPos(Pos + GoalVec * m_fPosAdjust); // 位置を徐々に目的地へ寄せる

	// 距離の差分が一定以下になったら目的の距離に設定して通常状態に戻す
	if (fDistance < m_fGoalDistance + s_fPerDistance && 
		fDistance > m_fGoalDistance - s_fPerDistance)
	{
		pCamera->SetDistance(m_fGoalDistance);
		pCamera->ChangeState(DBG_NEW CCamera_State_Normal());
	}

	// デバッグ表示
	CRenderer::SetDebugString("目標地点との距離：" + to_string(fDistance));
	CRenderer::SetDebugString("目標地点へのベクトル：X = " + to_string(GoalVec.x) + "、Y = " + to_string(GoalVec.y) + "、Z = " + to_string(GoalVec.z));
}
