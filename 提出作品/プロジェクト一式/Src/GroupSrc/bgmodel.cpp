//============================================================================
// 
// ２０２５年６月１１日：回転円柱との当たり判定を行う [BgModel.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "bgmodel.h"
#include "RotatingCylinder.h" // 回転円柱情報
#include "move_info.h" // 移動情報

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
CBgModel::CBgModel() :
	CObject_X{ LAYER::DEFAULT },
	m_pRotating_Cylinder{ DBG_NEW CRotatingCylinder() },
	m_AddRot{VEC3_INIT},
	m_pMoveInfo{DBG_NEW CMove_Info()}
{
	SetType(CObject::TYPE::BGMODEL);  // タイプを「ブロック」に設定
}

//============================================================================
// デストラクタ
//============================================================================
CBgModel::~CBgModel()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CBgModel::Init()
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
void CBgModel::Uninit()
{
	// 回転円柱情報終了
	if (m_pRotating_Cylinder != nullptr)
	{
		delete m_pRotating_Cylinder;
		m_pRotating_Cylinder = nullptr;
	}

	// 移動情報終了
	if (m_pMoveInfo != nullptr)
	{
		delete m_pMoveInfo;
		m_pMoveInfo = nullptr;
	}

	// Xオブジェクトクラスの終了処理
	CObject_X::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CBgModel::Update()
{
	D3DXVECTOR3 Pos = GetPos(); // 位置
	D3DXMATRIX MtxWorld = GetMtxWorld(); // ワールドマトリックス取得

	// Xオブジェクトクラスの更新
	CObject_X::Update();

	// 向きを加算
	SetRot(GetRot() + m_AddRot);

	// 移動情報更新
	if (m_pMoveInfo)
	{
		m_pMoveInfo->Update(Pos, this);
		SetPos(Pos);
	}

	// 回転円柱情報更新
	m_pRotating_Cylinder->SetPos(GetPos());
	m_pRotating_Cylinder->SetVtxMax(GetVtxMax());
	m_pRotating_Cylinder->SetVtxMin(GetVtxMin());
	m_pRotating_Cylinder->SetRot(GetRot());
}

//============================================================================
// 描画処理
//============================================================================
void CBgModel::Draw()
{
	// Xオブジェクトクラスの描画処理
	CObject_X::Draw();
}

//============================================================================
// 向き加算量を設定
//============================================================================
void CBgModel::SetAddRot(D3DXVECTOR3 AddRot)
{
	m_AddRot = AddRot;
}

//============================================================================
// 向き加算量を取得
//============================================================================
const D3DXVECTOR3& CBgModel::GetAddRot() const
{
	return m_AddRot;
}

//============================================================================
// 判定情報取得
//============================================================================
CCollisionInfo* CBgModel::GetCollisionInfo() const
{
	return m_pRotating_Cylinder;
}

//============================================================================
// 移動情報取得
//============================================================================
CMove_Info* CBgModel::GetMoveInfo() const
{
	return m_pMoveInfo;
}

//============================================================================
// 生成
//============================================================================
CBgModel* CBgModel::Create(D3DXVECTOR3 InitPos)
{
	// インスタンスを生成
	CBgModel* pBgModel = DBG_NEW CBgModel;

	// 生成失敗
	if (pBgModel == nullptr)
	{
		assert(false);
	}

	// タイプを設定
	pBgModel->SetType(TYPE::BGMODEL);

	// 初期設定
	pBgModel->Init();

	// 初期位置の設定
	pBgModel->SetPos(InitPos);

	// モデルを設定
	pBgModel->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey("Cylinder"));

	// サイズを設定
	pBgModel->SetSize(pBgModel->GetModel()->Size);

	// 最大頂点設定
	pBgModel->SetBaseVtxMax(pBgModel->GetModel()->VtxMax);

	// 最小頂点設定
	pBgModel->SetBaseVtxMin(pBgModel->GetModel()->VtxMin);

	// 回転円柱情報初期化
	CRotatingCylinder * pRotCylinder = pBgModel->m_pRotating_Cylinder;
	pRotCylinder->SetPos(pBgModel->GetPos());
	pRotCylinder->SetVtxMax(pBgModel->GetVtxMax());
	pRotCylinder->SetVtxMin(pBgModel->GetVtxMin());
	pRotCylinder->SetRot(pBgModel->GetRot());

	return pBgModel;
}