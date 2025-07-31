//============================================================================
// 
// ビルボードオブジェクト [object_Billboard.cpp]
// Author : 福田歩希
// 
//============================================================================

/* 削除予定 */
int op = D3DBLENDOP_ADD, src = D3DBLEND_BOTHSRCALPHA, dest = D3DBLEND_INVSRCALPHA;

//****************************************************
// インクルードファイル
//****************************************************
#include "object_billboard.h"

// デバイス取得用
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
CObject_Billboard::CObject_Billboard(LAYER Priority) :
	CObject{ Priority },
	m_pVtxBuff{ nullptr },
	m_pTex{ nullptr },
	m_Size{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Pos{ VEC3_INIT },
	m_Col{ XCOL_INIT },
	m_fLength{ 0.0f },
	m_fAngle{ 0.0f },
	m_fTexWidth{ 1.0f },
	m_fTexHeight{ 1.0f },
	m_nNowPatternU{ 0 },
	m_nNowPatternV{ 0 }
{
	D3DXMatrixIdentity(&m_MtxWorld);	// ワールド行列
}

//============================================================================
// デストラクタ
//============================================================================
CObject_Billboard::~CObject_Billboard()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CObject_Billboard::Init()
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
void CObject_Billboard::Uninit()
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
void CObject_Billboard::Update()
{
	if (m_pVtxBuff == nullptr)
	{ // 頂点バッファが消失
		assert(false);
	}

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

	// テクスチャ座標の設定
	pVtx[0].tex = { m_fTexWidth * m_nNowPatternU, m_fTexHeight * m_nNowPatternV };
	pVtx[1].tex = { m_fTexWidth * (m_nNowPatternU + 1), m_fTexHeight * m_nNowPatternV };
	pVtx[2].tex = { m_fTexWidth * (m_nNowPatternU), m_fTexHeight * (m_nNowPatternV + 1) };
	pVtx[3].tex = { m_fTexWidth * (m_nNowPatternU + 1), m_fTexHeight * (m_nNowPatternV + 1) };

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// ワールド行列の設定
	SetMtxWorld();

#if 0	/* 削除予定 */
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Alpha Blend")) {
		ImGui::InputInt("op", &op);
		ImGui::InputInt("src", &src);
		ImGui::InputInt("dest", &dest);
		ImGui::End();
	}
#endif
}

//============================================================================
// 描画処理
//============================================================================
void CObject_Billboard::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	//// アルファテストを有効にする
	//pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	//// アルファブレンドの設定を変更
	//pDev->SetRenderState(D3DRS_BLENDOPALPHA, op);
	//pDev->SetRenderState(D3DRS_SRCBLEND, src);
	//pDev->SetRenderState(D3DRS_DESTBLEND, dest);

	//// ライト反映を無効にする
	//pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	//// ライト反映を有効にする
	//pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//// アルファブレンドの設定を基本に戻す
	//pDev->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
	//pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//// アルファテストを無効にする
	//pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================
// テクスチャ割当
//============================================================================
void CObject_Billboard::BindTex(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTex = pTex;
}

//============================================================================
// サイズ取得
//============================================================================
const D3DXVECTOR3& CObject_Billboard::GetSize() const
{
	return m_Size;
}

//============================================================================
// サイズ設定
//============================================================================
void CObject_Billboard::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// 向き取得
//============================================================================
const D3DXVECTOR3& CObject_Billboard::GetRot()const
{
	return m_Rot;
}

//============================================================================
// 向き設定
//============================================================================
void CObject_Billboard::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// 座標取得
//============================================================================
const D3DXVECTOR3& CObject_Billboard::GetPos() const
{
	return m_Pos;
}

//============================================================================
// 座標設定
//============================================================================
void CObject_Billboard::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// 色を取得
//============================================================================
const D3DXCOLOR& CObject_Billboard::GetCol() const
{
	return m_Col;
}

//============================================================================
// 色を設定
//============================================================================
void CObject_Billboard::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// アルファ値を取得
//============================================================================
const float& CObject_Billboard::GetAlpha() const
{
	return m_Col.a;
}

//============================================================================
// アルファ値を設定
//============================================================================
void CObject_Billboard::SetAlpha(float fAlpha)
{
	m_Col.a = fAlpha;
}

//============================================================================
// 展開用対角線取得
//============================================================================
const float& CObject_Billboard::GetLength() const
{
	return m_fLength;
}

//============================================================================
// 横テクスチャ分割幅設定
//============================================================================
void CObject_Billboard::SetTexWidth(float fWidth)
{
	m_fTexWidth = fWidth;
}

//============================================================================
// 縦テクスチャ分割幅設定
//============================================================================
void CObject_Billboard::SetTexHeight(float fHeight)
{
	m_fTexHeight = fHeight;
}

//============================================================================
// 現在のテクスチャ横分割幅取得
//============================================================================
const int& CObject_Billboard::GetNowPatternU() const
{
	return m_nNowPatternU;
}

//============================================================================
// 現在のテクスチャ横分割幅設定
//============================================================================
void CObject_Billboard::SetNowPatternU(int nNowPatternU)
{
	m_nNowPatternU = nNowPatternU;
}

//============================================================================
// 現在のテクスチャ縦分割幅取得
//============================================================================
const int& CObject_Billboard::GetNowPatternV() const
{
	return m_nNowPatternV;
}

//============================================================================
// 現在のテクスチャ縦分割幅設定
//============================================================================
void CObject_Billboard::SetNowPatternV(int nNowPatternV)
{
	m_nNowPatternV = nNowPatternV;
}

//============================================================================
// 生成
//============================================================================
CObject_Billboard* CObject_Billboard::Create()
{
	CObject_Billboard* pObject3D = DBG_NEW CObject_Billboard{};

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
//=========================================================================
HRESULT CObject_Billboard::CreateVtxBuff()
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
void CObject_Billboard::SetMtxWorld()
{
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 計算用行列
	D3DXMATRIX mtxRot{}, mtxTrans{}, mtxView{};

	// ワールド行列を初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// ビュー行列取得
	pDev->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラの正面に向け続ける
	D3DXMatrixInverse(&m_MtxWorld, nullptr, &mtxView);

	// 逆行列を求める
	m_MtxWorld._41 = 0.0f;
	m_MtxWorld._42 = 0.0f;
	m_MtxWorld._43 = 0.0f;

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