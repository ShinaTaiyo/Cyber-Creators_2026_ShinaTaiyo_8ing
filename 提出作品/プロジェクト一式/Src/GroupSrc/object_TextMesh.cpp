//============================================================================
// 
// テキストメッシュオブジェクト [object_TextMesh.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object_TextMesh.h"
#include "renderer.h"
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
CObject_TextMesh::CObject_TextMesh(LAYER Priority) :
	CObject{ Priority },
	m_pVtxBuff{ nullptr },
	m_pTex{ nullptr },
	m_pSurface{ nullptr },
	m_Text{},
	m_TextTarget{},
	m_nCntDisp{ 0 },
	m_nTextSpeed{ 0 },
	m_nTextDelay{ 0 },
	m_nTimer{ -1 },
	m_TextSize{ VEC2_INIT },
	m_MeshSize{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Pos{ VEC3_INIT },
	m_Col{ XCOL_INIT },
	m_fLength{ 0.0f },
	m_fAngle{ 0.0f }
{
	// ワールド行列の初期化
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// デストラクタ
//============================================================================
CObject_TextMesh::~CObject_TextMesh()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CObject_TextMesh::Init()
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

	// テキストメッシュタイプに設定
	SetType(TYPE::TEXTMESH);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CObject_TextMesh::Uninit()
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
void CObject_TextMesh::Update()
{
	// 時間経過で消滅
	if (m_nTimer != -1)
	{
		DisapperByTimer();
	}

	// テキスト送り
	TextAnimation();

	if (m_pVtxBuff == nullptr)
	{ // 頂点バッファが消失
		assert(false);
	}

	// 必要な数値を計算
	m_fAngle = atan2f(m_MeshSize.x, m_MeshSize.y);
	m_fLength = sqrtf(m_MeshSize.x * m_MeshSize.x + m_MeshSize.y * m_MeshSize.y);

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

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// ワールド行列の設定
	SetMtxWorld();
}

//============================================================================
// 描画処理
//============================================================================
void CObject_TextMesh::Draw()
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
	pDev->SetTexture(0, m_pTex);

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
// サーフェイスを取得
//============================================================================
LPDIRECT3DSURFACE9 CObject_TextMesh::GetSurface()
{
	return m_pSurface;
}

//============================================================================
// テキストを取得
//============================================================================
const std::string& CObject_TextMesh::GetText() const
{
	return m_Text;
}

//============================================================================
// テキストを設定
//============================================================================
void CObject_TextMesh::SetText(std::string Text)
{
	m_Text = Text;
}

//============================================================================
// 目標テキストを取得
//============================================================================
const std::string& CObject_TextMesh::GetTextTarget() const
{
	return m_TextTarget;
}

//============================================================================
// 目標テキストを設定
//============================================================================
void CObject_TextMesh::SetTextTarget(std::string Text)
{
	m_TextTarget = Text;
}

//============================================================================
// タイマーを設定
//============================================================================
void CObject_TextMesh::SetTimer(int nTimer)
{
	m_nTimer = nTimer;
}

//============================================================================
// テキストサイズ取得
//============================================================================
const D3DXVECTOR2& CObject_TextMesh::GetTextSize() const
{
	return m_TextSize;
}

//============================================================================
// テキストサイズ設定
//============================================================================
void CObject_TextMesh::SetTextSize(D3DXVECTOR2 Size)
{
	m_TextSize = Size;
}

//============================================================================
// メッシュサイズ取得
//============================================================================
const D3DXVECTOR3& CObject_TextMesh::GetMeshSize() const
{
	return m_MeshSize;
}

//============================================================================
// メッシュサイズ設定
//============================================================================
void CObject_TextMesh::SetMeshSize(D3DXVECTOR3 Size)
{
	m_MeshSize = Size;
}

//============================================================================
// 向き取得
//============================================================================
const D3DXVECTOR3& CObject_TextMesh::GetRot()const
{
	return m_Rot;
}

//============================================================================
// 向き設定
//============================================================================
void CObject_TextMesh::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// 座標取得
//============================================================================
const D3DXVECTOR3& CObject_TextMesh::GetPos() const
{
	return m_Pos;
}

//============================================================================
// 座標設定
//============================================================================
void CObject_TextMesh::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// 色を取得
//============================================================================
const D3DXCOLOR& CObject_TextMesh::GetCol() const
{
	return m_Col;
}

//============================================================================
// 色を設定
//============================================================================
void CObject_TextMesh::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// アルファ値を取得
//============================================================================
const float& CObject_TextMesh::GetAlpha() const
{
	return m_Col.a;
}

//============================================================================
// アルファ値を設定
//============================================================================
void CObject_TextMesh::SetAlpha(float fAlpha)
{
	m_Col.a = fAlpha;
}

//============================================================================
// 展開用対角線取得
//============================================================================
const float& CObject_TextMesh::GetLength() const
{
	return m_fLength;
}

//============================================================================
// 生成
//============================================================================
CObject_TextMesh* CObject_TextMesh::Create(JSON Json)
{
	// インスタンスを生成
	CObject_TextMesh* pNewInstance = DBG_NEW CObject_TextMesh();

	// 生成失敗
	if (pNewInstance == nullptr)
	{
		assert(false && "テキストメッシュオブジェクトの生成に失敗");
	}

	{ // パラメータの設定

		// データをキャスト
		std::string
			Text = hws::JsonConvertToSJIS(Json["Text"]);
		int
			nTextSpeed = static_cast<int>(Json["TextSpeed"]),
			nTextDelay = static_cast<int>(Json["TextDelay"]),
			nInitTextDelay = static_cast<int>(Json["InitTextDelay"]),
			nTimer = static_cast<int>(Json["Timer"]);
		Vec2
			TextSize = hws::JsonConvertToVec2(Json["TextSize"]);
		Vec3
			MeshSize = hws::JsonConvertToVec3(Json["MeshSize"]),
			Rot = hws::JsonConvertToVec3(Json["Rot"]),
			Pos = hws::JsonConvertToVec3(Json["Pos"]);
		XCol
			Col = hws::JsonConvertToXCol(Json["Col"]);

		// データをセット
		pNewInstance->m_TextTarget = Text;			// 目標テキスト
		pNewInstance->m_nTextSpeed = nTextSpeed;	// テキストスピード
		pNewInstance->m_nTextDelay = nTextDelay;	// テキストディレイ
		pNewInstance->m_nCntDisp = nInitTextDelay;	// 初回テキストディレイ
		pNewInstance->m_nTimer = nTimer;			// タイマー
		pNewInstance->m_TextSize = TextSize;		// テキストサイズ
		pNewInstance->m_MeshSize = MeshSize;		// メッシュサイズ
		pNewInstance->m_Rot = Rot;					// 向き
		pNewInstance->m_Pos = Pos;					// 座標
		pNewInstance->m_Col = Col;					// 色
	}
	
	// テキストメッシュの初期設定
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
HRESULT CObject_TextMesh::CreateVtxBuff()
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
// テクスチャの生成
//============================================================================
HRESULT CObject_TextMesh::CreateTex()
{
	// デバイスを取得
	auto pDev = CRenderer::GetDeviece();

	// テクスチャを作成
	HRESULT hr = pDev->CreateTexture(
		static_cast<UINT>(m_TextSize.x),	// U
		static_cast<UINT>(m_TextSize.y),	// V
		0,									// ミップマップレベル
		D3DUSAGE_RENDERTARGET,				// テクスチャの性質
		D3DFMT_A8R8G8B8,					// ピクセルフォーマット
		D3DPOOL_DEFAULT,					// メモリ管理フラグ
		&m_pTex,							// テクスチャ保存先
		nullptr);							// ハンドル

	if (FAILED(hr))
	{ // 生成失敗
		return E_FAIL;
	}

	// テクスチャのサーフェイス情報を保持
	m_pTex->GetSurfaceLevel(0, &m_pSurface);

	return S_OK;
}

//============================================================================
// 時間経過による消滅
//============================================================================
void CObject_TextMesh::DisapperByTimer()
{
	// タイマーをデクリメント
	--m_nTimer;

	// タイマーがゼロで
	if (m_nTimer <= 0)
	{
		// テキストを破棄
		SetRelease();
	}
}

//============================================================================
// テキスト送り
//============================================================================
void CObject_TextMesh::TextAnimation()
{
	// テキストが全て表示されていたら処理をしない
	if (m_Text == m_TextTarget)
		return;
	
	// テキスト送りカウントをインクリメント
	++m_nCntDisp;

	// テキスト送りカウントが規定値に達したら
	if (m_nCntDisp > m_nTextSpeed)
	{
		// テキスト送りカウントをリセット
		m_nCntDisp = 0;

		// 目標テキストから1文字切り分ける
		std::string SjisChar = m_TextTarget.substr(m_Text.size(), 2);

		// 改行シンボルで文字置き換え
		if (SjisChar == "\n#")
		{
			// 改行シンボルの場所をコピー
			WORD wReturnPos = static_cast<WORD>(m_TextTarget.find("\n#", 0));

			// 正規の改行文字に置換
			m_TextTarget.replace(wReturnPos, 2, "\n");

			// 切り分けた文字も置換
			SjisChar = "\n";

			// テキスト送りカウントにディレイ
			m_nCntDisp = m_nTextDelay;
		}

		// テキストを1文字追加
		m_Text += SjisChar;

		// テキストSEを鳴らす
		CSound::GetInstance()->Play(CSound::LABEL::TEXT);
	}
}

//============================================================================
// ワールド行列設定
//============================================================================
void CObject_TextMesh::SetMtxWorld()
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