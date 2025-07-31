//============================================================================
// 
// HUDオブジェクト、ヘッダファイル [object_HUD.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_HUD_H_
#define _OBJECT_HUD_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_2D.h"

//****************************************************
// HUDオブジェクトクラス
//****************************************************
class CObject_HUD : public CObject_2D
{
public:

	// <special function>
	CObject_HUD(LAYER Priority = LAYER::UI);	// コンストラクタ
	~CObject_HUD() override;					// デストラクタ

	// <function>
	HRESULT	Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理
	void	SetVibration();		// 振動を与える
	void	SetWaving();		// 波打ちを与える

	// <Setter>
	void SetCorrectionCoef(float fCorrectionCoef);	// 補正係数設定

	// <getter/setter>
	const D3DXVECTOR3& GetPosTarget() const;	// 目標座標取得
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// 目標座標設定

	// <getter/setter>
	const D3DXVECTOR3& GetRotTarget() const;	// 目標向き取得
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// 目標向き設定

	// <getter/setter>
	const D3DXVECTOR3& GetSizeTarget() const;	// 目標サイズ取得
	void SetSizeTarget(D3DXVECTOR3 SizeTarget);	// 目標サイズ設定

	// <getter/setter>
	const D3DXCOLOR& GetColTarget() const;	// 目標色取得
	void SetColTarget(D3DXCOLOR ColTarget);	// 目標色設定

	// <static function>
	static CObject_HUD* Create(JSON Json);	// 生成

private:

	// <function>
	void CorrectToTarget();	// 目標値へ補正

	// <data>
	float		m_fCorrectionCoef;	// 補正係数
	D3DXVECTOR3	m_PosTarget;		// 目標座標
	D3DXVECTOR3	m_RotTarget;		// 目標向き
	D3DXVECTOR3 m_SizeTarget;		// 目標サイズ
	D3DXCOLOR	m_ColTarget;		// 目標色
};

#endif // _OBJECT_HUD_H_