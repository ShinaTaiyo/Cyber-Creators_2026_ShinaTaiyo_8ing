//============================================================================
// 
// HUDオブジェクト [object_HUD.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object_HUD.h"
#include "renderer.h"

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
CObject_HUD::CObject_HUD(LAYER Priority) :
	CObject_2D{ Priority },
	m_fCorrectionCoef{ 0.0f },
	m_PosTarget{ VEC3_INIT },
	m_RotTarget{ VEC3_INIT },
	m_SizeTarget{ VEC3_INIT },
	m_ColTarget{ XCOL_INIT }
{

}

//============================================================================
// デストラクタ
//============================================================================
CObject_HUD::~CObject_HUD()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CObject_HUD::Init()
{
	// 2Dオブジェクトの初期設定
	if (FAILED(CObject_2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CObject_HUD::Uninit()
{
	// 2Dオブジェクトの終了処理
	CObject_2D::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CObject_HUD::Update()
{
	// 目標値への補正
	CorrectToTarget();

	// 2Dオブジェクトの更新処理
	CObject_2D::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CObject_HUD::Draw()
{
	// 2Dオブジェクトの描画処理
	CObject_2D::Draw();
}

//============================================================================
// 振動を与える
//============================================================================
void CObject_HUD::SetVibration()
{
	Vec3 NewPos = GetPos();
	NewPos.x += hws::GetRandomValue<float>() * 0.05f;
	NewPos.y += hws::GetRandomValue<float>() * 0.05f;
	SetPos(NewPos);
}

//============================================================================
// 波打ちを与える
//============================================================================
void CObject_HUD::SetWaving()
{
	Vec3 NewSize = GetSize();
	NewSize.x *= 1.2f;
	NewSize.y *= 0.8f;
	SetSize(NewSize);
}

//============================================================================
// 補正係数設定
//============================================================================
void CObject_HUD::SetCorrectionCoef(float fCorrectioncoef)
{
	m_fCorrectionCoef = fCorrectioncoef;
}

//============================================================================
// 目標座標取得
//============================================================================
const D3DXVECTOR3& CObject_HUD::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// 目標座標設定
//============================================================================
void CObject_HUD::SetPosTarget(D3DXVECTOR3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// 目標向き取得
//============================================================================
const D3DXVECTOR3& CObject_HUD::GetRotTarget() const
{
	return m_RotTarget;
}

//============================================================================
// 目標向き設定
//============================================================================
void CObject_HUD::SetRotTarget(D3DXVECTOR3 RotTarget)
{
	m_RotTarget = RotTarget;
}

//============================================================================
// 目標サイズ取得
//============================================================================
const D3DXVECTOR3& CObject_HUD::GetSizeTarget() const
{
	return m_SizeTarget;
}

//============================================================================
// 目標サイズ設定
//============================================================================
void CObject_HUD::SetSizeTarget(D3DXVECTOR3 SizeTarget)
{
	m_SizeTarget = SizeTarget;
}

//============================================================================
// 目標色取得
//============================================================================
const D3DXCOLOR& CObject_HUD::GetColTarget() const
{
	return m_ColTarget;
}

//============================================================================
// 目標色設定
//============================================================================
void CObject_HUD::SetColTarget(D3DXCOLOR ColTarget)
{
	m_ColTarget = ColTarget;
}

//============================================================================
// 生成
//============================================================================
CObject_HUD* CObject_HUD::Create(JSON Json)
{
	// インスタンスを生成
	CObject_HUD* pNewInstance = DBG_NEW CObject_HUD();

	// 生成出来ていたら初期設定
	if (pNewInstance == nullptr)
	{
		assert(false && "HUDオブジェクトの生成に失敗");
	}

	// 初期設定
	pNewInstance->Init();

	{ // パラメータを設定

		// ジェイソンデータのエイリアス
		JSON
			ColData = Json["Col"],
			ColTargetData = Json["ColTarget"];

		// データをキャスト
		float
			fCorrectCoef = static_cast<float>(Json["CorrectionCoef"]);
		Vec3
			Size = hws::JsonConvertToVec3(Json["Size"]),
			SizeTarget = hws::JsonConvertToVec3(Json["SizeTarget"]),
			Rot = hws::JsonConvertToVec3(Json["Rot"]),
			RotTarget = hws::JsonConvertToVec3(Json["RotTarget"]),
			Pos = hws::JsonConvertToVec3(Json["Pos"]),
			PosTarget = hws::JsonConvertToVec3(Json["PosTarget"]);
		XCol
			Col = hws::JsonConvertToXCol(Json["Col"]),
			ColTarget = hws::JsonConvertToXCol(Json["ColTarget"]);

		// データをセット
		pNewInstance->SetCorrectionCoef(fCorrectCoef);	// 補間強度
		pNewInstance->SetSize(Size);					// サイズ
		pNewInstance->SetSizeTarget(SizeTarget);		// 目標サイズ
		pNewInstance->SetRot(Rot);						// 向き
		pNewInstance->SetRotTarget(RotTarget);			// 目標向き
		pNewInstance->SetPos(Pos);						// 座標
		pNewInstance->SetPosTarget(PosTarget);			// 目標座標
		pNewInstance->SetCol(Col);						// 色
		pNewInstance->SetColTarget(ColTarget);			// 目標色
	}

	return pNewInstance;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 目標値への補正
//============================================================================
void CObject_HUD::CorrectToTarget()
{
	// 目標座標へ移動
	Vec3 NowPos = GetPos();
	NowPos += (m_PosTarget - NowPos) * m_fCorrectionCoef;
	SetPos(NowPos);

	// 目標向きへ補正
	Vec3 NowRot = GetRot();
	hws::WrapAngleToRange(m_RotTarget.y, NowRot.y);	// 向きの範囲の補正
	NowRot += (m_RotTarget - NowRot) * m_fCorrectionCoef;
	SetRot(NowRot);

	// 目標サイズへ補正
	Vec3 NowSize = GetSize();
	NowSize += (m_SizeTarget - NowSize) * m_fCorrectionCoef;
	SetSize(NowSize);

	// 目標色補正
	XCol NowCol = GetCol();
	NowCol += (m_ColTarget - NowCol) * m_fCorrectionCoef;
	SetCol(NowCol);
}