//============================================================================
//
// 二次ベジェ曲線 [quadratic_bezier.cpp]
// Author : 福田歩希
//
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "quadratic_bezier.h"

// 描画用
#include "manager.h"
#include "renderer.h"

// オブジェクト生成用
#include "object_X.h"

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
// デフォルトコンストラクタ
//============================================================================
CQuadratic_Bezier::CQuadratic_Bezier() :
	m_pLineVtx(nullptr),
	m_fRate(0.0f),
	m_fSpeed(DEFAULT_SPEED),
	m_bUseReverse(false),
	m_bIsReverse(false),
	m_bIsEndFirstTime(false),
	m_bDispModel(false)
{
	// ワールド行列を初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 制御点の初期化
	for (WORD wIdx = 0; wIdx < NUM_CONTROL; ++wIdx)
	{
		m_ControlPoint[wIdx] = VEC3_INIT;
		m_pControlPointModel[wIdx] = nullptr;
	}

	// 移動点の初期化
	for (WORD wIdx = 0; wIdx < NUM_TRACE; ++wIdx)
	{
		m_TracePoint[wIdx] = VEC3_INIT;
		m_pTracePointModel[wIdx] = nullptr;
	}

	// 二次曲線軌道の初期化
	m_vTrajectoryModel.clear();
}

//============================================================================
// デストラクタ
//============================================================================
CQuadratic_Bezier::~CQuadratic_Bezier()
{
	// 念のためクリーンアップ
	Finalize();
}

