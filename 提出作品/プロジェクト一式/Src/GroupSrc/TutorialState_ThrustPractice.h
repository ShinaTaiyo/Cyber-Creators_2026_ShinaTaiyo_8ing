//============================================================================
// 
// ２０２５年７月２４日：チュートリアルを実装する [TutorialState_ThrustPractice.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _TUTORIALSTATE_THRUSTPRACTICE_H_
#define _TUTORIALSTATE_THRUSTPRACTICE_H_	

//****************************************************
// インクルード
//****************************************************
#include "TutorialState.h"

//****************************************************
// チュートリアル状態：「突き練習」クラス
//****************************************************
class CTutorialState_ThrustPractice : public CTutorialState
{
public:
	// コメント種類
	enum class COMMENT
	{
		GOOD = 0,
		BAD,
		MAX
	};

	// === 特殊関数 ===

	CTutorialState_ThrustPractice(CTutorial* pTutorial); // コンストラクタ
	~CTutorialState_ThrustPractice() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init(CTutorial* pTutorial) override; // 初期化処理
	void Uninit(CTutorial* pTutorial) override;  // 終了処理
	void Update(CTutorial* pTutorial) override; // 更新処理

private:
	// === 静的メンバ変数 ===

	static constexpr int
		s_nGoalSuccessNum = 5, // 目標突き成功回数
		s_nInitNextSceneCntDown = 120; // 次のシーンまでのカウントダウン初期値

	// === メンバ変数 ===
	int
		m_nSuccessNum, // 成功回数
		m_nNextSceneCntDown; // 次のシーンまでのカウントダウン
	bool
		m_bStartPhaseOld, // 1f前にフェーズが開始しているかどうか
		m_bStartPhase;    // フェーズが開始しているかどうか
	JSON m_Json;     // ジェイソン

	// === メンバ関数 ===

	void ExecuteReaction(); // 行動に対してリアクションを発動する
};
#endif // _COLLISION_H_