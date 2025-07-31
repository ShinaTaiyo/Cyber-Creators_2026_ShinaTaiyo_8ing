//============================================================================
//
// 二次ベジェ曲線、ヘッダファイル [quadratic_bezier.h]
// Author : 福田歩希
//
//============================================================================

// インクルードガード
#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"

//****************************************************
// 二次ベジェ曲線クラスの定義
//****************************************************
class CQuadratic_Bezier
{
	//****************************************************
	// 型エイリアスの定義
	//****************************************************
	using LINE_VTX = LPDIRECT3DVERTEXBUFFER9;

	//****************************************************
	// 静的メンバ定数の定義 (非公開)
	//****************************************************
	static constexpr unsigned int NUM_TRACE = 3;	// 移動点の数
	static constexpr float DEFAULT_SPEED = 0.005f;	// デフォルト移動速度

public:

	//****************************************************
	// 静的メンバ定数の定義 (公開)
	//****************************************************
	static constexpr int NUM_CONTROL = 3;	// 制御点の数

	//****************************************************
	// special function
	//****************************************************
	CQuadratic_Bezier();	// デフォルトコンストラクタ
	~CQuadratic_Bezier();	// デストラクタ

	//****************************************************
	// function
	//****************************************************

	/**
	 * 制御点間を結ぶ直線を描画するための頂点バッファの生成と、制御点 / 移動点のモデル生成を行います
	 */
	bool Initialize();

	/**
	 * 頂点バッファの破棄と、見た目用モデルへの破棄予約を行います
	 */
	void Finalize();

	/**
	 * 移動点の座標更新を行います
	 * 終端に到達した際に、trueを返します
	 */
	bool Update();

	/**
	 * 制御点間を結ぶ直線の表示と、制御点 / 移動点のモデルの座標設定を行います
	 * 初回移動時に限り、軌跡モデルの生成も行います
	 */
	void Draw();

	// 選択したインデックスの制御点の座標を設定
	inline void SetPosToControlPointForSelectedIdx(const D3DXVECTOR3& pos, int idx) { m_ControlPoint[idx] = pos; }

	// 二次軌跡上にある移動点のポインタを取得
	[[ deprecated ]] inline CObject_X* GetPtrTracePointOnTrajectoryLine() const { return m_pTracePointModel[2]; }

	// 二次軌跡上にある移動点の座標を取得
	inline const D3DXVECTOR3& GetPosTracePointOnTrajectoryLine() const { return m_TracePoint[2]; }

	// 進行速度を設定
	inline void SetSpeed(float speed) { m_fSpeed = speed; }

	// 反転使用フラグの状態の取得 / 設定
	inline bool GetUseReverse() const { return m_bUseReverse; }
	inline void SetUseReverse(bool state) { m_bUseReverse = state; }

private:

	//****************************************************
	// function
	//****************************************************
	HRESULT CreateVtxBuff();			// 頂点バッファを生成
	void	SetReleaseAllTrajectory();	// 二次軌跡用の全てのモデルに破棄予約
	bool	UpdateRate();				// 進行度の更新
	void	UpdateVtxPos();				// 頂点座標の更新

	//****************************************************
	// data
	//****************************************************
	D3DXVECTOR3	m_ControlPoint[NUM_CONTROL];	// 制御点座標
	D3DXVECTOR3	m_TracePoint[NUM_TRACE];		// 移動点座標
	float		m_fRate;						// 進行度
	float		m_fSpeed;						// 進行速度
	bool		m_bUseReverse;					// 反転使用フラグ
	bool		m_bIsReverse;					// 反転フラグ
	bool		m_bIsEndFirstTime;				// 初回移動終了フラグ
	bool		m_bDispModel;					// モデル表示フラグ

	// 見た目用
	D3DXMATRIX m_MtxWorld;							// ワールド行列
	LINE_VTX   m_pLineVtx;							// 頂点バッファ
	CObject_X* m_pControlPointModel[NUM_CONTROL];	// 制御点モデル
	CObject_X* m_pTracePointModel[NUM_TRACE];		// 移動点モデル
	std::vector<CObject_X*> m_vTrajectoryModel;		// 二次軌跡モデル
};