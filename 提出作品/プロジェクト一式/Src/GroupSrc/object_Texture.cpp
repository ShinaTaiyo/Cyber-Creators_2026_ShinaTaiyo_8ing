//============================================================================
// 
// テクスチャオブジェクト [object_Texture.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object_Texture.h"

// デバイス取得用
#include "renderer.h"

// サウンド取得用
#include "sound.h"

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
CObject_Texture::CObject_Texture(LAYER Priority) :
	CObject(Priority),
	m_pVtxBuff(nullptr),
	m_pTexture(nullptr),
	m_pSurface(nullptr),
	m_TextureSize(VEC2_INIT),
	m_TextureMove(VEC2_INIT),
	m_MeshSize(VEC2_INIT),
	m_MeshSizeTarget(VEC2_INIT),
	m_Rot(VEC3_INIT),
	m_RotTarget(VEC3_INIT),
	m_Pos(VEC3_INIT),
	m_PosTarget(VEC3_INIT),
	m_Col(XCOL_INIT),
	m_ColTarget(XCOL_INIT),
	m_fLength(0.0f),
	m_fAngle(0.0f)
{
	// ワールド行列の初期化
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// デストラクタ
//============================================================================
CObject_Texture::~CObject_Texture()
{
	// 念のためクリーンアップ
	Uninit();
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CObject_Texture::Init()
{
	// 頂点バッファの生成
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// テクスチャの生成
	if (FAILED(CreateTex()))
	{
		return E_FAIL;
	}

	// テクスチャタイプに設定
	SetType(TYPE::TEXTURE);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CObject_Texture::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// テクスチャの破棄
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

//============================================================================
// 更新処理
//============================================================================
void CObject_Texture::Update()
{
	// 頂点バッファが消失
	if (!m_pVtxBuff)
	{ 
		throw std::runtime_error("'CObject_Texture::Update()' was Failed");
	}

	// 目標値への補間
	CorrectToTarget();

	// 必要な数値を計算
	m_fAngle = atan2f(m_MeshSize.x, m_MeshSize.y);
	m_fLength = sqrtf(m_MeshSize.x * m_MeshSize.x + m_MeshSize.y * m_MeshSize.y);

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	{ // 頂点座標の設定

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
	}

	// 頂点色の設定
	for (WORD wCntVtx = 0; wCntVtx < NUM_VTX; ++wCntVtx)
	{
		pVtx[wCntVtx].col = m_Col;
	}

	// テクスチャ座標の設定
	pVtx[0].tex = { 0.0f + m_TextureMove.x, 0.0f + m_TextureMove.y };
	pVtx[1].tex = { 1.0f + m_TextureMove.x, 0.0f + m_TextureMove.y };
	pVtx[2].tex = { 0.0f + m_TextureMove.x, 1.0f + m_TextureMove.y };
	pVtx[3].tex = { 1.0f + m_TextureMove.x, 1.0f + m_TextureMove.y };

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// ワールド行列の設定
	UpdateMtxWorld(m_MtxWorld, VEC3_INIT, m_Rot, m_Pos);
}

//============================================================================
// 描画処理
//============================================================================
void CObject_Texture::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// アルファテストを有効にする
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDev->SetRenderState(D3DRS_ALPHAREF, 0);
	pDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ライト反映を無効にする
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールド行列の設定
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// 頂点バッファをデータストリームに設定
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDev->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDev->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,										// 頂点情報の先頭アドレス
		NUM_PRIM);								// プリミティブ数

	// ライト反映を有効にする
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	// アルファテストを無効にする
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================
// 生成
//============================================================================
CObject_Texture* CObject_Texture::Create(const JSON& json)
{
	// インスタンスを生成
	CObject_Texture* pNewInstance = DBG_NEW CObject_Texture();

	// 生成失敗
	if (!pNewInstance)
	{
		throw std::runtime_error("'CObject_Texture::Create(const JSON& json)' was Failed");
	}

	{ // パラメータの設定

		// データをキャスト
		float
			DecayRate = static_cast<float>(json["DecayRate"]);
		Vec2
			TextureSize = hws::JsonConvertToVec2(json["TextureSize"]),
			TextureMove = hws::JsonConvertToVec2(json["TextureMove"]),
			MeshSize = hws::JsonConvertToVec2(json["MeshSize"]),
			MeshSizeTarget = hws::JsonConvertToVec2(json["MeshSizeTarget"]);
		Vec3
			Rot = hws::JsonConvertToVec3(json["Rot"]),
			RotTarget = hws::JsonConvertToVec3(json["RotTarget"]),
			Pos = hws::JsonConvertToVec3(json["Pos"]),
			PosTarget = hws::JsonConvertToVec3(json["PosTarget"]);
		XCol
			Col = hws::JsonConvertToXCol(json["Col"]),
			ColTarget = hws::JsonConvertToXCol(json["ColTarget"]);

		// データをセット
		pNewInstance->m_fDecayRate = DecayRate;				// 減衰割合
		pNewInstance->m_TextureSize = TextureSize;			// テクスチャサイズ
		pNewInstance->m_TextureMove = TextureMove;			// テクスチャムーブ
		pNewInstance->m_MeshSize = MeshSize;				// メッシュサイズ
		pNewInstance->m_MeshSizeTarget = MeshSizeTarget;	// 目標メッシュサイズ
		pNewInstance->m_Rot = Rot;							// 向き
		pNewInstance->m_RotTarget = RotTarget;				// 目標向き
		pNewInstance->m_Pos = Pos;							// 座標
		pNewInstance->m_PosTarget = PosTarget;				// 目標座標
		pNewInstance->m_Col = Col;							// 色
		pNewInstance->m_ColTarget = ColTarget;				// 目標色
	}

	// テキストメッシュの初期化処理
	pNewInstance->Init();

	return pNewInstance;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 頂点バッファの生成
//============================================================================
HRESULT CObject_Texture::CreateVtxBuff()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 頂点バッファの生成
	pDev->CreateVertexBuffer(
		sizeof(VERTEX_3D) * NUM_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);
	
	// 生成失敗
	if (!m_pVtxBuff)
	{
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

	// テクスチャ座標の初期化
	InitializeTextureMappingInVertex(pVtx);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// テクスチャの生成
//============================================================================
HRESULT CObject_Texture::CreateTex()
{
	// デバイスを取得
	auto pDev = CRenderer::GetDeviece();

	// テクスチャを作成
	HRESULT hr = pDev->CreateTexture(
		static_cast<UINT>(m_TextureSize.x),	// U
		static_cast<UINT>(m_TextureSize.y),	// V
		0,									// ミップマップレベル
		D3DUSAGE_RENDERTARGET,				// テクスチャの性質
		D3DFMT_A8R8G8B8,					// ピクセルフォーマット
		D3DPOOL_DEFAULT,					// メモリ管理フラグ
		&m_pTexture,						// テクスチャ保存先
		nullptr);							// ハンドル

	// 生成失敗
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// テクスチャのサーフェイス情報を保持
	m_pTexture->GetSurfaceLevel(0, &m_pSurface);

	return S_OK;
}

//============================================================================
// 目標値への補間
//============================================================================
void CObject_Texture::CorrectToTarget()
{
	// 補間の強度
	const float fCoef = 0.05f;

	// メッシュサイズ
	ExponentialDecay(m_MeshSize, m_MeshSizeTarget, fCoef);

	// 向き
	ExponentialDecay(m_Rot, m_RotTarget, fCoef);

	// 座標
	ExponentialDecay(m_Pos, m_PosTarget, fCoef);

	// 色
	ExponentialDecay(m_Col, m_ColTarget, fCoef);
}