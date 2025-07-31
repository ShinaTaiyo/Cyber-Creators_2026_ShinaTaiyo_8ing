//============================================================================
// 
// ２０２５年７月２２日：チュートリアルを実装する [TutorialState_GamePurpose.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _TUTORIALSTATE_GAMEPURPOSE_H_
#define _TUTORIALSTATE_GAMEPURPOSE_H_	

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
// チュートリアル状態：「ゲーム説明」クラス
//****************************************************
class CTutorialState_GamePurpose : public CTutorialState
{
public:

	// 説明種類
	enum class EXPLANATION
	{
		PURPOSE = 0,
		SUPPORT,
		MAX
	};

	// === 特殊関数 ===

	CTutorialState_GamePurpose(CTutorial * pTutorial); // コンストラクタ
	~CTutorialState_GamePurpose() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init(CTutorial* pTutorial) override; // 初期化処理
	void Uninit(CTutorial* pTutorial) override;  // 終了処理
    void Update(CTutorial* pTutorial) override;  // 更新処理

private:
	// === 静的メンバ変数 ===
	static const std::string s_StrExplanation[static_cast<int>(EXPLANATION::MAX)];

	// === メンバ変数 ===
	int
		m_nChooseExplanation; // 説明番号
	CButton* m_pButton; // ボタンへのポインタ
	// === メンバ関数 ===
	void ChangeStrExplanation(CTutorial * pTutorial); // 説明文字列を変更する

};
#endif // _COLLISION_H_