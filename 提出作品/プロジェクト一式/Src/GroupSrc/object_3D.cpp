//============================================================================
// 
// 3Dオブジェクト [object_3D.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object_3D.h"
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
CObject_3D::CObject_3D(LAYER Priority) :
	CObject{ Priority },
	m_pVtxBuff{ nullptr },
	m_pTex{ nullptr },
	m_Size{ VEC3_INIT },
	m_InitSize{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Pos{ VEC3_INIT },
	m_Scale{ SCALE_INIT },
	m_Col{ XCOL_INIT },
	m_fLength{ 0.0f },
	m_fAngle{ 0.0f },
	m_upTextureAnimator(nullptr)
{
	// ワールド行列の初期化
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// デストラクタ
//============================================================================
CObject_3D::~CObject_3D()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CObject_3D::Init()
{
	// 頂点バッファの生成
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CObject_3D::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//============================================================================
// 更新処理
//============================================================================
void CObject_3D::Update()
{
	if (m_pVtxBuff == nullptr)
	{ // 頂点バッファが消失
		assert(false);
	}

	// サイズ更新
	UpdateSize();

	// 必要な数値を計算
	m_fAngle = atan2f(m_Size.x, m_Size.y);
	m_fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// 頂点座標の設定
	pVtx[0].pos = {
		sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		-cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[1].pos = {
		sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		-cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[2].pos = {
		sinf(m_Rot.z - m_fAngle) * m_fLength,
		-cosf(m_Rot.z - m_fAngle) * m_fLength,
		0.0f
	};

	pVtx[3].pos = {
		sinf(m_Rot.z + m_fAngle) * m_fLength,
		-cosf(m_Rot.z + m_fAngle) * m_fLength,
		0.0f
	};

	// 頂点色の設定
	for (WORD wCntVtx = 0; wCntVtx < NUM_VTX; ++wCntVtx)
	{
		pVtx[wCntVtx].col = m_Col;
	}

	// テクスチャアニメーション処理
	if (m_upTextureAnimator)
	{
		m_upTextureAnimator->Animate(pVtx);
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// ワールド行列の設定
	SetMtxWorld();
}

//============================================================================
// 描画処理
//============================================================================
void CObject_3D::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// アルファテストを有効にする
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDev->SetRenderState(D3DRS_ALPHAREF, 0);
	pDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// アルファブレンドを行う
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // 透明度が５０パーセントだと５０％の色の強さとしてブレンドされる
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // 描画先の色の透明度を１ーアルファ値にする（描画先の背景が５０％の強さで残る）

	// ライト反映を無効にする
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールド行列の設定
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// 頂点バッファをデータストリームに設定
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDev->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDev->SetTexture(0, m_pTex);

	// ポリゴンの描画
	pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,										// 頂点情報の先頭アドレス
		NUM_PRIM);								// プリミティブ数

	// アルファブレンドの設定を基本に戻す
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDev->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ライト反映を有効にする
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	// アルファテストを無効にする
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================
// テクスチャ割当
//============================================================================
void CObject_3D::BindTex(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTex = pTex;
}

//============================================================================
// サイズ取得
//============================================================================
const D3DXVECTOR3& CObject_3D::GetSize() const
{
	return m_Size;
}

//============================================================================
// サイズ設定
//============================================================================
void CObject_3D::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// 向き取得
//============================================================================
const D3DXVECTOR3& CObject_3D::GetRot()const
{
	return m_Rot;
}

//============================================================================
// 向き設定
//============================================================================
void CObject_3D::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// 座標取得
//============================================================================
const D3DXVECTOR3& CObject_3D::GetPos() const
{
	return m_Pos;
}

//============================================================================
// 座標設定
//============================================================================
void CObject_3D::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// 拡大率取得
//============================================================================
const D3DXVECTOR3& CObject_3D::GetScale() const
{
	return m_Scale;
}

//============================================================================
// 拡大率設定
//============================================================================
void CObject_3D::SetScale(D3DXVECTOR3 Scale)
{
	m_Scale = Scale;
}

//============================================================================
// 初期サイズ取得
//============================================================================
const D3DXVECTOR3& CObject_3D::GetInitSize() const
{
	return m_InitSize;
}

//============================================================================
// 初期サイズ設定
//============================================================================
void CObject_3D::SetInitSize(D3DXVECTOR3 Size)
{
	m_InitSize = Size;
}

//============================================================================
// 色を取得
//============================================================================
const D3DXCOLOR& CObject_3D::GetCol() const
{
	return m_Col;
}

//============================================================================
// 色を設定
//============================================================================
void CObject_3D::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// アルファ値を取得
//============================================================================
const float& CObject_3D::GetAlpha() const
{
	return m_Col.a;
}

//============================================================================
// アルファ値を設定
//============================================================================
void CObject_3D::SetAlpha(float fAlpha)
{
	m_Col.a = fAlpha;
}

//============================================================================
// 展開用対角線取得
//============================================================================
const float& CObject_3D::GetLength() const
{
	return m_fLength;
}

//============================================================================
// 生成
//============================================================================
CObject_3D* CObject_3D::Create()
{
	// インスタンスを生成
	CObject_3D* pObject3D = DBG_NEW CObject_3D();

	// 生成出来ていたら初期設定
	if (pObject3D != nullptr)
	{
		pObject3D->Init();
	}

	return pObject3D;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 頂点バッファの生成
//============================================================================
HRESULT CObject_3D::CreateVtxBuff()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 頂点バッファの生成
	pDev->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (m_pVtxBuff == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (WORD wNumVtx = 0; wNumVtx < NUM_VTX; ++wNumVtx)
	{
		// 頂点座標の設定
		pVtx[wNumVtx].pos = { VEC3_INIT };

		// 法線ベクトルの設定
		pVtx[wNumVtx].nor = { VEC3_INIT };

		// 頂点色の設定
		pVtx[wNumVtx].col = XCOL_INIT;
	}

	// テクスチャ座標の設定
	pVtx[0].tex = { 0.0f, 0.0f };
	pVtx[1].tex = { 1.0f, 0.0f };
	pVtx[2].tex = { 0.0f, 1.0f };
	pVtx[3].tex = { 1.0f, 1.0f };

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// ワールド行列設定
//============================================================================
void CObject_3D::SetMtxWorld()
{
	// 計算用行列
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールド行列を初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転行列作成
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_Rot.y,
		m_Rot.x,
		m_Rot.z);

	// 回転行列との掛け算
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxRot);

	// 平行移動行列作成
	D3DXMatrixTranslation(&mtxTrans,
		m_Pos.x,
		m_Pos.y,
		m_Pos.z);

	// 平行移動行列との掛け算
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxTrans);
}

//============================================================================
// サイズ更新
//============================================================================
void CObject_3D::UpdateSize()
{
	m_Size = D3DXVECTOR3(
		m_InitSize.x * m_Scale.x,
		m_InitSize.y * m_Scale.y,
		m_InitSize.z * m_Scale.z
	);
}
