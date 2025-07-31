//============================================================================
// 
// ポップアップオブジェクト、ヘッダファイル [object_PopUp.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_POPUP_H_
#define _OBJECT_POPUP_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_3D.h"

//****************************************************
// 前方宣言
//****************************************************
class CObject_TextMesh;

//****************************************************
// ポップアップオブジェクトクラス
//****************************************************
class CObject_PopUp : public CObject_3D
{
public:

	// <special function>
	CObject_PopUp(LAYER Priority = LAYER::FRONT);	// コンストラクタ
	~CObject_PopUp() override;					// デストラクタ

	// <function>
	HRESULT	Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理
	void	SetDisappear();		// 消滅
	void	AllTargetReset();	// 全目標値リセット

	// <Setter>
	void SetCorrectCoef(float fCorrectionCoef);	// 補間強度を設定

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
	static CObject_PopUp* Create(JSON Json);	// 生成

private:

	// <function>
	void CorrectToTarget();	// 目標値へ補間

	// <data>
	float			  m_fCorrectCoef;	// 補間強度
	D3DXVECTOR3		  m_PosTarget;		// 目標座標
	D3DXVECTOR3		  m_RotTarget;		// 目標向き
	D3DXVECTOR3		  m_SizeTarget;		// 目標サイズ
	D3DXCOLOR		  m_ColTarget;		// 目標色
	CObject_TextMesh* m_pTextMesh;		// テキストメッシュ
};

#endif // _OBJECT_POPUP_H_