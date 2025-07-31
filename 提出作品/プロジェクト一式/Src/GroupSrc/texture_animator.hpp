//============================================================================
// 
// テクスチャアニメーター、ヘッダファイル [texture_animator.hpp]
// Author : 福田歩希
// 
//============================================================================

// インクルードガード
#pragma once

//****************************************************
// 名前空間usefulを定義
//****************************************************
namespace useful
{
	//****************************************************
	// テクスチャアニメータークラスを定義
	//****************************************************
	class CTextureAnimator
	{
		//****************************************************
		// テクスチャアニメーション用情報の構造体を定義
		//****************************************************
		struct TEXTURE_ANIMATION_DATA
		{
			// デフォルトコンストラクタ
			TEXTURE_ANIMATION_DATA() :
				nChangeRate(0),
				nFrameCounter(0),
				nNowPetternU(0),
				nNowPetternV(0),
				fTexSplitWidth(0.0f),
				fTexSplitHeight(0.0f)
			{}

			int
				nChangeRate,	// 切り替え速度
				nFrameCounter,	// フレームカウンター
				nNowPetternU,	// U軸の現在のパターン数
				nNowPetternV;	// V軸の現在のパターン数

			// テクスチャ分割の寸法
			float
				fTexSplitWidth,		// 横
				fTexSplitHeight;	// 縦
		};

	public:

		//****************************************************
		// function
		//****************************************************

		/**
		 * テクスチャアニメーションを行います
		 * 返り値はアニメーションの完了を通知します
		 *  -> 通常更新時はfalse
		 *  -> 1巡完了時のみtrue
		 *
		 * refVtx -> 頂点フォーマットのポインタ参照、必ずロックされた直後のポインタを渡してください
		 */
		bool Animate(VERTEX_2D*& refVtx);
		bool Animate(VERTEX_3D*& refVtx);

		// 切り替え速度の設定
		inline void SetChangeRate(int rate) { m_Data.nChangeRate = rate; }

		// テクスチャ分割の寸法の設定
		inline void SetTexSplitWidth (float texSplitWidth)  { m_Data.fTexSplitWidth = texSplitWidth; }
		inline void SetTexSplitHeight(float texSplitHeight) { m_Data.fTexSplitHeight = texSplitHeight; }

	private:

		//****************************************************
		// funciton
		//****************************************************
		bool ChangeTexturePattern();	// テクスチャパターンの切り替え

		//****************************************************
		// data
		//****************************************************
		TEXTURE_ANIMATION_DATA m_Data;	// テクスチャアニメーション用情報
	};
}