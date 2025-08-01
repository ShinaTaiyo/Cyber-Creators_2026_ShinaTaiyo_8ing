//============================================================================
// 
// ２０２５年７月２３日：UI用テキストを描画する [UITextManager.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _UITEXTMANAGER_H_
#define _UITEXTMANAGER_H_

//****************************************************
// インクルード
//****************************************************
#include "UiText.h"

//****************************************************
// UIテキストクラス
//****************************************************
class CUITextManager
{
public:
	// === 特殊関数 ===

	CUITextManager();  // コンストラクタ
	~CUITextManager(); // デストラクタ

	// === ライフサイクルメンバ関数 ===

	void Update(); // 更新処理
	void Draw();   // 描画処理
	 
	// === メンバ関数 ===
	void Clear(); // UIテキスト動的配列をクリアする
	void Add(CUIText * pUiText); // UIテキスト動的配列を追加する

private:
	// === メンバ変数 ===

	std::vector<CUIText*> m_UITexts;
};

#endif // _PALYER_H_