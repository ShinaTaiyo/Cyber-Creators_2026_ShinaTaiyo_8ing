//============================================================================
// 
// 揺れ再生、ヘッダファイル [swaying_player.h]
// Author : 福田歩希
// 
//============================================================================

// インクルードガード
#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "quadratic_bezier.h"
#include "camera_manager.h"

//****************************************************
// 揺れ再生クラスを定義
//****************************************************
class CSwayingPlayer
{
	//****************************************************
	// 静的メンバ定数を宣言
	//****************************************************

	// ビュー上書き用パラメータ
	static const std::array<JSON, 2> s_aOverWriteViewParam;

	//****************************************************
	// 揺れ状態構造体を定義
	//****************************************************
	struct SWAY_STATE
	{
		// デストラクタ
		virtual ~SWAY_STATE() = default;

		// 状態の更新
		static bool UpdateState(CSwayingPlayer*& refSwayingPlayer);

		// 実行
		virtual bool Execute(CSwayingPlayer*& refSwayingPlayer);

		// 変更
		virtual void Change(useful::up<SWAY_STATE>& refState);
	};

	//****************************************************
	// 揺れイントロ状態構造体を定義
	//****************************************************
	struct SWAY_INTRO :public SWAY_STATE
	{
		// デフォルトコンストラクタ
		SWAY_INTRO() = default;

		// デストラクタ
		~SWAY_INTRO() override = default;

		bool Execute(CSwayingPlayer*& refSwayingPlayer) override;
		void Change(useful::up<SWAY_STATE>& refState) override;
	};

	//****************************************************
	// 揺れ再生状態構造体を定義
	//****************************************************
	struct SWAY_PLAY :public SWAY_STATE
	{
		// 最大カウント数
		static constexpr int NUM_MAXCOUNT = 120 - 90;

		// デフォルトコンストラクタ
		SWAY_PLAY() :
			m_Bezier{}
		{}

		// デストラクタ
		~SWAY_PLAY() override
		{
			m_Bezier.Finalize();
		}

		bool Execute(CSwayingPlayer*& refSwayingPlayer) override;
		void Change(useful::up<SWAY_STATE>& refState) override;

		// ベジェ曲線
		CQuadratic_Bezier m_Bezier;
	};

	//****************************************************
	// 揺れアウトロ状態構造体を定義
	//****************************************************
	struct SWAY_OUTRO :public SWAY_STATE
	{
		// デフォルトコンストラクタ
		SWAY_OUTRO() :
			m_Bezier{}
		{}

		// デストラクタ
		~SWAY_OUTRO() override
		{
			m_Bezier.Finalize();
			/* 応急処置 */
			CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game")->OverwriteView(nullptr);
			CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game")->SetUseDecayState(false);
		};

		bool Execute(CSwayingPlayer*& refSwayingPlayer) override;
		void Change(useful::up<SWAY_STATE>& refState) override;

		// ベジェ曲線
		CQuadratic_Bezier m_Bezier;
	};

public:
	
	//****************************************************
	// special function
	//****************************************************
	CSwayingPlayer();	// デフォルトコンストラクタ

	//****************************************************
	// function
	//****************************************************

	/**
	 * 振動を発生させます
	 *
	 * coolTimeCnt -> 召喚クールタイムカウント
	 */
	bool Sway(int coolTimeCnt);

private:

	//****************************************************
	// data
	//****************************************************
	useful::up<SWAY_STATE> m_upState;		// 状態
	CCamera*			   m_pMainCamera;	// メインカメラのポインタ
	int					   m_nCoolTimeCnt;	// クールタイムカウント
};