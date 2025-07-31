//============================================================================
// 
// ２０２５年７月２４日：チュートリアルを実装する [TutorialState_HandSwordPractice.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _TUTORIALSTATE_HANDSWORDPRACTICE_H_
#define _TUTORIALSTATE_HANDSWORDPRACTICE_H_	

//****************************************************
// インクルード
//****************************************************
#include "TutorialState.h"

//****************************************************
// 前方宣言
//****************************************************
class CButton; // ボタン

//****************************************************
// チュートリアル状態：「突き練習」クラス
//****************************************************
class CTutorialState_HandSwordPractice : public CTutorialState
{
public:
	// コメント種類
	enum class COMMENT
	{
	    INTRO = 0, // 導入
		TAKOSIA,   // たこしあ
		KONOMAMAZYA, // このままじゃあたる
		EXECUTE, // 手刀発動コメント
		SUMMARY, // まとめ
		MAX
	};

	// === 特殊関数 ===

	CTutorialState_HandSwordPractice(CTutorial* pTutorial); // コンストラクタ
	~CTutorialState_HandSwordPractice() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init(CTutorial* pTutorial) override; // 初期化処理
	void Uninit(CTutorial* pTutorial) override;  // 終了処理
	void Update(CTutorial* pTutorial) override; // 更新処理

private:
	// === 静的メンバ変数 ===
	
	// === メンバ変数 ===
	int
		m_nCommentNum, // 現在のコメント番号
		m_nCommentDispFrame; // コメント表示フレーム
	bool
		m_bExecuteHandSword; // 手刀を発動するかどうか
	JSON m_Json;       // ジェイソン
    // === メンバ関数 ===
	void ChangeStrExplanation(CTutorial * pTutorial); // 説明文字列を変更する
};
#endif // _COLLISION_H_