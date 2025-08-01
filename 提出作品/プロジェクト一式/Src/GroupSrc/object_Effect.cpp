//============================================================================
// 
// エフェクトオブジェクト [object_Effect.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object_Effect.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CObject_Effect::CObject_Effect(LAYER Priority) :
	CObject_Billboard{ Priority },
	m_fCorrectionCoef{ 0.0f },
	m_SizeTarget{ VEC3_INIT },
	m_RotTarget{ VEC3_INIT },
	m_PosTarget{ VEC3_INIT },
	m_ColTarget{ XCOL_INIT },
	m_nMaxDuration{ 0 },
	m_nDuration{ 0 }
{

}

//============================================================================
// デストラクタ
//============================================================================
CObject_Effect::~CObject_Effect()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CObject_Effect::Init()
{
	// ビルボードオブジェクトの初期設定
	if (FAILED(CObject_Billboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CObject_Effect::Uninit()
{
	// ビルボードオブジェクトの終了処理
	CObject_Billboard::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CObject_Effect::Update()
{
	// 継続カウント
	CountDuration();

	// 目標値への補正
	CorrectToTaraget();

	// ビルボードオブジェクトの更新処理
	CObject_Billboard::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CObject_Effect::Draw()
{
	// ビルボードオブジェクトの描画処理
	CObject_Billboard::Draw();
}

//============================================================================
// 補正係数を設定
//============================================================================
void CObject_Effect::SetCorrectionCoef(float fCoef)
{
	m_fCorrectionCoef = fCoef;
}

//============================================================================
// 目標サイズを取得
//============================================================================
const D3DXVECTOR3& CObject_Effect::GetSizeTarget() const
{
	return m_SizeTarget;
}

//============================================================================
// 目標サイズを設定
//============================================================================
void CObject_Effect::SetSizeTarget(D3DXVECTOR3 SizeTarget)
{
	m_SizeTarget = SizeTarget;
}

//============================================================================
// 目標向きを取得
//============================================================================
const D3DXVECTOR3& CObject_Effect::GetRotTarget() const
{
	return m_RotTarget;
}

//============================================================================
// 目標向きを設定
//============================================================================
void CObject_Effect::SetRotTarget(D3DXVECTOR3 RotTarget)
{
	m_RotTarget = RotTarget;
}

//============================================================================
// 目標座標を取得
//============================================================================
const D3DXVECTOR3& CObject_Effect::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// 目標座標を設定
//============================================================================
void CObject_Effect::SetPosTarget(D3DXVECTOR3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// 目標色を取得
//============================================================================
const D3DXCOLOR& CObject_Effect::GetColTarget() const
{
	return m_ColTarget;
}

//============================================================================
// 目標色を設定
//============================================================================
void CObject_Effect::SetColTarget(D3DXCOLOR ColTarget)
{
	m_ColTarget = ColTarget;
}

//============================================================================
// 最大期間を取得
//============================================================================
const int& CObject_Effect::GetMaxDuration()
{
	return m_nMaxDuration;
}

//============================================================================
// 最大期間を設定
//============================================================================
void CObject_Effect::SetMaxDuration(int nMax)
{
	m_nMaxDuration = nMax;
}

//============================================================================
// 継続期間を取得
//============================================================================
const int& CObject_Effect::GetDuration()
{
	return m_nDuration;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 継続カウント
//============================================================================
void CObject_Effect::CountDuration()
{
	m_nDuration++;

	// 最大継続期間に到達していたら
	if (m_nDuration >= m_nMaxDuration)
	{
		// 自身を破棄予約
		SetRelease();
	}
}

//============================================================================
// 目標値への補正
//============================================================================
void CObject_Effect::CorrectToTaraget()
{
	// 目標サイズ
	Vec3 NewSize = GetSize();
	NewSize += (m_SizeTarget - NewSize) * m_fCorrectionCoef;
	SetSize(NewSize);

	// 目標向きへ補正
	Vec3 NewRot = GetRot();
	hws::NormalizeAngleToDest(NewRot.y, m_RotTarget.y);	// 角度の差を補正
	NewRot += (m_RotTarget - NewRot) * m_fCorrectionCoef;
	SetRot(NewRot);

	// 目標座標へ移動
	Vec3 NewPos = GetPos();
	NewPos += (m_PosTarget - NewPos) * m_fCorrectionCoef;
	SetPos(NewPos);

	// 目標色へ補正
	XCol NewCol = GetCol();
	NewCol += (m_ColTarget - NewCol) * m_fCorrectionCoef;
	SetCol(NewCol);
}