//============================================================================
// 
// ２０２５年５月２２日：ホーミング情報クラスを作成 [horming_info.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _horming_INFO_H_
#define _horming_INFO_H_	// 二重インクルード防止

//****************************************************
// インクルード
//****************************************************
#include "object.h"

//****************************************************
// 召喚マネージャーステートクラス
//****************************************************
class CHorming_Info
{
public:
	// === 列挙型 ===

	// 狙う種類
	enum class LOCKONTYPE
	{
		NONE = 0, // なし
		PLAYER,   // プレイヤー
		MAX       // 最大
	};

	// === 特別関数 ===

	CHorming_Info();  // コンストラクタ
	~CHorming_Info(); // デストラクタ

	// === ライフサイクルメンバ関数 ===

	const D3DXVECTOR3 & Update(D3DXVECTOR3& Pos,CObject * pObj); // 更新処理

	// === メンバ関数 ===

	// 目標位置
	const D3DXVECTOR3& GetGoalPos() const; // 取得
	void SetGoalPos(D3DXVECTOR3 GoalPos);  // 設定

	// 移動量
	const D3DXVECTOR3& GetMove() const; // 取得
	void SetMove(D3DXVECTOR3 Move);     // 設定

	// 補正値
	const float& GetAdjust() const; // 取得
	void SetAdjust(float fAdjust);  // 設定

	// 速度
	const float& GetSpeed() const;   // 取得
	void SetSpeed(float fSpeed);           // 設定

	// この処理を使用するかどうか
	const bool& GetUseHorming() const; // 取得
	void SetUseHorming(bool bUse);     // 設定

	// Yaw値
	const float& GetYaw() const; // 取得
	void SetYaw(float fYaw);     // 設定

	// Pitch値
	const float& GetPitch() const; // 取得
	void SetPitch(float fPitch);   // 設定

	// 加算速度
	const float& GetAddSpeed() const;  // 取得
	void SetAddSpeed(float fAddSpeed); // 設定

	// 加算補正度
	const float& GetAddAdjust() const;   // 取得
	void SetAddAdjust(float fAddAdjust); // 設定

	// 狙うオブジェクト種類
	const LOCKONTYPE& GetLockOnType() const;   // 取得
	void SetLockOnType(LOCKONTYPE TargetType); // 設定
private:

	// === メンバ変数 ===

	D3DXVECTOR3 
		m_GoalPos,   // 目標位置
	    m_Move;      // 移動量
	LOCKONTYPE m_LockOnType; // 狙うオブジェクト種類
	float m_fAdjust,         // 補正値
		m_fAddAdjust,        // 加算補正値
		m_fSpeed,            // 速度
		m_fAddSpeed,         // 加速度
		m_fYaw,              // Yaw値
		m_fPitch;            // Pitch値
	bool m_bUse;             // ホーミングを使用するかどうか

	// === メンバ関数 ===

	void CalcYaw(D3DXVECTOR3 & Pos,CObject * pObj);     // Yaw計算
	void CalcPitch(D3DXVECTOR3 & Pos,CObject * pObj);   // Pitch計算
	void LockOnPlayer(CObject* pObj); // プレイヤーを狙う
	
};
#endif // _Summon_MANAGER_H_