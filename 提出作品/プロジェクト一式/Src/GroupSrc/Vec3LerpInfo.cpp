//============================================================================
// 
// ２０２５年７月９日：3次元ベクトル線形補間クラスを作成 [Vec3LerpInfo.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "Vec3LerpInfo.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//============================================================================
// コンストラクタ
//============================================================================
CVec3LerpInfo::CVec3LerpInfo() :
	m_nCntDuration{0},
	m_nLerpDuration{0},
	m_StartVec{VEC3_INIT},
	m_GoalVec{VEC3_INIT},
	m_bUse{false}
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CVec3LerpInfo::~CVec3LerpInfo()
{
	// 何もなし
}

//============================================================================
// 更新処理
//============================================================================
void CVec3LerpInfo::Update(D3DXVECTOR3& Scale, CObject* pObj)
{
	// 線形補間到達時間よりもカウントが上なら処理をしない
	if (m_nCntDuration > m_nLerpDuration)
	{
		m_bUse = false;
	}

	// 使用するなら線形補間開始
	if (m_bUse)
	{
		// 到達時間の割合
		float fRatio = static_cast<float>(m_nCntDuration) / static_cast<float>(m_nLerpDuration);

		// 線形補間する
		Scale = (1.0f - fRatio) * m_StartVec + fRatio * m_GoalVec;

		// 到達時間をカウントする
		m_nCntDuration++;
	}
}

//============================================================================
// 初期化関数
//============================================================================
void CVec3LerpInfo::SetInitialize(int nDuration, D3DXVECTOR3 StartVec, D3DXVECTOR3 GoalVec)
{
	m_nCntDuration = 0; // 到達時間カウント用
	m_nLerpDuration = nDuration; // 到達時間
	m_StartVec = StartVec; // 開始値
	m_GoalVec = GoalVec;   // 目標値
	m_bUse = true;
}

//============================================================================
// 使用するかどうか取得
//============================================================================
const bool& CVec3LerpInfo::GetUse() const
{
	return m_bUse;
}

//============================================================================
// 使用するかどうか設定
//============================================================================
void CVec3LerpInfo::SetUse(bool bUse)
{
	m_bUse = bUse;
}
