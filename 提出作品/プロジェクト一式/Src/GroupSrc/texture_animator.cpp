//============================================================================
// 
// テクスチャアニメーター [texture_animator.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "texture_animator.hpp"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//============================================================================
// テクスチャアニメーション
//============================================================================
bool useful::CTextureAnimator::Animate(VERTEX_2D*& refVtx)
{
	// テクスチャ分割の寸法をコピー
	float
		fWidth = m_Data.fTexSplitHeight,
		fHeight = m_Data.fTexSplitHeight;

	// 現在のパターンをコピー
	int
		nNowPatternU = m_Data.nNowPetternU,
		nNowPatternV = m_Data.nNowPetternV;

	// テクスチャ座標の設定
	refVtx[0].tex = { fWidth * nNowPatternU,       fHeight * nNowPatternV };
	refVtx[1].tex = { fWidth * (nNowPatternU + 1), fHeight * nNowPatternV };
	refVtx[2].tex = { fWidth * nNowPatternU,       fHeight * (nNowPatternV + 1) };
	refVtx[3].tex = { fWidth * (nNowPatternU + 1), fHeight * (nNowPatternV + 1) };

	// テクスチャパターンの切り替え
	if (ChangeTexturePattern())
	{
		return true;
	}

	return false;
}

//============================================================================
// テクスチャアニメーション
//============================================================================
bool useful::CTextureAnimator::Animate(VERTEX_3D*& refVtx)
{
	// テクスチャ分割の寸法をコピー
	float
		fWidth = m_Data.fTexSplitHeight,
		fHeight = m_Data.fTexSplitHeight;

	// 現在のパターンをコピー
	int
		nNowPatternU = m_Data.nNowPetternU,
		nNowPatternV = m_Data.nNowPetternV;

	// テクスチャ座標の設定
	refVtx[0].tex = { fWidth * nNowPatternU,       fHeight * nNowPatternV };
	refVtx[1].tex = { fWidth * (nNowPatternU + 1), fHeight * nNowPatternV };
	refVtx[2].tex = { fWidth * nNowPatternU,       fHeight * (nNowPatternV + 1) };
	refVtx[3].tex = { fWidth * (nNowPatternU + 1), fHeight * (nNowPatternV + 1) };

	// テクスチャパターンの切り替え
	if (ChangeTexturePattern())
	{
		return true;
	}

	return false;
}

//============================================================================
// テクスチャパターンの切り替え
//============================================================================
bool useful::CTextureAnimator::ChangeTexturePattern()
{
	// フレームカウンターのインクリメント
	++m_Data.nFrameCounter;

	// 規定値に到達するまで切り替え無し
	if (m_Data.nFrameCounter < m_Data.nChangeRate)
	{
		return false;
	}

	// カウンターはリセット
	m_Data.nFrameCounter = 0;

	// ① U軸のパターン数をインクリメント
	++m_Data.nNowPetternU;

	// ② 右端 (1.0f) に到達したらU軸のみ折り返す
	if (m_Data.fTexSplitWidth * m_Data.nNowPetternU >= 1.0f)
	{
		// (U軸パターンはリセット)
		m_Data.nNowPetternU = 0;

		// ③ V軸のパターン数をインクリメント
		++m_Data.nNowPetternV;
	}

	// ④ 下端に到達したら両軸リセット
	if (m_Data.fTexSplitHeight * m_Data.nNowPetternV >= 1.0f)
	{
		m_Data.nNowPetternU = m_Data.nNowPetternV = 0;

		// アニメーションの1巡完了を通知する
		return true;
	}

	return false;
}