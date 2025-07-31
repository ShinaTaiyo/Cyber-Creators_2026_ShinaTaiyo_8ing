//============================================================================
// 
// ２０２５年７月１０日：召喚関係のバグの改善とプレイヤー条件分岐をステートパターンにする [PlayerState_BeforeThrust.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _PLAYERSTATE_BEFORETHRUST_H_
#define _PLAYERSTATE_BEFORETHRUST_H_	

//****************************************************
// インクルードファイル
//****************************************************
#include "PlayerState.h"

//****************************************************
// プレイヤー状態クラス
//****************************************************
class CPlayerState_BeforeThrust : public CPlayerState
{
public:
	// === 特殊関数 ===

	CPlayerState_BeforeThrust(CPlayer* pPlayer,int nAnticipationFrame); // コンストラクタ
    ~CPlayerState_BeforeThrust() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===
	void Init(CPlayer* pPlayer) override;   // 初期化処理
	void Uninit(CPlayer* pPlayer) override; // 終了処理
	void Update(CPlayer* pPlayer) override; // 更新処理

	// === メンバ関数 ===

	// 見切りフレーム数
	const int& GetAnticipationFrame() const; // 取得
	void SetAnticipationFrame(int nFrame);   // 設定

	// === 静的メンバ関数 ===

	static const int& GetHandSwordCost(); // 気の消費量を取得
	static const int& GetNormalAnticipationFrame(); // 通常見切りフレーム数取得
protected:
	// === メンバ関数 ===

	void DecisionGoal(CPlayer* pPlayer); // 目的地決定処理
private:
	// === 静的メンバ変数 ===
	static constexpr float
		s_fCameraZoom_PosAdjust = 0.25f, // カメラのズームを開始する時の位置補正値
		s_fCameraZoom_DistanceAdjust = 0.12f, // カメラのズームを開始する時の距離補正値
		s_fAddSummonObjectSlowStartPos_Z = 10.0f, // ターゲットよりどれくらい前を基準にスローを開始するか
		s_fSummonObjectSlowRange = 20.0f, // 召喚されたオブジェクトをスロー状態にする範囲
		s_fSummonObjLerpLength = 20.0f, // 召喚されたオブジェクトをリープさせる一定距離
		s_fPlayerMoveAdjust = 0.5f; // プレイヤー移動量補正値
	static constexpr int
		s_nNormalAnticipationFrame = 90, // 通常見切りフレーム数
		s_nHandSwordCost = 5; // 手刀時に消費する気のコスト

	// === メンバ変数 ===

	int m_nAnticipationFrame; // 見切りフレーム数
	bool
		m_bStartAnticipation; // 見切り開始

	// === メンバ関数 ===

	virtual bool HandSword(CPlayer * pPlayer); // 手刀処理
	void FixedPos(CPlayer* pPlayer);   // 位置固定処理
	void JudgeLerpPosToSummonObj(CPlayer* pPlayer); // 召喚オブジェクト線形補間処理をするかどうか判断する
	void SetLerpPosToSummonObj(CPlayer* pPlayer);   // 召喚オブジェクト線形補間処理を設定する
	void ClearLerpPosToSummonObj();       // 召喚オブジェクト線形補間処理を解除する
	virtual bool ControlDecisionGoal();    // 目的地を決める操作
	void StopObstacleAddRot(); // 障害物の向きの加算を止める
	void SummonObjSlow(int nSlowFrame);        // 召喚オブジェクトをスロー状態にする
	void SummonObjClearSlow(); // ステージのオブジェクトのスロー状態を解除
};
#endif // _COLLISION_H_