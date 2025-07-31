//============================================================================
// 
// ２０２５年７月１３日：線形補間クラスを作る [Vec3LerpInfo.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _LERPSCALE_INFO_H_
#define _LERPSCALE_INFO_H_	// 二重インクルード防止

//****************************************************
// インクルード
//****************************************************
#include "object.h"

//****************************************************
// ３次元ベクトル線形補間クラス
//****************************************************
class CVec3LerpInfo
{
public:
	// === 特別関数 ===
	CVec3LerpInfo();  // コンストラクタ
	~CVec3LerpInfo(); // デストラクタ

	// === ライフサイクルメンバ関数 ===

	void Update(D3DXVECTOR3& Vec, CObject* pObj); // 更新処理

	// === メンバ関数 ===

	// 初期化関数
	void SetInitialize(int nDuration, D3DXVECTOR3 StartVec, D3DXVECTOR3 GoalVec);

	// 使用するかどうか
	const bool& GetUse() const; // 取得
	void SetUse(bool bUse); // 設定

private:
	// === 静的メンバ変数 ===
	static constexpr int s_nNormalLerpDuration = 5; // 通常線形補間到達時間

	// === メンバ変数 ===

	int m_nLerpDuration; // 線形補間到達時間
	int m_nCntDuration; // 到達時間カウント用
	bool m_bUse; // 使用するかどうか
	D3DXVECTOR3 m_StartVec; // 開始拡大率
	D3DXVECTOR3 m_GoalVec; // 目標拡大率
};
#endif
