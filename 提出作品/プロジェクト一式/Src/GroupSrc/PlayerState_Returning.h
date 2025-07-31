//============================================================================
// 
// ２０２５年７月１０日：召喚関係のバグの改善とプレイヤー条件分岐をステートパターンにする [PlayerState_Returning.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _PLAYERSTATE_RETURNING_H_
#define _PLAYERSTATE_RETURNING_H_	

//****************************************************
// インクルードファイル
//****************************************************
#include "PlayerState.h"

//****************************************************
// プレイヤー状態クラス
//****************************************************
class CPlayerState_Returning : public CPlayerState
{
public:
	// === 特殊関数 ===

	CPlayerState_Returning(CPlayer* pPlayer); // コンストラクタ
	~CPlayerState_Returning() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	void Init(CPlayer* pPlayer) override;   // 初期化処理
	void Uninit(CPlayer* pPlayer) override; // 終了処理
	void Update(CPlayer* pPlayer) override; // 更新処理

	// === メンバ関数 ===

	// 元の位置へ戻るのが完了したかどうか
	const bool& GetIsReturnComplete() const; // 取得

private:
	// === 静的メンバ変数 ===
	static constexpr float
		s_fCameraZoom_PosAdjust = 0.15f, // カメラのズームを開始する時の位置補正値
		s_fCameraZoom_DistanceAdjust = 0.05f, // カメラのズームを開始する時の距離補正値
		s_fPlayerMoveAdjust = 0.18f, // プレイヤー移動量補正値
		s_fPlayerGoalToleranceDistance = 0.1f; // プレイヤーが目的地に到達したとみなす許容距離

	// === メンバ変数 ===
	D3DXVECTOR3 m_GoalPos; // 目的地
	bool m_bIsReturnComplete; // 元の位置に戻るのが完了したかどうか

};
#endif // _COLLISION_H_