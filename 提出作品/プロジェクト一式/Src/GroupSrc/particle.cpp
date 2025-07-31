//============================================================================
// 
// ２０２５年５月２２日：パーティクルを作成 [particle.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "particle.h" // パーティクル

// デバイス取得用
#include "renderer.h"

// テクスチャ取得用
#include "texture_manager.h"

// 情報
#include "move_info.h"    // 移動
#include "horming_info.h" // ホーミング情報

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
CParticle::CParticle() : 
	m_nLife{ 1 },
	m_nMaxLife{ 1 },
	m_MaxSize{ VEC3_INIT },
	m_pMove_Info{ DBG_NEW CMove_Info() },
	m_pHorming_Info{DBG_NEW CHorming_Info()},
	m_nCntFrame{0},
	CObject_Billboard()
{
	SetType(CObject::TYPE::PARTICLE); // タイプを「パーティクル」に設定
}

//============================================================================
// デストラクタ
//============================================================================
CParticle::~CParticle()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CParticle::Init()
{
	CObject_Billboard::Init(); // ビルボード初期化処理
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CParticle::Uninit()
{
	// 移動情報終了処理
	if (m_pMove_Info != nullptr)
	{
		delete m_pMove_Info;    // 破棄
		m_pMove_Info = nullptr; // ポインタ初期化
	}

	// ホーミング情報終了処理
	if (m_pHorming_Info != nullptr)
	{
		delete m_pHorming_Info;    // 破棄
		m_pHorming_Info = nullptr; // ポインタ初期化
	}

	CObject_Billboard::Uninit(); // ビルボード終了処理
}

//============================================================================
// 更新処理
//============================================================================
void CParticle::Update()
{
	float fRatioLife = static_cast<float>(m_nLife) / static_cast<float>(m_nMaxLife); // 体力の割合
	D3DXCOLOR Col = GetCol();   // 色合い
	D3DXVECTOR3 Pos = GetPos(); // 位置
	Col.a = fRatioLife;         // アルファ値を体力割合に設定

	m_nCntFrame++; // フレーム数をカウントアップ
	m_nLife -= 1;  // 体力を減らす
	SetCol(Col);   // 体力割合を反映させた色合いに設定
	SetSize(m_MaxSize * fRatioLife); // 現在のサイズを体力の割合で設定

	// 移動情報があるならその更新を行う
	if (m_pMove_Info != nullptr)
	{
		m_pMove_Info->Update(Pos, this);
		SetPos(Pos);
	}

	// ホーミング情報があるならその更新を行う
	if (m_pHorming_Info != nullptr)
	{
		SetPos(Pos + m_pHorming_Info->Update(Pos,this));
	}

	// 体力がなくなったら破棄
	if (m_nLife <= 0)
	{
		SetRelease();
	}
	
	CObject_Billboard::Update(); // ビルボード更新処理
}

//============================================================================
// 描画処理
//============================================================================
void CParticle::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ライトを無効にする
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zバッファに書き込まない
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテストを有効(アルファ値が０より大きい場合に描画する）
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDev->SetRenderState(D3DRS_ALPHAREF, 0); // 基準値を０とする
	pDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 基準値より大きい場合に描画する

	// aブレンディングを加算合成に設定（最終色 = ソース色 × ソースのアルファ + 背景色 × 1）
	// ※この設定はアルファテストをONにしないと適用されません。
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // アルファブレンドを使用する
	pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); // ブレンドの計算方法を加算合成にする
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // 描画するテクスチャや色をアルファ値に比例させる
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE); // 背景の影響度を１００％に

	CObject_Billboard::Draw(); // ビルボードの描画処理

	//ライトを有効に戻す
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Zバッファに書き込む
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテスト無効に戻す
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//aブレンディングを元に戻す
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // アルファブレンドを使用しない
	pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//============================================================================
// 移動情報を取得する
//============================================================================
CMove_Info* CParticle::GetMoveInfo() const
{
	return m_pMove_Info;
}

//============================================================================
// ホーミング情報を取得する
//============================================================================
CHorming_Info* CParticle::GetHormingInfo() const
{
	return m_pHorming_Info;
}

//============================================================================
// 最大サイズ取得
//============================================================================
const D3DXVECTOR3& CParticle::GetMaxSize() const
{
	return m_MaxSize;
}

//============================================================================
// 最大サイズ設定
//============================================================================
void CParticle::SetMaxSize(D3DXVECTOR3 MaxSize)
{
	m_MaxSize = MaxSize;
}

//============================================================================
// 体力取得
//============================================================================
const int& CParticle::GetLife() const
{
	return m_nLife;
}

//============================================================================
// 体力設定
//============================================================================
void CParticle::SetLife(int nLife)
{
	m_nLife = nLife;
}

//============================================================================
// 最大体力取得
//============================================================================
const int& CParticle::GetMaxLife() const
{
	return m_nMaxLife;
}

//============================================================================
// 最大体力設定
//============================================================================
void CParticle::SetMaxLife(int nMaxLife)
{
	m_nMaxLife = nMaxLife;
}

//============================================================================
// フレーム数を取得する
//============================================================================
const int& CParticle::GetCntFrame() const
{
	return m_nCntFrame;
}

//============================================================================
// 生成処理
//============================================================================
CParticle* CParticle::Create(D3DXVECTOR3 InitPos, D3DXVECTOR3 InitSize, D3DXCOLOR Col, int nLife, int nMaxLife)
{
	CParticle * pParticle = DBG_NEW CParticle(); // インスタンスを生成

	// 生成失敗
	if (pParticle == nullptr)
	{ 
		assert(false);
	}

	// タイプを設定
	pParticle->SetType(TYPE::PARTICLE);

	// 色合いを設定
	pParticle->SetCol(Col);

	// テクスチャ設定
	pParticle->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("Shadow_000"));

	// 初期化処理
	pParticle->Init();

	// 位置設定
	pParticle->SetPos(InitPos);

	// サイズ設定
	pParticle->SetSize(InitSize);
	pParticle->SetMaxSize(InitSize);

	// 体力設定
	pParticle->SetLife(nLife);
	pParticle->SetMaxLife(nMaxLife);

	return pParticle;
}