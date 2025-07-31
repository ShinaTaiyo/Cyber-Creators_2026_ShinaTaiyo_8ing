//============================================================================
// 
// ２０２５年６月１１日：回転円柱との当たり判定を行う [Hole.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "Hole.h"
#include "RotatingCylinder.h" // 回転円柱情報
#include "move_info.h" // 移動情報
#include "StencilMask.h" // ステンシルマスク
#include "renderer.h"
// 入力用
#include "manager.h"

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
CHole::CHole() :
	CObject_X{ LAYER::DEFAULT },
	m_pRotating_Cylinder{ DBG_NEW CRotatingCylinder() },
	m_ParentPos{ VEC3_INIT },
	m_ParentRot{ VEC3_INIT }
{
	SetType(CObject::TYPE::HOLE);     // タイプを「穴」に設定

//#ifndef  _DEBUG
	SetUseCol(true); // Release時だけ色合いを使用する
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); // 色合いを透明にする
//#endif // ! _DEBUG

}

//============================================================================
// デストラクタ
//============================================================================
CHole::~CHole()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CHole::Init()
{
	// Xオブジェクトクラスの初期設定
	if (FAILED(CObject_X::Init()))
	{
		assert(false && "プレイヤーの初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CHole::Uninit()
{
	// 回転円柱情報終了
	if (m_pRotating_Cylinder != nullptr)
	{
		delete m_pRotating_Cylinder;
		m_pRotating_Cylinder = nullptr;
	}

	// Xオブジェクトクラスの終了処理
	CObject_X::Uninit();
}

//============================================================================
// 更新処理（オブジェクトクラスで呼びたくない）
//============================================================================
void CHole::ExtraUpdate()
{
	// Xオブジェクトクラスの更新
	CObject_X::Update();

	const D3DXVECTOR3&
		VtxMax = GetVtxMax(), // 最大頂点
		VtxMin = GetVtxMin(), // 最小頂点
		Rot = GetRot();       // 向き
	

	// 回転円柱情報更新
	m_pRotating_Cylinder->SetPos(GetPos());
	m_pRotating_Cylinder->SetVtxMax(VtxMax);
	m_pRotating_Cylinder->SetVtxMin(VtxMin);
	m_pRotating_Cylinder->SetRot(Rot);
}

//============================================================================
// 描画処理
//============================================================================
void CHole::Draw()
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetRenderer()->GetDeviece();
#ifdef _DEBUG
	// ステンシルマスクの影響を受けたくない場所で呼ぶ
	CStencilMask::SetOtherStencilAvoid();

	CObject_X::Draw();

	// ステンシル設定を元に戻す
	CStencilMask::SetOtherResetStencilState();

#endif // _DEBUG
}

//============================================================================
// 回転円柱情報取得
//============================================================================
CRotatingCylinder* CHole::GetRotatingCylinder() const
{
	return m_pRotating_Cylinder;
}

//============================================================================
// 親の位置を設定
//============================================================================
void CHole::SetParentPos(D3DXVECTOR3 ParentPos)
{
	m_ParentPos = ParentPos;
}

//============================================================================
// 親の向きを設定
//============================================================================
void CHole::SetParentRot(D3DXVECTOR3 ParentRot)
{
	m_ParentRot = ParentRot;
}

//============================================================================
// 生成
//============================================================================
CHole* CHole::Create(D3DXVECTOR3 InitPos)
{
	// インスタンスを生成
	CHole* pHole = DBG_NEW CHole;

	// 生成失敗
	if (pHole == nullptr)
	{
		assert(false);
	}

	// タイプを設定
	pHole->SetType(TYPE::HOLE);

	// 初期設定
	pHole->Init();

	// 初期位置の設定
	pHole->SetPos(InitPos);

	// モデルを設定
	pHole->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey("Cylinder"));

	// サイズを設定
	pHole->SetSize(pHole->GetModel()->Size);

	// 最大頂点設定
	pHole->SetBaseVtxMax(pHole->GetModel()->VtxMax);

	// 最小頂点設定
	pHole->SetBaseVtxMin(pHole->GetModel()->VtxMin);

	// 回転円柱情報初期化
	CRotatingCylinder* pRotCylinder = pHole->m_pRotating_Cylinder;
	pRotCylinder->SetPos(pHole->GetPos());
	pRotCylinder->SetVtxMax(pHole->GetBaseVtxMax());
	pRotCylinder->SetVtxMin(pHole->GetBaseVtxMin());
	pRotCylinder->SetRot(pHole->GetRot());

	return pHole;
}

//============================================================================
// ワールドマトリックスを設定する（親の拡大率を反映させたくないので手動で設定）
//============================================================================
void CHole::UpdateMtxWorld()
{
	// ※なぜ親のワールド行列から位置や向きを参照しない？
	//→ワールド行列の位置や向きには、今回反映させたくない"スケール"も反映されてしまいます。スケール使いたくないので
	//スケール抜きにしてもう一度計算し直します。

	// 計算用マトリックス
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;
	D3DXMATRIX mtxWorld;

	// ワールドマトリックスを初期化
	D3DXMatrixIdentity(&mtxWorld);

	const D3DXVECTOR3& Pos = GetPos();     // 位置取得
	const D3DXVECTOR3& Scale = GetScale(); // 拡大率取得
	const D3DXVECTOR3& Rot = GetRot();     // 向き取得

   // 自分のスケールをワールド行列に掛け合わせる
	D3DXMatrixScaling(&mtxScale, Scale.x, Scale.y, Scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 自身の向きをワールド行列に掛け合わせる
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 自身の位置をワールド行列に掛け合わせる
	D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// 親のスケールを無視したい場合、親の回転と位置だけを掛ける（順番に注意）
	D3DXMATRIX 
		mtxParentRot, 
		mtxParentTrans,
		mtxParentNoScale;

	// スケールを無視する親のワールドマトリックスを初期化
	D3DXMatrixIdentity(&mtxParentNoScale);

	// 親の向きを親のワールド行列に掛け合わせる
	D3DXMatrixRotationYawPitchRoll(&mtxParentRot, m_ParentRot.y, m_ParentRot.x, m_ParentRot.z);
	D3DXMatrixMultiply(&mtxParentNoScale, &mtxParentNoScale, &mtxParentRot);

	// 親の位置を親のワールド行列に掛け合わせる
	D3DXMatrixTranslation(&mtxParentTrans, m_ParentPos.x, m_ParentPos.y, m_ParentPos.z);
	D3DXMatrixMultiply(&mtxParentNoScale, &mtxParentNoScale, &mtxParentTrans);

	// 親と子を掛け合わせる
	D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxParentNoScale);

	// セット
	SetMtxWorld(mtxWorld);
}