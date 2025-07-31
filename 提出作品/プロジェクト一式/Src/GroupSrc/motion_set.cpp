//============================================================================
// 
// モーションセット [motion_set.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "motion_set.h"
#include "object_Parts.h"

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
CMotion_Set::CMotion_Set(LAYER Layer) :
	CObject{ Layer },
	m_Rot{ VEC3_INIT },
	m_Pos{ VEC3_INIT },
	m_bStop{ false },
	m_wNowMotion{ 0 },
	m_wNowKey{ 0 },
	m_wNowFrame{ 0 },
	m_vpModelParts{},
	m_pMostParent{ nullptr },
	m_wMaxParts{ 0 },
	m_wMaxMotion{ 0 },
	m_vpMotion{}
{
	m_vpModelParts.clear();
	m_vpMotion.clear();
}

//============================================================================
// デストラクタ
//============================================================================
CMotion_Set::~CMotion_Set()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CMotion_Set::Init()
{
	/* 抽象化対策 */

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CMotion_Set::Uninit()
{
	for (WORD wCntMotion = 0; wCntMotion < m_wMaxMotion; ++wCntMotion)
	{
		// モーション情報のポインタを作成
		CMotion_Set::Motion* const pMotion = &m_vpMotion[wCntMotion];

		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// キー情報のポインタを作成
			CMotion_Set::Key* const pKey = &pMotion->vpKey[wCntMotionKey];

			// 目標値情報のポインタ配列を破棄
			if (pKey->apDest != nullptr)
			{
				delete[] pKey->apDest;
				pKey->apDest = nullptr;
			}
		}
	}

	for (WORD wCntMotion = 0; wCntMotion < m_wMaxMotion; ++wCntMotion)
	{
		// モーションごとにキー情報ベクターのクリア
		m_vpMotion[wCntMotion].vpKey.clear();
	}

	// モーション情報のベクターのクリア
	m_vpMotion.clear();

	// パーツ用オブジェクトの消去
	for (auto it : m_vpModelParts)
	{
		// 破棄予約
		it->SetRelease();
	}

	// モデルパーツベクターのクリア
	m_vpModelParts.clear();

	// 最後にこのモーションセットを破棄予約
	SetRelease();
}

//============================================================================
// 更新処理
//============================================================================
void CMotion_Set::Update()
{
	// 動作
	Animation();

	// 向き・座標の情報をパーツにシンクロさせる
	SetSyncRot(m_Rot);
	SetSyncPos(m_Pos);
}

//============================================================================
// 描画処理
//============================================================================
void CMotion_Set::Draw()
{
	/* 抽象化対策 */
}

//============================================================================
// シンクロ向きを反映
//============================================================================
void CMotion_Set::SetSyncRot(D3DXVECTOR3 Rot)
{
	// シンクロ向きを親パーツに反映
	m_pMostParent->SetSyncRot(Rot);
}

//============================================================================
// シンクロ座標を反映
//============================================================================
void CMotion_Set::SetSyncPos(D3DXVECTOR3 Pos)
{
	// シンクロ座標を親パーツに反映
	m_pMostParent->SetSyncPos(Pos);
}

//============================================================================
// 向きを取得
//============================================================================
D3DXVECTOR3 CMotion_Set::GetRot() const
{
	return m_Rot;
}

//============================================================================
// 向きを設定
//============================================================================
void CMotion_Set::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// 座標を取得
//============================================================================
D3DXVECTOR3 CMotion_Set::GetPos() const
{
	return m_Pos;
}

//============================================================================
// 座標を設定
//============================================================================
void CMotion_Set::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// 再生中のモーション番号を取得
//============================================================================
WORD CMotion_Set::GetNowMotion() const
{
	return m_wNowMotion;
}

//============================================================================
// 再生中のモーション番号を設定
//============================================================================
void CMotion_Set::SetNowMotion(WORD nIdx)
{
	m_bStop = false;
	m_wNowMotion = nIdx;
	m_wNowKey = 0;
	m_wNowFrame = 0;
}

//============================================================================
// ストップフラグを取得
//============================================================================
bool CMotion_Set::GetStopState() const
{
	return m_bStop;
}

//============================================================================
// 親パーツを取得
//============================================================================
CObject_Parts* CMotion_Set::GetParentParts()
{
	return m_pMostParent;
}

