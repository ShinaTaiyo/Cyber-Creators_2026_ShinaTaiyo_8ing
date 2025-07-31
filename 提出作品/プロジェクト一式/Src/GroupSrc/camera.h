//============================================================================
//
// カメラ、ヘッダファイル [camera.h]
// Author : 福田歩希
//
//============================================================================

// インクルードガード
#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "rotator_roll.h"

//****************************************************
// 前方宣言
//****************************************************
class CCamera_State; // カメラ状態

//****************************************************
// カメラクラス
//****************************************************
class CCamera
{
public:

	//****************************************************
	// special function
	//****************************************************
	CCamera();								// デフォルトコンストラクタ
	CCamera(const JSON& json);				// 変換コンストラクタ
	CCamera(const CCamera& obj) = default;	// コピーコンストラクタ
	~CCamera();								// デストラクタ

	//****************************************************
	// function
	//****************************************************
	
	/**
	 * メンバ変数の初期化はイニシャライザーで事足りているため
	 * この初期化処理は現在不使用です
	 */
	[[ deprecated ]] bool Initialize();

	/**
	 * 定点カメラでない場合、メインループで呼び出してください
	 */
	void Update();

	/**
	 * オブジェクトの描画直前に呼び出すことで、カメラをセットします
	 * 
	 * pos - 引数に数値を渡さないでください、{ 0.0f, 0.0f, 0.0f }固定で使用してください
	 */
	void SetCamera(D3DXVECTOR3 pos);

	/**
	 * ビューをまるごと上書きします
	 * 
	 * fp - 上書きしたい処理、nullptr指定でデフォルト挙動に戻ります
	 */
	void OverwriteView(std::function<void(CCamera&)>&& fp);

	/**
	 * 目標値から実値を強制的に少しずらし、振動再現します
	 * 自動補間との併用となるので必要範囲で毎フレーム呼び出してください
	 * 
	 * coef - ずらし強度
	 */
	void SetVibration(float coef);

	// ビュー上書きの使用状態を取得
	inline bool GetStateOverwriteView()
	{
		if (m_fpOverwriteView)
		{
			return true;
		}
		
		return false;
	}

	// 向きの取得 / 設定
	const D3DXVECTOR3& GetRot() const;
	void SetRot(const D3DXVECTOR3& rot);

	// 目標向きの取得 / 設定
	const D3DXVECTOR3& GetRotTarget() const;
	void SetRotTarget(const D3DXVECTOR3& rotTarget);

	// 座標の取得 / 設定
	const D3DXVECTOR3& GetPos() const;
	void SetPos(const D3DXVECTOR3& pos);

	// 目標座標の取得 / 設定
	const D3DXVECTOR3& GetPosTarget() const;
	void SetPosTarget(const D3DXVECTOR3& posTarget);

	// 間距離の取得 / 設定
	float GetDistance() const;
	void SetDistance(float distance);

	// 目標間距離の取得 / 設定
	float GetDistanceTarget() const;
	void SetDistanceTarget(float distanceTarget);

	// 俯瞰度合いの取得 / 設定
	float GetUpAdjust() const;
	void SetUpAdjust(float upAdjust);

	// 目標俯瞰度合いの取得 / 設定
	float GetUpAdjustTarget() const;
	void SetUpAdjustTarget(float upAdjustTarget);

	// 指数減衰の使用状態設定
	void SetUseDecayState(bool state);

	// カメラの視点座標取得
	const D3DXVECTOR3& GetPosV() const; // 取得
	void SetPosV(D3DXVECTOR3 PosV);     // 設定

	// カメラの注視点座標取得
	const D3DXVECTOR3& GetPosR() const;  // 取得
	void SetPosR(D3DXVECTOR3 PosR);      // 設定

	// プロジェクションマトリックスのポインタを取得
	D3DXMATRIX* GetMtxProj();

	// ビューマトリックスのポインタを取得
	D3DXMATRIX* GetMtxView();

