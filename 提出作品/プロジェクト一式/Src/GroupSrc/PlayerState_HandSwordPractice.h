//============================================================================
// 
// ２０２５年７月１０日：召喚関係のバグの改善とプレイヤー条件分岐をステートパターンにする [PlayerState_HandSwordPractice.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _PLAYERSTATE_HandSwordPractice_H_
#define _PLAYERSTATE_HandSwordPractice_H_	

//****************************************************
// インクルードファイル
//****************************************************
#include "PlayerState_BeforeThrust.h"
#include "Vec3LerpInfo.h"

//****************************************************
// 前方宣言
//****************************************************
class CButton;

//****************************************************
// プレイヤー突き前ガイドクラス
//****************************************************
class CPlayerState_HandSwordPractice : public CPlayerState_BeforeThrust
{
public:
	// === 特殊関数 ===

	CPlayerState_HandSwordPractice(
		CPlayer* pPlayer,       // プレイヤーへのポインタ
		int nAnticipationFrame // 見切りフレーム数
	); // コンストラクタ
	~CPlayerState_HandSwordPractice() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	void Init(CPlayer* pPlayer) override;   // 初期化処理
	void Uninit(CPlayer* pPlayer) override; // 終了処理
	void Update(CPlayer* pPlayer) override; // 更新処理

	// === メンバ関数 ===
	static bool GetEndPractice(); // 練習が終わったかどうか取得

private:
	// === 静的メンバ変数 ===
	
	static bool s_bEndPractice; // 練習が終了したかどうか

	// === メンバ関数 ===

	bool ControlDecisionGoal() override; // 目的地を決める操作
};
#endif // _COLLISION_H_