//============================================================================
// 
// エフェクトオブジェクト、ヘッダファイル [object_Effect.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_EFFECT_H_
#define _OBJECT_EFFECT_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_Billboard.h"

// テクスチャ割り当て用
#include "texture_manager.h"

//****************************************************
// エフェクトオブジェクトクラス
//****************************************************
class CObject_Effect : public CObject_Billboard
{
public:

	// <special function>
	CObject_Effect(LAYER Priority = LAYER::BG);	// コンストラクタ
	~CObject_Effect() override = 0;				// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <setter>
	void SetCorrectionCoef(float fCoef);	// 補正係数を設定

	// <getter/setter>
	const D3DXVECTOR3& GetSizeTarget() const;	// 目標サイズを取得
	void SetSizeTarget(D3DXVECTOR3 SizeTarget);	// 目標サイズを設定

	// <getter/setter>
	const D3DXVECTOR3& GetRotTarget() const;	// 目標向きを取得
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// 目標向きを設定

	// <getter/setter>
	const D3DXVECTOR3& GetPosTarget() const;	// 目標座標を取得
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// 目標座標を設定

	// <getter/setter>
	const D3DXCOLOR& GetColTarget() const;	// 目標色を取得
	void SetColTarget(D3DXCOLOR ColTarget);	// 目標色を設定

	// <getter/setter>
	const int& GetMaxDuration();	// 最大期間を取得
	void SetMaxDuration(int nMax);	// 最大期間を設定

	// <getter>
	const int& GetDuration();	// 継続期間を取得

private:

	// <function>
	void CountDuration();		// 継続カウント
	void CorrectToTaraget();	// 目標値への補正

	// <data>
	float		m_fCorrectionCoef;	// 補正係数
	D3DXVECTOR3	m_SizeTarget;		// 目標サイズ
	D3DXVECTOR3	m_RotTarget;		// 目標向き
	D3DXVECTOR3	m_PosTarget;		// 目標座標
	D3DXCOLOR	m_ColTarget;		// 目標色
	int			m_nMaxDuration;		// 最大期間
	int			m_nDuration;		// 継続期間
};

#endif // _OBJECT_BILLBOARD_H_