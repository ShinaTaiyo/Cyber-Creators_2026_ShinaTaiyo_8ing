//============================================================================
// 
// ポップアップオブジェクト [object_PopUp.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object_PopUp.h"
#include "object_TextMesh.h"
#include "renderer.h"
#include "manager.h"

// テクスチャ取得用
#include "texture_manager.h"

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
CObject_PopUp::CObject_PopUp(LAYER Priority) :
	CObject_3D{ Priority },
	m_fCorrectCoef{ 0.0f },
	m_PosTarget{ VEC3_INIT },
	m_RotTarget{ VEC3_INIT },
	m_SizeTarget{ VEC3_INIT },
	m_ColTarget{ XCOL_INIT },
	m_pTextMesh{ nullptr }
{

}

//============================================================================
// デストラクタ
//============================================================================
CObject_PopUp::~CObject_PopUp()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CObject_PopUp::Init()
{
	// 3Dオブジェクトの初期設定
	if (FAILED(CObject_3D::Init()))
	{
		return E_FAIL;
	}

	// タイプ無し
	SetType(TYPE::NONE);

	// ポップアップテクスチャ
	BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtIndex(0));

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CObject_PopUp::Uninit()
{
	// 3Dオブジェクトの終了処理
	CObject_3D::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CObject_PopUp::Update()
{
	// 目標値への補正
	CorrectToTarget();

	// 3Dオブジェクトの更新処理
	CObject_3D::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CObject_PopUp::Draw()
{
	// 3Dオブジェクトの描画処理
	CObject_3D::Draw();
}

//============================================================================
// 消滅設定
//============================================================================
void CObject_PopUp::SetDisappear()
{
	// サイズと色の目標値を初期化
	m_SizeTarget = VEC3_INIT;
	m_ColTarget = XCOL_INIT;

	// テキストを強制的に終了させる (消えながらも効果音がうるさい)
	m_pTextMesh->SetTextTarget(m_pTextMesh->GetText());
}

//============================================================================
// 全目標値リセット
//============================================================================
void CObject_PopUp::AllTargetReset()
{
	// 全ての目標値をリセット
	m_fCorrectCoef *= 0.5f;
	m_SizeTarget = VEC3_INIT;
	m_RotTarget = VEC3_INIT;
	m_PosTarget = VEC3_INIT;
	m_ColTarget = XCOL_INIT;
}

//============================================================================
// 補間強度を設定
//============================================================================
void CObject_PopUp::SetCorrectCoef(float fCoef)
{
	m_fCorrectCoef = fCoef;
}

//============================================================================
// 目標座標取得
//============================================================================
const D3DXVECTOR3& CObject_PopUp::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// 目標座標設定
//============================================================================
void CObject_PopUp::SetPosTarget(D3DXVECTOR3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// 目標向き取得
//============================================================================
const D3DXVECTOR3& CObject_PopUp::GetRotTarget() const
{
	return m_RotTarget;
}

//============================================================================
// 目標向き設定
//============================================================================
void CObject_PopUp::SetRotTarget(D3DXVECTOR3 RotTarget)
{
	m_RotTarget = RotTarget;
}

//============================================================================
// 目標サイズ取得
//============================================================================
const D3DXVECTOR3& CObject_PopUp::GetSizeTarget() const
{
	return m_SizeTarget;
}

//============================================================================
// 目標サイズ設定
//============================================================================
void CObject_PopUp::SetSizeTarget(D3DXVECTOR3 SizeTarget)
{
	m_SizeTarget = SizeTarget;
}

//============================================================================
// 目標色取得
//============================================================================
const D3DXCOLOR& CObject_PopUp::GetColTarget() const
{
	return m_ColTarget;
}

//============================================================================
// 目標色設定
//============================================================================
void CObject_PopUp::SetColTarget(D3DXCOLOR ColTarget)
{
	m_ColTarget = ColTarget;
}

//============================================================================
// 生成
//============================================================================
CObject_PopUp* CObject_PopUp::Create(JSON Json)
{
	// インスタンスを生成
	CObject_PopUp* pNewInstance = DBG_NEW CObject_PopUp();

	// 生成失敗
	if (pNewInstance == nullptr)
	{
		assert(false && "ポップアップオブジェクトの生成に失敗");
	}

	// 初期設定
	pNewInstance->Init();

	{ // パラメータを設定

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
		pNewInstance->SetCorrectCoef(fCorrectCoef);	// 補間強度
		pNewInstance->SetSize(Size);				// サイズ
		pNewInstance->SetSizeTarget(SizeTarget);	// 目標サイズ
		pNewInstance->SetRot(Rot);					// 向き
		pNewInstance->SetRotTarget(RotTarget);		// 目標向き
		pNewInstance->SetPos(Pos);					// 座標
		pNewInstance->SetPosTarget(PosTarget);		// 目標座標
		pNewInstance->SetCol(Col);					// 色
		pNewInstance->SetColTarget(ColTarget);		// 目標色
	}

	// テキストメッシュ用データのパスをコピー
	std::string FileName = Json["TextMeshFile"];

	// テキストメッシュを作成
	pNewInstance->m_pTextMesh = CObject_TextMesh::Create(hws::OpenJsonFile(FileName));

	return pNewInstance;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 目標値への補間
//============================================================================
void CObject_PopUp::CorrectToTarget()
{
	// サイズを取得
	Vec3 NowSize = GetSize();
	
	// この時、現在値・目標値とも非表示化されているなら
	if (m_SizeTarget == VEC3_INIT &&
		NowSize.x + NowSize.y < 0.1f)
	{
		// ポップアップと、付随するテキストメッシュを破棄設定
		this->SetRelease();
		m_pTextMesh->SetRelease();

		return;
	}

	// 目標サイズへ補正
	NowSize += (m_SizeTarget - NowSize) * m_fCorrectCoef;
	SetSize(NowSize);

	// 目標向きへ補正
	Vec3 NowRot = GetRot();
	hws::NormalizeAngleToDest(m_RotTarget.y, NowRot.y);	// 向きの範囲の補正
	NowRot += (m_RotTarget - NowRot) * m_fCorrectCoef;
	SetRot(NowRot);

	// 目標座標へ移動
	Vec3 NowPos = GetPos();
	NowPos += (m_PosTarget - NowPos) * m_fCorrectCoef;
	SetPos(NowPos);

	// 目標色補正
	XCol NowCol = GetCol();
	NowCol += (m_ColTarget - NowCol) * m_fCorrectCoef;
	SetCol(NowCol);

	// さらにこの時、テキストメッシュにも同様のパラメータを与える
	m_pTextMesh->SetMeshSize(NowSize);
	m_pTextMesh->SetRot(NowRot);
	m_pTextMesh->SetPos(NowPos);
	m_pTextMesh->SetCol(NowCol);
}