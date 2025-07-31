//============================================================================
// 
// ２０２５年７月９日：３次元ベクトル指数減衰クラスを作る [Vec3ExponertialInfo.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _VEC3EXPONERTIAL_INFO_H_
#define _VEC3EXPONERTIAL_INFO_H_	// 二重インクルード防止

//****************************************************
// インクルード
//****************************************************
#include "object.h"

//****************************************************
// ３次元ベクトル指数減衰クラス
//****************************************************
class CVec3ExponertialInfo
{
public:
	// === 特別関数 ===

	CVec3ExponertialInfo();  // コンストラクタ
	~CVec3ExponertialInfo(); // デストラクタ

	// === ライフサイクルメンバ関数 ===

    void Update(D3DXVECTOR3& Self,CObject * pObj); // 更新処理

	// === メンバ関数 ===

	// 初期設定
	void SetInitialize(
		float fExpDecayRate, // 指数減衰倍率
		D3DXVECTOR3 Target   // 目標値
	);

	// 目標値
	const D3DXVECTOR3& GetTarget() const; // 取得
	void SetTarget(D3DXVECTOR3 Target); // 設定

	// 使用するかどうか
	const bool& GetUse() const; // 取得
	void Setuse(bool bUse); // 設定
		 
private:
	// === 静的メンバ変数 ===
	static constexpr float s_fNormalExpDecayRate = 0.5f; // 通常指数減衰倍率
	static constexpr float s_fCompleteDistance = 0.01f; // 処理を完了とする距離

	// === メンバ変数 ===
	float m_fExpDecayRate; // 指数減衰倍率
	bool m_bUse; // 使用するかどうか
	D3DXVECTOR3 m_Target; // 目標値
};
#endif
