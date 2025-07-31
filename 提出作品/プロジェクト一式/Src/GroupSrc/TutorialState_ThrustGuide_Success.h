//============================================================================
// 
// ２０２５年７月２２日：チュートリアルを実装する [TutorialState_ThrustGuide_Success.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _TUTORIALSTATE_THRUSTGUIDE_SUCCESS_H_
#define _TUTORIALSTATE_THRUSTGUIDE_SUCCESS_H_	

//****************************************************
// インクルード
//****************************************************
#include "TutorialState.h"

//****************************************************
// 静的メンバ宣言
//****************************************************
class CButton; // ボタン

//****************************************************
// usingディレクティブ
//****************************************************

//****************************************************
// チュートリアル状態：「手刀ガイド成功」クラス
//****************************************************
class CTutorialState_ThrustGuide_Success : public CTutorialState
{
public:

	// 説明種類
	enum class EXPLANATION
	{
		START = 0,
		DEMO_1,
		DEMO_2,
		MAX
	};

	// === 特殊関数 ===

	CTutorialState_ThrustGuide_Success(CTutorial* pTutorial); // コンストラクタ
	~CTutorialState_ThrustGuide_Success() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init(CTutorial* pTutorial) override; // 初期化処理
	void Uninit(CTutorial* pTutorial) override;  // 終了処理
	void Update(CTutorial* pTutorial) override; // 更新処理

private:
	// === メンバ変数 ===
	int
		m_nChooseExplanation, // 説明番号
		m_nCntFrame; // フレーム数カウント
	bool m_bMotion; // モーションするかどうか
	JSON m_JsonExplanation; // 説明ジェイソンファイル
	// === メンバ関数 ===
	bool ChangeStrExplanation(CTutorial* pTutorial); // 説明文字列を変更する

};
#endif // _COLLISION_H_