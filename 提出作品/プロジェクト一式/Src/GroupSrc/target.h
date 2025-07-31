//============================================================================
// 
// ２０２５年５月８日：ターゲットクラスを作成 [Target.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _TARGET_H_
#define _TARGET_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_3D.h" // オブジェクト3D 

//****************************************************
// 前方宣言
//****************************************************
class CTransparent_Info; // 透明度情報

//****************************************************
// プレイヤークラス
//****************************************************
class CTarget : public CObject_3D
{
public:

	// <special function>
	CTarget();				// コンストラクタ
	~CTarget() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <getter/setter>
	const bool& GetUseControl() const; // 操作するかどうか取得
	void SetUseControl(bool bControl); // 操作するかどうか設定

	// <getter>
	CTransparent_Info* GetTransparent_Info() const; // 透明度情報を取得

	// <setter>
	void SetPosTarget(D3DXVECTOR3 Pos); // 目的地を設定

	// 移動させるかどうか
	const bool& GetUseMove() const; // 取得
	void SetUseMove(bool bUse);     // 設定

	// カメラの位置を調整するかどうか
	const bool& GetUseAdjustCameraPos() const; // 取得
	void SetUseAdjustCameraPos(bool bUse); // 設定

	// <static function>
	static CTarget* Create(D3DXVECTOR3 InitPos);	// 生成

private:
	//****************************************************
	// 静的定数を定義
	//****************************************************
	static constexpr float MOVESPEED = 5.0f; // 移動速度
	static constexpr float
		s_fMoveAdjust = 0.2f, // 目的地へ移動させる補正量
		s_fMoveLimitMinX = -235.0f,
		s_fMoveLimitMaxX = 235.0f,
		s_fMoveLimitMinY = -140.0f,
		s_fMoveLimitMaxY = 140.0f;

	// === 変数 ===

	bool m_bControl; // 操作するかどうか
	bool m_bUseMove; // 移動させるかどうか
	bool m_bUseAdjustCameraPos; // カメラの位置を調整するかどうか
	D3DXVECTOR3 m_Move;    // 移動量
	D3DXVECTOR3 m_PosOld;  // 1f前の位置
	D3DXVECTOR3 m_PosTarget; // 狙う位置
	D3DXVECTOR3 m_PosTargetOld; // 1f前の狙う位置
	CTransparent_Info* m_pTransParent_Info; // 透明度情報

	// === メンバ関数 ===

	void Control();  // 操作する
	void LimitRange();   // 画面外にはみ出さないようにする
	void AdjustCameraPos(); // カメラの位置を調整する
};

#endif // _PALYER_H_