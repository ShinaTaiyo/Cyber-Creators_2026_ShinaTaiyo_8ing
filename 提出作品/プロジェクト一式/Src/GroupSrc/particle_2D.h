//============================================================================
// 
// ２０２５年５月１２日：タイマーによるシーン遷移実装 [particle_2D.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _PARTICLE_2D_H_
#define _PARTICLE_2D_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_2D.h"

//****************************************************
// 前方宣言
//****************************************************
class CMove_Info; // 移動情報

//****************************************************
// プレイヤークラス
//****************************************************
class CParticle_2D : public CObject_2D
{
public:
	// <special function>
	CParticle_2D();				// コンストラクタ
	~CParticle_2D() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <getter/setter>
	const int& GetLife() const; // 体力取得
	void SetLife(int nLife);    // 体力設定

	// <getter/setter>
	const int& GetMaxLife() const; // 最大体力取得
	void SetMaxLife(int nMaxLife); // 最大体力設定

	// <getter/setter>
	const D3DXVECTOR3& GetInitSize() const; // 最初のサイズ取得
	void SetInitSize(D3DXVECTOR3 InitSize); // 最初のサイズ設定

	// <getter>
	CMove_Info* GetMoveInfo() const; // 移動情報を取得する

	// <static function>
	static CParticle_2D* Create(D3DXCOLOR Col,D3DXVECTOR3 InitPos);	// 生成
private:
	D3DXVECTOR3 m_InitSize;      // 最初のサイズ
	CMove_Info* m_pMove_Info;    // 移動情報
	int m_nLife;                 // 体力
	int m_nMaxLife;              // 最大体力
};

#endif // _PALYER_H_