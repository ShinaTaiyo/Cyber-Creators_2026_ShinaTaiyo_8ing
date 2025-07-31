//============================================================================
// 
// Xオブジェクト [object_X.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"
#include "renderer.h"
#include "manager.h"
#include "StencilMask.h" // ステンシルマスク
#include "scene.h"   // 現在のシーンを識別するため
#include "game.h"    // ↑のシーンが「ゲーム」だったらの判別のため

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
CObject_X::CObject_X(LAYER Priority) :
	CObject{ Priority },
	m_pModel{ nullptr },
	m_Pos{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Scale{ 1.0f, 1.0f, 1.0f },
	m_Col{ XCOL_INIT },
	m_bUseCol{ false },
	m_Size{VEC3_INIT},
	m_VtxMax{VEC3_INIT},
	m_VtxMin{VEC3_INIT},
	m_BaseVtxMax{VEC3_INIT},
	m_BaseVtxMin{VEC3_INIT}
{
	// ワールド行列を初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	SetObjType(CObject::OBJTYPE::X);//オブジェクト分類タイプをXに設定
}

//============================================================================
// デストラクタ
//============================================================================
CObject_X::~CObject_X()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CObject_X::Init()
{
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CObject_X::Uninit()
{

}

//============================================================================
// 更新処理
//============================================================================
void CObject_X::Update()
{
	// Vector系の数値補間処理
	if (m_upVectorInterpolator)
	{
		// 保有している変数を渡す
		CVectorInterpolator::VectorRefWrapper rWrap(m_Scale, m_Rot, m_Pos);
		m_upVectorInterpolator->Interpolate(rWrap);
	}

	//頂点を調整する
	AdjustVtx();

	// 向きを調整する
	AdjustRotAngle();

	// ワールド行列の設定
	UpdateMtxWorld();
}

//============================================================================
// 描画処理
//============================================================================
void CObject_X::Draw()
{
	// モデルが設定されていない
	if (m_pModel == nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetDebugString(static_cast<std::string>("【") + typeid(*this).name() + static_cast<std::string>("にモデル情報がありません！】"));
#endif	// _DEBUG

		return;
	}

	// マテリアルのバッファが異常
	if (m_pModel->pBuffMat == nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetDebugString(static_cast<std::string>("【") + typeid(*this).name() + static_cast<std::string>("にマテリアル情報がありません！】"));
#endif	// _DEBUG

		return;
	}

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 頂点法線の自動正規化を有効に
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

//#ifdef _DEBUG	// 判定表示系のモデルは深度バッファに書き込まない
//	bool bDetectRenderCollision = false;
//	CX_Manager::MODEL* pModel = this->GetModel();
//	if (pModel == CX_Manager::GetInstance()->GetModel(CX_Manager::TYPE::RENDER_BOX) ||
//		pModel == CX_Manager::GetInstance()->GetModel(CX_Manager::TYPE::RENDER_CYLINDER) ||
//		pModel == CX_Manager::GetInstance()->GetModel(CX_Manager::TYPE::RENDER_SPHERE))
//	{
//		bDetectRenderCollision = true;	// 判定簡略用
//		pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//	}
//#endif // _DEBUG

	// ワールドマトリックスの設定
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// 現在のマテリアルを取得
	pDev->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

	for (WORD wCntMat = 0; wCntMat < static_cast<WORD>(m_pModel->dwNumMat); wCntMat++)
	{
		// マテリアル色の設定
		m_bUseCol ? pMat[wCntMat].MatD3D.Diffuse = m_Col : pMat[wCntMat].MatD3D.Diffuse = m_pModel->apColMat[wCntMat];

		// マテリアルの設定
		pDev->SetMaterial(&pMat[wCntMat].MatD3D);

		// テクスチャの設定
		pDev->SetTexture(0, m_pModel->apTex[wCntMat]);

		// オブジェクトパーツの描画
		m_pModel->pMesh->DrawSubset(wCntMat);
	}

	// 保存していたマテリアルを戻す
	pDev->SetMaterial(&matDef);

	// 頂点法線の自動正規化を無効に
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

	//アルファテスト無効に戻す
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//Zバッファに書き込む
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDev->SetRenderState(D3DRS_ZENABLE, TRUE);

//#ifdef _DEBUG	// 判定表示系のモデルは深度バッファに書き込まない
//	if (bDetectRenderCollision)
//		pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//#endif // _DEBUG
}

//============================================================================
// モデル取得
//============================================================================
CX_Manager::MODEL* CObject_X::GetModel()
{
	return m_pModel;
}

//============================================================================
// モデル割当
//============================================================================
void CObject_X::BindModel(CX_Manager::MODEL* pModel)
{
	m_pModel = pModel;
}
//============================================================================
// 向き取得
//============================================================================
const D3DXVECTOR3& CObject_X::GetRot()const
{
	return m_Rot;
}

//============================================================================
// 向き設定
//============================================================================
void CObject_X::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// 座標取得
//============================================================================
const D3DXVECTOR3& CObject_X::GetPos() const
{
	return m_Pos;
}


//============================================================================
// 座標設定
//============================================================================
void CObject_X::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// 縮尺を取得
//============================================================================
const D3DXVECTOR3& CObject_X::GetScale() const
{
	return m_Scale;
}


//============================================================================
// 縮尺を設定
//============================================================================
void CObject_X::SetScale(D3DXVECTOR3 Scale)
{
	m_Scale = Scale;
}

//============================================================================
// 最大頂点取得
//============================================================================
const D3DXVECTOR3& CObject_X::GetVtxMax() const
{
	return m_VtxMax;
}

//============================================================================
// 最大頂点設定
//===========================================================================
void CObject_X::SetVtxMax(D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//============================================================================
// 最小頂点取得
//===========================================================================
const D3DXVECTOR3& CObject_X::GetVtxMin() const
{
	return m_VtxMin;
}

//============================================================================
// 最小頂点設定
//===========================================================================
void CObject_X::SetVtxMin(D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//============================================================================
// 元の最大頂点取得
//============================================================================
const D3DXVECTOR3& CObject_X::GetBaseVtxMax() const
{
	return m_BaseVtxMax;
}

//============================================================================
// 元の最大頂点設定
//============================================================================
void CObject_X::SetBaseVtxMax(D3DXVECTOR3 BaseVtxMax)
{
	m_BaseVtxMax = BaseVtxMax;
}

//============================================================================
// 元の最小頂点取得
//============================================================================
const D3DXVECTOR3& CObject_X::GetBaseVtxMin() const
{
	return m_BaseVtxMin;
}

//============================================================================
// 元の最小頂点設定
//============================================================================
void CObject_X::SetBaseVtxMin(D3DXVECTOR3 BaseVtxMin)
{
	m_BaseVtxMin = BaseVtxMin;
}

//============================================================================
// 色を取得
//============================================================================
const D3DXCOLOR& CObject_X::GetCol() const
{
	return m_Col;
}

//============================================================================
// 色を設定
//============================================================================
void CObject_X::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// サイズ設定
//============================================================================
void CObject_X::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// 色反映を取得
//============================================================================
const bool& CObject_X::GetUseCol() const
{
	return m_bUseCol;
}

//============================================================================
// 色反映を設定
//============================================================================
void CObject_X::SetUseCol(bool bUse)
{
	m_bUseCol = bUse;
}

//============================================================================
// サイズを取得
//============================================================================
D3DXVECTOR3 CObject_X::GetSize() const
{
	return m_Size;
}

//============================================================================
// 半径を取得
//============================================================================
float CObject_X::GetRadius() const
{
	return 0.0f;
}

//============================================================================
// 高さを取得
//============================================================================
float CObject_X::GetHeight() const
{
	return 0.0f;
}

//============================================================================
// ワールドマトリックスを取得
//============================================================================
const D3DXMATRIX CObject_X::GetMtxWorld() const
{
	return m_MtxWorld;
}

//============================================================================
// ワールドマトリックスを設定
//============================================================================
void CObject_X::SetMtxWorld(D3DXMATRIX Mtx)
{
	m_MtxWorld = Mtx;
}

//============================================================================
// 生成
//============================================================================
CObject_X* CObject_X::Create(const std::string& key)
{
	// インスタンスを生成
	CObject_X* pObjectX = DBG_NEW CObject_X();

	// 生成失敗
	if (pObjectX == nullptr)
	{
		assert(false && "Xオブジェクトの生成に失敗しました");
	}

	// モデルを設定
	pObjectX->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey(key));

	// Xオブジェクトの初期設定
	pObjectX->Init();

	return pObjectX;
}

//============================================================================
// 生成
//============================================================================
CObject_X* CObject_X::Create(LAYER Priority, const std::string& key)
{
	// インスタンスを生成
	CObject_X* pObjectX = DBG_NEW CObject_X(Priority);

	// 生成失敗
	if (pObjectX == nullptr)
	{
		assert(false && "Xオブジェクトの生成に失敗しました");
	}

	// モデルを設定
	pObjectX->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey(key));

	// Xオブジェクトの初期設定
	pObjectX->Init();

	return pObjectX;
}

//============================================================================
// 生成
//============================================================================
CObject_X* CObject_X::Create(const JSON& Json)
{
	// パラメータをコピーしていく
	const auto& Priority = static_cast<int>(Json["Priority"]);
	const auto& ModelType = static_cast<std::string>(Json["ModelType"]);
	const auto& Rot = hws::JsonConvertToVec3(Json["Rot"]);
	const auto& Pos = hws::JsonConvertToVec3(Json["Pos"]);
	const auto& Scale = hws::JsonConvertToVec3(Json["Scale"]);

	// インスタンスを生成
	CObject_X* pObjectX = DBG_NEW CObject_X(static_cast<CObject::LAYER>(Priority));

	// 生成失敗
	if (pObjectX == nullptr)
	{
		assert(false && "Xオブジェクトの生成に失敗しました");
	}

	// モデルを設定
	pObjectX->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey(ModelType));

	// Xオブジェクトの初期設定
	pObjectX->Init();

	// 各種パラメータを設定
	pObjectX->SetRot(Rot);
	pObjectX->SetPos(Pos);
	pObjectX->SetScale(Scale);

	if (Json.contains("VectorInterpolator"))
	{
		if (static_cast<bool>(Json["VectorInterpolator"]))
		{
			CVectorInterpolator::VectorCopyWrapper wrapper =
			{
				hws::JsonConvertToVec3(Json["ScaleTarget"]),
				hws::JsonConvertToVec3(Json["RotTarget"]),
				hws::JsonConvertToVec3(Json["PosTarget"])
			};

			float aRate[3] = { 0.0f, 0.0f, 0.0f };
			unsigned char aFlags[3] = { 0, 0, 0 };

			for (unsigned char ucIdx = 0; ucIdx < 3; ++ucIdx)
			{
				aRate[ucIdx] = static_cast<float>(Json["Rate"][ucIdx]);
#if SWITCH_STARTUP_MODE
				aFlags[ucIdx] = static_cast<unsigned char>(static_cast<VectorInterpolationFlags>(Json["Flags"][ucIdx]) & ~VectorInterpolationFlags::UseInterpolation);
#else				
				aFlags[ucIdx] = static_cast<unsigned char>(Json["Flags"][ucIdx]);
#endif
			}

			Compose(pObjectX->m_upVectorInterpolator,
				[wrapper, aRate, aFlags]() -> decltype(auto)
				{
					up<CVectorInterpolator> uPtr = nullptr;
					uPtr = make_unique<CVectorInterpolator>();
					uPtr->SetScaleTarget(wrapper.Scale);
					uPtr->SetRotTarget(wrapper.Rot);
					uPtr->SetPosTarget(wrapper.Pos);
					for (unsigned char ucCntLoop = 0; ucCntLoop < 3; ++ucCntLoop)
					{
						uPtr->SetRate(ucCntLoop, aRate[ucCntLoop]);
						uPtr->SetFlags(ucCntLoop, static_cast<VectorInterpolationFlags>(aFlags[ucCntLoop]));
					}
					return uPtr;
				});
		}
	}

	return pObjectX;
}

//============================================================================
// 生成
//============================================================================
std::unique_ptr<CObject_X> CObject_X::upCreate(const JSON& Json)
{
	// パラメータをコピーしていく
	const auto& Priority = static_cast<int>(Json["Priority"]);
	const auto& ModelType = static_cast<std::string>(Json["ModelType"]);
	const auto& Rot = hws::JsonConvertToVec3(Json["Rot"]);
	const auto& Pos = hws::JsonConvertToVec3(Json["Pos"]);
	const auto& Scale = hws::JsonConvertToVec3(Json["Scale"]);

	// インスタンスを生成
	std::unique_ptr<CObject_X> pObjectX = std::make_unique<CObject_X>(static_cast<CObject::LAYER>(Priority));

	// 生成失敗
	if (!pObjectX)
	{
		return nullptr;
	}

	// モデルを設定
	pObjectX->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey(ModelType));

	// Xオブジェクトの初期設定
	pObjectX->Init();

	// 各種パラメータを設定
	pObjectX->SetRot(Rot);
	pObjectX->SetPos(Pos);
	pObjectX->SetScale(Scale);

	return pObjectX;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 頂点を調整する
//============================================================================
void CObject_X::AdjustVtx()
{
	//最大頂点にスケール値を掛ける
	m_VtxMax.x = m_BaseVtxMax.x * m_Scale.x;
	m_VtxMax.y = m_BaseVtxMax.y * m_Scale.y;
	m_VtxMax.z = m_BaseVtxMax.z * m_Scale.z;

	//最小頂点にスケール値を掛ける
	m_VtxMin.x = m_BaseVtxMin.x * m_Scale.x;
	m_VtxMin.y = m_BaseVtxMin.y * m_Scale.y;
	m_VtxMin.z = m_BaseVtxMin.z * m_Scale.z;

	//サイズ計算
	m_Size = (m_VtxMax - m_VtxMin) * 0.5f;
}

//============================================================================
// 向きを調整する
//============================================================================
void CObject_X::AdjustRotAngle()
{
	while (fabsf(m_Rot.y) > D3DX_PI)
	{
		hws::WrapAngleToRange(m_Rot.y);
	}
}

//============================================================================
// ワールド行列設定
//============================================================================
void CObject_X::UpdateMtxWorld()
{
	// 計算用行列
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// ワールド行列を初期化
	D3DXMatrixIdentity(&m_MtxWorld);
	
	// 拡縮行列作成
	D3DXMatrixScaling(&mtxScale,
		m_Scale.x,
		m_Scale.y,
		m_Scale.z);

	// 拡縮行列との掛け算
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxScale);

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