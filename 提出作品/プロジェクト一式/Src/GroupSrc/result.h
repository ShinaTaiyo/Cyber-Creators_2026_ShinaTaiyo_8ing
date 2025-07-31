//============================================================================
// 
// リザルト、ヘッダファイル [result.h]
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
// リザルトクラス
//****************************************************
class CResult final : public CScene
{
public:

	/**
	 * リザルト用の更新処理です、
	 * シーンマネージャーの更新で自動的に呼び出されます
	 */
	void Update() override;

	/**
	 * リザルト用の描画処理です、
	 * シーンマネージャーの描画で自動的に呼び出されます
	 */
	void Draw() override;

	/**
	 * 更新時にこの関数を呼ぶと、設定されている次のシーンに遷移します
	 * 基本的にフレーム更新の最後に呼び出してください
	 */
	void To_Next() override;

	/**
	 * リザルトシーンのインスタンスを生成します
	 */
	static CResult* Create();

	// インゲームの連続成功回数の引継ぎ
	inline void SetPhaseComboCnt(int count) { m_nPheseComboCntKeep = count; }

	// 敵モーションの引継ぎ
	inline void SetEnemy(CMotion_Set* p) { m_pEnemy = p; }

	// 疑似床モーションの引継ぎ (解放用のみ)
	inline void SetFakeField(CMotion_Set* p) { m_pFakeField = p; }

	// 山オブジェクトの引継ぎ
	void SetMountain(CObject_X* p);

private:

	//****************************************************
	// special function
	//****************************************************
	CResult();				// デフォルトコンストラクタ
	~CResult() override;	// デストラクタ

	//****************************************************
	// function
	//****************************************************
	HRESULT Init() override;	// 初期化処理
	void	Uninit() override;	// 終了処理
	void	UpdatePhase();		// フェーズの更新処理

	// 溜め解放に合わせてカメラをセット
	void SetCamImpact(const char* key);

	//****************************************************
	// data
	//****************************************************
	int m_nPheseComboCntKeep;		// インゲームの連続成功回数
	CObject_HUD* m_pAtk;			// 攻撃催促
	CObject_HUD* m_pTY;				// 感謝
	CMotion_Set* m_pEnemy;			// 敵モーション
	CMotion_Set* m_pHuman;			// 主人公モーション
	CMotion_Set* m_pFakeField;		// 疑似床モーション
	CObject_X* m_pMountain;			// 山モーション
	CMotion_Set* m_pMountBreakable;	// 山(可壊)モーション

	int	m_nPhase;			// イベントフェーズ
	int	m_nFrameCounter;	// 汎用フレームカウター
};