//============================================================================
// 生成
//============================================================================
CMotion_Set* CMotion_Set::Create(LAYER Layer, TYPE Type, JSON Json)
{
	// モーションセットを生成
	CMotion_Set* pNew = DBG_NEW CMotion_Set(Layer);

	// 生成失敗
	if (!pNew)
	{
		return nullptr;
	}

	// 総パーツ数を取得
	pNew->m_wMaxParts = static_cast<WORD>(Json["MaxParts"]);

	// パーツ数分のパーツオブジェクトを先行して生成
	for (WORD wCntParts = 0; wCntParts < pNew->m_wMaxParts; ++wCntParts)
	{
		pNew->m_vpModelParts.push_back(CObject_Parts::Create(Layer, static_cast<std::string>(Json["ModelType"][wCntParts]), nullptr));

		// 各パーツオブジェクトにタイプを設定する
		pNew->m_vpModelParts[wCntParts]->SetType(Type);

		/* 応急処置 */
		pNew->m_vpModelParts[wCntParts]->SetPos({ -1000.0f, -1000.0f, -1000.0f });
		pNew->m_vpModelParts[wCntParts]->Update();
	}

	// 生成されたパーツに対し、各種設定を行う
	for (WORD wCntParts = 0; wCntParts < pNew->m_wMaxParts; ++wCntParts)
	{
		// 親パーツのインデックス番号を取得
		const SHORT& shParentIdx = static_cast<SHORT>(Json["ParentIdx"][wCntParts]);

		// パーツのポインタをコピー
		CObject_Parts* pParts = pNew->m_vpModelParts[wCntParts];

		if (shParentIdx == -1)
		{
			// インデックスが-1のものは親を持たない
			pParts->SetParent(nullptr);

			// 親にのみ縮尺オフセットを反映
			pParts->SetScaleOffset(hws::JsonConvertToVec3(Json["ScaleOffset"]));

			// 親パーツをセット
			pNew->m_pMostParent = pParts;
		}
		else
		{
			// 親パーツのポインタを渡す
			pParts->SetParent(pNew->m_vpModelParts[shParentIdx]);
		}

		// オフセット値を設定
		pParts->SetRotOffset(hws::JsonConvertToVec3(Json["RotOffset"][wCntParts]));
		pParts->SetPosOffset(hws::JsonConvertToVec3(Json["PosOffset"][wCntParts]));
	}

	// 総モーション数を取得
	pNew->m_wMaxMotion = static_cast<WORD>(Json["MaxMotion"]);

	// モーション数分のモーション情報を生成
	pNew->m_vpMotion.resize(pNew->m_wMaxMotion);

	// モーション情報の設定
	for (WORD wCntMotion = 0; wCntMotion < pNew->m_wMaxMotion; ++wCntMotion)
	{
		// モーション情報のポインタを作成
		Motion* const pMotion = &pNew->m_vpMotion[wCntMotion];

		// ループフラグを取得
		pMotion->bLoop = static_cast<bool>(Json["Loop"][wCntMotion]);

		// モーションの総キー数を取得
		pMotion->wMaxKey = static_cast<WORD>(Json["MaxKey"][wCntMotion]);

		// キー情報の設定
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// キー情報オブジェクトを作成
			Key Key;

			// キーの総フレーム数を取得
			Key.nMaxFrame = static_cast<WORD>(Json["MaxFrame"][wCntMotion][wCntMotionKey]);

			// パーツ数分の目標値情報を生成
			Key.apDest = DBG_NEW CMotion_Set::KeyDest[pNew->m_wMaxParts];

			for (WORD wCntModelParts = 0; wCntModelParts < pNew->m_wMaxParts; ++wCntModelParts)
			{
				// 目標値情報のポインタを作成
				KeyDest* const pDest = &Key.apDest[wCntModelParts];

				// 各種パラメータを設定
				pDest->ScaleTarget = hws::JsonConvertToVec3(Json["ScaleTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);	// 目標縮尺
				pDest->RotTarget = hws::JsonConvertToVec3(Json["RotTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);		// 目標向き
				pDest->PosTarget = hws::JsonConvertToVec3(Json["PosTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);		// 目標座標
			}

			pMotion->vpKey.push_back(Key);
		}
	}

	/* モーションセットそのもののタイプ指定 */
	pNew->SetType(Type);

	return pNew;
}

//============================================================================
// 
// protectedメンバ
// 
//============================================================================

//============================================================================
// モーションを設定
//============================================================================
void CMotion_Set::SetMotion(LAYER Layer, JSON Json)
{
	// 総パーツ数を取得
	m_wMaxParts = static_cast<WORD>(Json["MaxParts"]);

	// パーツ数分のパーツオブジェクトを先行して生成
	for (WORD wCntParts = 0; wCntParts < m_wMaxParts; ++wCntParts)
	{
		m_vpModelParts.push_back(CObject_Parts::Create(Layer, static_cast<std::string>(Json["ModelType"][wCntParts]), nullptr));
	}

	// 生成されたパーツに対し、各種設定を行う
	for (WORD wCntParts = 0; wCntParts < m_wMaxParts; ++wCntParts)
	{
		// 親パーツのインデックス番号を取得
		const SHORT& shParentIdx = static_cast<SHORT>(Json["ParentIdx"][wCntParts]);

		// パーツのポインタをコピー
		CObject_Parts* pParts = m_vpModelParts[wCntParts];

		if (shParentIdx == -1)
		{
			// インデックスが-1のものは親を持たない
			pParts->SetParent(nullptr);

			// 親にのみ縮尺オフセットを反映
			pParts->SetScaleOffset(hws::JsonConvertToVec3(Json["ScaleOffset"]));

			// 親パーツをセット
			m_pMostParent = pParts;
		}
		else
		{
			// 親パーツのポインタを渡す
			pParts->SetParent(m_vpModelParts[shParentIdx]);
		}

		// オフセット値を設定
		pParts->SetRotOffset(hws::JsonConvertToVec3(Json["RotOffset"][wCntParts]));
		pParts->SetPosOffset(hws::JsonConvertToVec3(Json["PosOffset"][wCntParts]));
	}

	// 総モーション数を取得
	m_wMaxMotion = static_cast<WORD>(Json["MaxMotion"]);

	// モーション数分のモーション情報を生成
	m_vpMotion.resize(m_wMaxMotion);

	// モーション情報の設定
	for (WORD wCntMotion = 0; wCntMotion < m_wMaxMotion; ++wCntMotion)
	{
		// モーション情報のポインタを作成
		Motion* const pMotion = &m_vpMotion[wCntMotion];

		// ループフラグを取得
		pMotion->bLoop = static_cast<bool>(Json["Loop"][wCntMotion]);

		// モーションの総キー数を取得
		pMotion->wMaxKey = static_cast<WORD>(Json["MaxKey"][wCntMotion]);

		// キー情報の設定
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// キー情報オブジェクトを作成
			Key Key;

			// キーの総フレーム数を取得
			Key.nMaxFrame = static_cast<WORD>(Json["MaxFrame"][wCntMotion][wCntMotionKey]);

			// パーツ数分の目標値情報を生成
			Key.apDest = DBG_NEW CMotion_Set::KeyDest[m_wMaxParts];

			for (WORD wCntModelParts = 0; wCntModelParts < m_wMaxParts; ++wCntModelParts)
			{
				// 目標値情報のポインタを作成
				KeyDest* const pDest = &Key.apDest[wCntModelParts];

				// 各種パラメータを設定
				pDest->ScaleTarget = hws::JsonConvertToVec3(Json["ScaleTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);	// 目標縮尺
				pDest->RotTarget = hws::JsonConvertToVec3(Json["RotTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);		// 目標向き
				pDest->PosTarget = hws::JsonConvertToVec3(Json["PosTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);		// 目標座標
			}

			pMotion->vpKey.push_back(Key);
		}
	}
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 動作
//============================================================================
void CMotion_Set::Animation()
{
	// フレームカウント
	CountFrame();

	// 目標値への補正
	CorrectTarget();
}

//============================================================================
// フレームカウント
//============================================================================
void CMotion_Set::CountFrame()
{
	// ストップ中なら処理しない
	if (m_bStop)
		return;

	// 現在のフレーム数をインクリメント
	m_wNowFrame++;

	// フレーム数が、現在再生中のキーの総フレーム数に達したら
	if (m_wNowFrame >= GetNowKey()->nMaxFrame)
	{
		// 現在のフレーム数をリセット
		m_wNowFrame = 0;

		// 現在のキー数をインクリメント
		m_wNowKey++;

		// キー数が、現在再生中のモーションの総キー数に達したら
		if (m_wNowKey >= GetNowMotionPtr()->wMaxKey)
		{
			// 現在のキー数をリセット
			m_wNowKey = 0;

			// ループしないモーションなら
			if (GetNowMotionPtr()->bLoop == false)
				m_bStop = true;
		}
	}
}

//============================================================================
// 目標値への補正
//============================================================================
void CMotion_Set::CorrectTarget()
{
	// ストップ中なら処理しない
	if (m_bStop)
		return;

	// フレームの進行度合を作成 (総フレーム数 - 現在のフレーム)
	WORD wFrameCoef = static_cast<WORD>(m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].nMaxFrame - m_wNowFrame);

	// 親パーツ以外がそれぞれの目標値へ補正したパラメータを設定する
	for (WORD wCntModelParts = 0; wCntModelParts < m_wMaxParts; ++wCntModelParts)
	{
		// 目標縮尺
		Vec3 NewScale = m_vpModelParts[wCntModelParts]->GetScale();
		NewScale += (m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].apDest[wCntModelParts].ScaleTarget - NewScale) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetScale(NewScale);

		// 目標向き
		Vec3 NewRot = m_vpModelParts[wCntModelParts]->GetRot();
		NewRot += (m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].apDest[wCntModelParts].RotTarget - NewRot) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetRot(NewRot);

		// 目標座標
		Vec3 NewPos = m_vpModelParts[wCntModelParts]->GetPos();
		NewPos += (m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].apDest[wCntModelParts].PosTarget - NewPos) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetPos(NewPos);
	}
}

//============================================================================
// 現在のモーションのポインタを取得
//============================================================================
CMotion_Set::Motion* const CMotion_Set::GetNowMotionPtr()
{
	return &m_vpMotion[m_wNowMotion];
}

//============================================================================
// 現在のキーのポインタを取得
//============================================================================
CMotion_Set::Key* const CMotion_Set::GetNowKey()
{
	return &GetNowMotionPtr()->vpKey[m_wNowKey];
}