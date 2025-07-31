//============================================================================
// 
// プロローグ、ヘッダファイル [prologue.h]
// Author : 福田歩希
// 
//============================================================================

// インクルードガード
#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "scene.h"

//****************************************************
// 前方宣言
//****************************************************
class CObject_HUD;
class CObject_X;
class CMotion_Set;

//****************************************************
// プロローグクラス
//****************************************************
class CPrologue : public CScene
{
public:

	// <function>
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理
	void To_Next() override;	// 次のシーンへ

	// <static function>
	static CPrologue* Create();	// プロローグを生成

private:

	// <special function>
	CPrologue();			// コンストラクタ
	~CPrologue() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	UpdateText();		// テキストの更新処理
	void	UpdatePhase();		// フェーズの更新処理
	void	SetLeaves();		// 大量の葉っぱセット
	void	UpdateLeaves();		// 大量の葉っぱ更新処理

	//****************************************************
	// data
	//****************************************************
	int						m_nPhase;			// イベントフェーズ
	int						m_nFrameCounter;	// 汎用フレームカウター
	CObject_HUD*			m_pText;			// テキスト
	CMotion_Set*			m_pEnemy;			// 敵のモーションセット
	CMotion_Set*			m_pHuman;			// 主人公のモーションセット
	CObject_X*				m_pGold;			// 金インゴット
	std::vector<CObject_X*> m_vpLeaves;			// 大量の葉っぱ
	float					m_fAlphaRate;		// ？
};