	// カメラ状態の変更
	void ChangeState(CCamera_State* state);
	CCamera_State* GetState();

	// Roll軸回転制御ポインタの参照
	inline useful::up<CRotatorRoll>& RefRotatorRoll() { return m_upRotatorRoll; }

	/**
	 * フリーカメラのみの更新を行います
	 */
	static void UpdateFreeCam();

	// フリーカメラの参照
	static inline std::unique_ptr<CCamera>& RefFreeCam() { return s_upFreeCam; }

	// フリーカメラの使用状態取得
	static inline bool GetStateUseFreeCam() { return s_bUseFree; }

	// 最初の距離取得
	static inline float GetInitDistance() { return s_fInitDistance; };

private:

	//****************************************************
	// 型エイリアスを定義
	//****************************************************
	using upState = std::unique_ptr<CCamera_State>();

	//****************************************************
	// 静的メンバ定数を定義
	//****************************************************
	static constexpr float COEF_ADJUST = 0.05f;
	static constexpr float s_fInitDistance = 400.0f; // 最初の距離

	//****************************************************
	// function
	//****************************************************
	void Control();						// カメラ操作処理
	void ControlImGui();				// ImGuiによる操作
	void ControlFly();					// フライ操作
	void ControlOrbit();				// オービット操作
	void CorrectToTarget();				// 目標値への補間
	void AutoRestrictPitch();			// ピッチ角の範囲制限
	void CameraTransformFly();			// フライ操作でのカメラ適応
	void CameraTransformOrbit();		// オービット操作でのカメラ適応
	void CalcMtxProjection();			// プロジェクション行列計算
	void CalcMtxView(D3DXVECTOR3 pos);	// ビュー行列計算
	void PrintDebug();					// デバッグ表示
	
	//****************************************************
	// data
	//****************************************************
	
	// 強制ビュー上書き
	std::function<void(CCamera&)> m_fpOverwriteView;
	CCamera_State*				  m_pState;

	// カメラパラメータ
	D3DXVECTOR3	m_Rot;				// 向き
	D3DXVECTOR3	m_RotTarget;		// 目標向き
	D3DXVECTOR3	m_Pos;				// 座標
	D3DXVECTOR3	m_PosTarget;		// 目標座標
	D3DXVECTOR3	m_PosV;				// 視点座標
	D3DXVECTOR3	m_PosTargetV;		// 目標視点座標
	D3DXVECTOR3	m_PosR;				// 注視点座標
	D3DXVECTOR3	m_PosTargetR;		// 目標注視点座標
	float		m_fDistance;		// 視点 -> 注視点間の距離
	float		m_fDistanceTarget;	// 目標間距離
	float		m_fUpAdjust;		// 俯瞰度合い
	float		m_fUpAdjustTarget;	// 目標俯瞰度合い
	D3DXVECTOR3	m_VecU;				// 上方向ベクトル
	D3DXMATRIX	m_MtxProjection;	// プロジェクション行列
	D3DXMATRIX	m_MtxView;			// ビュー行列
	bool		m_bNowUse;			// 現在使用しているカメラかどうか
	bool		m_bNowUseOld;		// 1f前に使用していたかどうか

	// Roll軸回転制御処理合成先
	useful::up<CRotatorRoll> m_upRotatorRoll;
	
	// ビュー状態
	bool  m_bUseFirstPerson;	// 一人称視点の使用状態 (falseの場合は三人称視点)
	bool  m_bUseDecay;			// 指数減衰の使用状態
	bool  m_bUseControlDrag;	// ドラッグ操作の使用状態
	bool  m_bUseHorizon;		// 水平ビューの使用状態
	bool  m_bUseVertical;		// 垂直ビューの使用状態
	float m_fDragMoveSpeed;		// ドラッグ操作時の移動速度
	float m_fDragRotateSpeed;	// ドラッグ操作時の回転速度

	// フリーカメラ情報
	static bool s_bUseFree;							// 使用状態
	static std::unique_ptr<CCamera> s_upFreeCam;	// カメラのインスタンス
};