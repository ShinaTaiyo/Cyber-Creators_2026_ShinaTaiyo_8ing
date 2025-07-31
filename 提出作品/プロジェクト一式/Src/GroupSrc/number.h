//============================================================================
// 
// ２０２５年５月１２日：タイマーによるシーン遷移実装 [number.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _NUMBER_H_
#define _NUMBER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_2D.h"

//****************************************************
// プレイヤークラス
//****************************************************
class CNumber : public CObject_2D
{
public:
	// <special function>
	CNumber();				// コンストラクタ
	~CNumber() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <static function>
	static CNumber* Create(D3DXVECTOR3 InitPos);	// 生成
private:
};

#endif // _PALYER_H_