//============================================================================
// 
// ２０２５年７月１０日：召喚関係のバグの改善とプレイヤー条件分岐をステートパターンにする [PlayerState_Thrusting.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _PLAYERSTATE_THRUSTING_H_
#define _PLAYERSTATE_THRUSTING_H_	

//****************************************************
// インクルードファイル
//****************************************************
#include "PlayerState.h"

//****************************************************
// 静的メンバ宣言
//****************************************************
class CCollisionInfo_BoundingBox; // 箱判定情報
class COBB; // OBB判定情報

//****************************************************
// プレイヤー状態クラス
//****************************************************
class CPlayerState_Thrusting : public CPlayerState
{
public:
	// === 特殊関数 ===

	CPlayerState_Thrusting(CPlayer* pPlayer,D3DXVECTOR3 GoalPos); // コンストラクタ
	~CPlayerState_Thrusting() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	void Init(CPlayer* pPlayer) override;   // 初期化処理
	void Uninit(CPlayer* pPlayer) override; // 終了処理
	void Update(CPlayer* pPlayer) override; // 更新処理

	// === メンバ関数 ===

	// 目的地に到達したかどうか
	const bool& GetGoalPosArrived() const; // 取得

	// 現在が当たったかどうか
	const bool& GetNowCollision() const; // 取得

	// 障害物に当たったかどうか
	const bool& GetCollisionObstacle() const; // 取得

private:
	// === 静的メンバ変数 ===

	static constexpr float
		s_fCameraZoom_PosAdjust = 0.15f,       // カメラのズームを開始する時の位置補正値
		s_fCameraZoom_DistanceAdjust = 0.05f,  // カメラのズームを開始する時の距離補正値
		s_fPlayerMoveAdjust = 0.18f,           // プレイヤー移動量補正値
		s_fZoneParticleSpeed = 5.0f, // ゾーンを通過した時のパーティクル速度
		s_fPlayerGoalToleranceDistance = 0.5f; // プレイヤーが目的地に到達したとみなす許容距離
	static constexpr int 
	    s_nHitSafeZoneToAddPower = 5,  // セーフゾーン通過時の気を増やす量
		s_nZoneParticleSummonNum = 50; // ゾーン通過時のパーティクル召喚量

	// === メンバ変数 ===

	D3DXVECTOR3 m_GoalPos;  // 目標位置
	bool m_bGoalPosArrived; // 目標位置に到達したかどうか
	bool m_bAvoidObstacle;  // 障害物を回避するかどうか
	bool m_bNowCollision; // 現在当たったかどうか
	bool m_bCollisionObstacle; // 障害物に当たったかどうか

	CCollisionInfo_BoundingBox* m_pCollisionInfo_Box; // 箱判定情報
	COBB* m_pOBB; // OBB情報

	// === メンバ関数 ===
	void Collision(CPlayer * pPlayer); // 当たり判定
	void CollisionObstacle(CPlayer* pPlayer,CObject* pOther);      // 障害物との当たり判定
	void CollisionZone(CPlayer* pPlayer, CObject* pOther);         // ゾーンとの当たり判定 
	void CollisionHole(CPlayer* pPlayer);         // 穴との当たり判定
	void CollisionHoleObstacle(CPlayer* pPlayer, CObject* pOther); // 穴あき障害物との当たり判定
	void UpdateBoxInfo(CPlayer* pPlayer); // ボックス判定情報更新
	void SpreadObstacle(CPlayer * pPlayer);	// 障害物を吹き飛ばす

};
#endif // _COLLISION_H_
