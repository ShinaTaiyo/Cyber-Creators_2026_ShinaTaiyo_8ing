//============================================================================
// 
// ２０２５年４月２８日：ブロッククラスを作成 [block.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "block.h"        // 元ヘッダ
#include "bounding_box.h" // 箱判定情報
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
CBlock::CBlock() :
	CObject_X{ LAYER::DEFAULT },
	m_pCollisionInfo_Box{DBG_NEW CCollisionInfo_BoundingBox()}
{
	SetType(CObject::TYPE::BLOCK); // タイプを「ブロック」に設定

	// 箱判定情報初期化
	m_pCollisionInfo_Box->SetUpdateInfo(GetPos(),VEC3_INIT, VEC3_INIT, GetSize(),GetVtxMax(),GetVtxMin(),false,false,false,false);
}

//============================================================================
// デストラクタ
//============================================================================
CBlock::~CBlock()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CBlock::Init()
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
void CBlock::Uninit()
{
	if (m_pCollisionInfo_Box != nullptr)
	{
		delete m_pCollisionInfo_Box;    // 解放
		m_pCollisionInfo_Box = nullptr; // ポインタ初期化
	}

	// Xオブジェクトクラスの終了処理
	CObject_X::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CBlock::Update()
{
	// Xオブジェクトクラスの更新
	CObject_X::Update();

	// 箱判定情報更新
	m_pCollisionInfo_Box->SetUpdateInfo(GetPos(),VEC3_INIT,VEC3_INIT, GetSize(), GetVtxMax(), GetVtxMin(), false, false, false, false);
}

//============================================================================
// 描画処理
//============================================================================
void CBlock::Draw()
{
	// Xオブジェクトクラスの描画処理
	CObject_X::Draw();
}

//============================================================================
// 判定情報取得
//============================================================================
CCollisionInfo* CBlock::GetCollisionInfo() const
{
	return m_pCollisionInfo_Box;
}

//============================================================================
// 生成
//============================================================================
CBlock* CBlock::Create(D3DXVECTOR3 InitPos)
{
	// インスタンスを生成
	CBlock* pBlock = DBG_NEW CBlock;

	// 生成失敗
	if (pBlock == nullptr)
	{ 
		assert(false);
	}

	// タイプを設定
	pBlock->SetType(TYPE::BLOCK);

	// 初期設定
	pBlock->Init();

	// 初期位置の設定
	pBlock->SetPos(InitPos);

	// モデルを設定
	pBlock->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey("Block"));

	// サイズを設定
	pBlock->SetSize(pBlock->GetModel()->Size);

	// 最大頂点設定
	pBlock->SetBaseVtxMax(pBlock->GetModel()->VtxMax);

	// 最小頂点設定
	pBlock->SetBaseVtxMin(pBlock->GetModel()->VtxMin);

	//判定情報設定
	pBlock->m_pCollisionInfo_Box->SetUpdateInfo(pBlock->GetPos(),VEC3_INIT,VEC3_INIT, pBlock->GetSize(), pBlock->GetVtxMax(), pBlock->GetVtxMin(), false, false, false, false);

	return pBlock;
}