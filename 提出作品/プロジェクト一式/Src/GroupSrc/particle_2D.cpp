//============================================================================
// 
// ２０２５年５月１２日：タイマーによる遷移を作成 [particle_2D.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "particle_2D.h"
#include "bounding_box.h"
#include "renderer.h"

// テクスチャ取得用
#include "texture_manager.h"

// 情報
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
CParticle_2D::CParticle_2D() :
	CObject_2D{ LAYER::FRONT },
	m_nLife{1},
	m_nMaxLife{1},
	m_InitSize{VEC3_INIT},
	m_pMove_Info{DBG_NEW CMove_Info()}
{
	SetType(CObject::TYPE::PARTICLE_2D); // タイプを「パーティクル2D」に設定
}

//============================================================================
// デストラクタ
//============================================================================
CParticle_2D::~CParticle_2D()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CParticle_2D::Init()
{
	// オブジェクト2Dクラスの初期設定
	if (FAILED(CObject_2D::Init()))
	{
		assert(false && "数字の初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CParticle_2D::Uninit()
{
	// 移動情報終了処理
	if (m_pMove_Info != nullptr)
	{
		delete m_pMove_Info;    // 破棄
		m_pMove_Info = nullptr; // ポインタ初期化
	}

	// オブジェクト2Dクラスの終了処理
	CObject_2D::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CParticle_2D::Update()
{
	// === 変数 ===

	float fRatioLife = static_cast<float>(m_nLife) / static_cast<int>(m_nMaxLife); // 体力割合
	D3DXCOLOR Col = GetCol(); //色合い
	D3DXVECTOR3 Pos = GetPos(); // 位置

	// === 処理 ===

	SetSize(m_InitSize * fRatioLife);  // 初期サイズに体力割合を掛けて縮小する
	m_nLife -= 1; // 体力を減らす

	if (m_nLife <= 0)
	{// 体力がなくなったら
		SetRelease(); //　破棄する
	}

	SetCol(D3DXCOLOR(Col.r, Col.g, Col.b, fRatioLife)); // アルファ値を体力割合にする

	// 移動情報が存在するなら、それの更新
	if (m_pMove_Info != nullptr)
	{
		m_pMove_Info->Update(Pos, this);
	}

	// 位置の設定
	SetPos(Pos);

	// オブジェクト2Dクラスの更新
	CObject_2D::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CParticle_2D::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	//aブレンディングを加算合成に設定
	pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject_2D::Draw();//オブジェクト2Dの描画処理

	//aブレンディングを元に戻す
	pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================================
// 体力取得
//============================================================================
const int& CParticle_2D::GetLife() const
{
	return m_nLife;
}

//============================================================================
// 体力設定
//============================================================================
void CParticle_2D::SetLife(int nLife)
{
	m_nLife = nLife;
}

//============================================================================
// 最大体力取得
//============================================================================
const int& CParticle_2D::GetMaxLife() const
{
	return m_nMaxLife;
}

//============================================================================
// 最大体力設定
//============================================================================
void CParticle_2D::SetMaxLife(int nMaxLife)
{
	m_nMaxLife = nMaxLife;
}

//============================================================================
// 最初のサイズ取得
//============================================================================
const D3DXVECTOR3& CParticle_2D::GetInitSize() const
{
	return m_InitSize;
}

//============================================================================
// 最初のサイズ設定
//============================================================================
void CParticle_2D::SetInitSize(D3DXVECTOR3 InitSize)
{
	m_InitSize = InitSize;
}

//============================================================================
// 移動情報を取得する
//============================================================================
CMove_Info* CParticle_2D::GetMoveInfo() const
{
	return m_pMove_Info;
}

//============================================================================
// 生成
//============================================================================
CParticle_2D* CParticle_2D::Create(D3DXCOLOR Col, D3DXVECTOR3 InitPos)
{
	// インスタンスを生成
	CParticle_2D* pParticle_2D = DBG_NEW CParticle_2D();

	if (pParticle_2D == nullptr)
	{ // 生成失敗
		assert(false);
	}

	// タイプを設定
	pParticle_2D->SetType(TYPE::PARTICLE_2D);

	// 色合いを設定
	pParticle_2D->SetCol(Col);

	// テクスチャ設定
	pParticle_2D->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("Shadow_000"));

	// CParticle_2D
	pParticle_2D->Init();

	// 初期位置の設定
	pParticle_2D->SetPos(InitPos);

	return pParticle_2D;
}