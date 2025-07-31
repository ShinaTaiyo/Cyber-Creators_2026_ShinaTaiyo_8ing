//============================================================================
// 
// ２０２５年５月２０日：カメラ状態クラスを作る [camera_state_targetzoom.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _CAMERA_STATE_TARGETZOOM_H_
#define _CAMERA_STATE_TARGETZOOM_H_	// 二重インクルード防止

//****************************************************
// インクルード
//****************************************************
#include "camera_state.h" // カメラ状態

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// カメラステートクラス
//****************************************************
class CCamera_State_TargetZoom : public CCamera_State
{
public:
	// === 特殊関数 ===

	// コンストラクタ
	CCamera_State_TargetZoom(D3DXVECTOR3 GoalPos, // 目的地
		                     float fGoalDistance, // 目的の距離
		                     float fPosAdjust, // 位置補正値
		                     float fDistanceAdjust, // 距離補正値
		                     bool bAfterTurn,     // 処理が終わった後に元の位置に戻すかどうか 
		                     CCamera * pCamera);  // カメラへのポインタ
	~CCamera_State_TargetZoom(); // デストラクタ

	// === ライフサイクルメンバ関数 ===

	void Update(CCamera* pCamera) override; // 更新処理

	// === メンバ関数 ===

private:
	// === 静的変数 ===

	static constexpr float s_fPerDistance = 0.7f; // 許容する距離

	// === 変数 ===

	D3DXVECTOR3 m_GoalPos;  // 目標の向き
	float 
		m_fGoalDistance,  // 目標の位置との距離
		m_fPosAdjust,     // 位置補正値
		m_fRotAdjust,     // 向き補正値
	    m_fDistanceAdjust; // 距離補正値
	bool m_bAfterTurn;      // 処理が終わった後に元の位置に戻すかどうか 

};
#endif // _Summon_MANAGER_H_