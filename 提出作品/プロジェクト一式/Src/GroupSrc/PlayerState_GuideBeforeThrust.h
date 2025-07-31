//============================================================================
// 
// ２０２５年７月１０日：召喚関係のバグの改善とプレイヤー条件分岐をステートパターンにする [PlayerState_GuideBeforeThrust.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _PLAYERSTATE_GUIDEBEFORETHRUST_H_
#define _PLAYERSTATE_GUIDEBEFORETHRUST_H_	

//****************************************************
// インクルードファイル
//****************************************************
#include "PlayerState_BeforeThrust.h"
#include "Vec3LerpInfo.h"

//****************************************************
// プレイヤー突き前ガイドクラス
//****************************************************
class CPlayerState_GuideBeforeThrust : public CPlayerState_BeforeThrust
{
public:
	// === 特殊関数 ===

	CPlayerState_GuideBeforeThrust(
		CPlayer* pPlayer,       // プレイヤーへのポインタ
		int nAnticipationFrame // 見切りフレーム数
	); // コンストラクタ
	~CPlayerState_GuideBeforeThrust() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	void Init(CPlayer* pPlayer) override;   // 初期化処理
	void Uninit(CPlayer* pPlayer) override; // 終了処理
	void Update(CPlayer* pPlayer) override; // 更新処理
private:
	// === 静的メンバ関数 ===
	static constexpr float
		s_fSummonObjectSlowRange = 20.0f, // 召喚されたオブジェクトをスロー状態にする範囲
		s_fSummonObjLerpLength = 20.0f; // 召喚されたオブジェクトをリープさせる一定距離

	// === メンバ変数 ===

	D3DXVECTOR3 m_GoalPos; // 目的地
	JSON m_JsonMotion; // モーションジェイソンファイル
	int m_nNowMotion; // 現在のモーション番号
	int m_nMotionFrame; // モーションフレーム数
	bool m_bNowEndMotionAfterThrust; // 現在のモーションが終わったら突きを発動するかどうか
	CVec3LerpInfo m_Vec3LerpInfo; // 3次元線形補間情報

	// === メンバ関数 ===

	void LerpProcess(CPlayer * pPlayer); // 線形補間処理
	bool ControlDecisionGoal() override; // 目的地を決める操作
	bool HandSword(CPlayer* pPlayer) override; // 手刀
	bool NextMotion(CPlayer * pPlayer);  // モーションを次に進める
};
#endif // _COLLISION_H_