//============================================================================
// 初期化処理
//============================================================================
bool CQuadratic_Bezier::Initialize()
{
	// モデル表示を行わない場合初期化するものが無い
	if (!m_bDispModel)
	{
		return true;
	}

	// 頂点バッファの生成
	if (FAILED(CreateVtxBuff()))
	{
		return false;
	}

	// 制御点生成用のJSONファイルを展開
	const JSON& Json_Control = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\ControlPoint.json");

	// ① - 制御点の生成
	for (WORD wIdx = 0; wIdx < NUM_CONTROL; ++wIdx)
	{
		m_pControlPointModel[wIdx] = CObject_X::Create(Json_Control);
	}

	// 移動点生成用のJSONファイルを展開
	const JSON& Json_Trace = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\TracePoint.json");

	// ② - 移動点の生成
	for (WORD wIdx = 0; wIdx < NUM_TRACE; ++wIdx)
	{
		m_pTracePointModel[wIdx] = CObject_X::Create(Json_Trace);
	}

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CQuadratic_Bezier::Finalize()
{
	// 頂点バッファの破棄
	if (m_pLineVtx != nullptr)
	{
		m_pLineVtx->Release();
		m_pLineVtx = nullptr;
	}

	// 制御点の解放予約
	for (WORD wIdx = 0; wIdx < NUM_CONTROL; ++wIdx)
	{
		if (!m_pControlPointModel[wIdx])
		{
			continue;
		}

		m_pControlPointModel[wIdx]->SetRelease();
	}

	// 移動点の解放予約
	for (WORD wIdx = 0; wIdx < NUM_TRACE; ++wIdx)
	{
		if (!m_pTracePointModel[wIdx])
		{
			continue;
		}

		m_pTracePointModel[wIdx]->SetRelease();
	}

	// 二次軌跡用の全てのモデルに破棄予約
	SetReleaseAllTrajectory();
}

//============================================================================
// 更新処理
//============================================================================
bool CQuadratic_Bezier::Update()
{
	// 進行度の更新
	if (UpdateRate())
	{
		return true;
	}

	return false;
}

//============================================================================
// 描画処理
//============================================================================
void CQuadratic_Bezier::Draw()
{
	// モデル表示を行わない場合描画処理を行ってはいけない
	if (!m_bDispModel)
	{
		return;
	}

	// 頂点座標の更新
	UpdateVtxPos();

	// ワールド行列の更新
	UpdateMtxWorld(m_MtxWorld, SCALE_INIT, VEC3_INIT, VEC3_INIT);

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ライトをオフ
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの設定
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// 頂点バッファをデータストリームに設定
	pDev->SetStreamSource(0, m_pLineVtx, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDev->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDev->SetTexture(0, nullptr);

	// 線の描画
	pDev->DrawPrimitive(
		D3DPT_LINESTRIP,	// プリミティブの種類
		0,					// 頂点情報の先頭アドレス
		NUM_CONTROL - 1);	// プリミティブ数

	// ライトをオン
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 初回移動時に限り、軌跡を描くためのモデルを生成する
	if (!m_bIsEndFirstTime)
	{
		// モデルに必要な追加パラメータの展開
		const JSON& Json_QiadraticTrajectory = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\TracePoint.json");

		// モデルインスタンスを生成
		CObject_X* p = CObject_X::Create(Json_QiadraticTrajectory);

		// 生成失敗
		if (!p)
		{
			throw std::runtime_error("Failed to Create QiadraticTrajectory");
		}

		// この時点での移動点の座標を、軌跡の座標として設定する
		p->SetPos(m_TracePoint[2]);

		// 念のため、配列にポインタを格納しておく
		m_vTrajectoryModel.push_back(p);
	}
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 頂点バッファを生成
//============================================================================
HRESULT CQuadratic_Bezier::CreateVtxBuff()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 頂点バッファの生成
	pDev->CreateVertexBuffer(
		sizeof(VERTEX_3D) * NUM_CONTROL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pLineVtx,
		nullptr);

	// 生成失敗
	if (m_pLineVtx == nullptr)
	{
		return E_FAIL;
	}

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pLineVtx->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (WORD wIdx = 0; wIdx < NUM_CONTROL; ++wIdx)
	{
		// 頂点座標を設定
		pVtx[wIdx].pos = VEC3_INIT;

		// 法線ベクトルの設定
		pVtx[wIdx].nor = VEC3_INIT;

		// 頂点色の設定
		pVtx[wIdx].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[wIdx].tex = VEC2_INIT;
	}

	// 頂点バッファをアンロックする
	m_pLineVtx->Unlock();

	return S_OK;
}

//============================================================================
// 二次軌跡用の全てのモデルに破棄予約
//============================================================================
void CQuadratic_Bezier::SetReleaseAllTrajectory()
{
	for (const auto& it : m_vTrajectoryModel)
	{
		if (!it)
		{
			continue;
		}

		it->SetRelease();
	}

	// コンテナ自体も初期化
	m_vTrajectoryModel.clear();
}

//============================================================================
// 進行度の更新
//============================================================================
bool CQuadratic_Bezier::UpdateRate()
{
	// 終端に達したフラグ
	bool bIsReachEnd = false;

	// 進行度の変動
	if (!m_bIsReverse)
	{
		if (m_fRate < 1.0f)
		{
			m_fRate += m_fSpeed;
		}
	}
	else
	{
		if (m_fRate > 0.0f)
		{
			m_fRate += -m_fSpeed;
		}
	}

	/*
		超過してしまった際の数値を強制敵に制限していますが…
		移動速度を鑑みて、線分の長さと、超過してしまう予定の距離から、
		最も綺麗に進行していく割合に補正したいけど、誤魔化したままでも良い…
	*/
	if (m_fRate > 1.0f)
	{
		m_fRate = 1.0f;

		if (!m_bUseReverse)
		{
			// 終端に到達したフラグを立てる
			bIsReachEnd = true;
		}
		else
		{
			// 反転フラグを反転する…
			m_bIsReverse = !m_bIsReverse;
		}

		// 初回移動終了フラグを立てる
		if (!m_bIsEndFirstTime)
		{
			m_bIsEndFirstTime = true;
		}
	}
	else if (m_fRate < 0.0f)
	{
		m_fRate = 0.0f;

		// 反転フラグを反転する…
		m_bIsReverse = !m_bIsReverse;
	}

	// ① - 進行度に合わせて、制御点間上の移動点の座標を更新
	for (WORD wIdx = 0; wIdx < NUM_CONTROL - 1; ++wIdx)
	{
		const Vec3& posLinear = Lerp(m_ControlPoint[wIdx], m_ControlPoint[wIdx + 1], m_fRate);
		m_TracePoint[wIdx] = posLinear;
	}

	// ② - 移動点同士を結ぶ線上に、同じ進行度でさらなる移動点を設定
	const Vec3& posQuadratic = Lerp(m_TracePoint[0], m_TracePoint[1], m_fRate);
	m_TracePoint[2] = posQuadratic;

	/* 制御点と移動点の座標を出力する */
#if 0
	for (WORD i = 0; i < NUM_CONTROL; ++i)
	{
		CRenderer::SetDebugString(to_string(m_pTracePointModel[i]->GetPos().x) + " : " + to_string(m_pTracePointModel[i]->GetPos().y) + " : " + to_string(m_pTracePointModel[i]->GetPos().z));
	}

	for (WORD i = 0; i < NUM_CONTROL; ++i)
	{
		CRenderer::SetDebugString(to_string(m_ControlPoint[i].x) + " : " + to_string(m_ControlPoint[i].y) + " : " + to_string(m_ControlPoint[i].z));
	}
#endif

	return bIsReachEnd;
}

//============================================================================
// 頂点座標の更新
//============================================================================
void CQuadratic_Bezier::UpdateVtxPos()
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pLineVtx->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// 頂点座標を制御点の座標に同期
	for (WORD wIdx = 0; wIdx < NUM_CONTROL; ++wIdx)
	{
		pVtx[wIdx].pos = m_ControlPoint[wIdx];
	}

	// 頂点バッファをアンロックする
	m_pLineVtx->Unlock();
}