//============================================================================
// 
// ２０２５年７月２３日：UI用テキストを描画する [UIText.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _UITEXT_H_
#define _UITEXT_H_

//****************************************************
// 前方宣言
//****************************************************
class CObject_2D;

//****************************************************
// UIテキストクラス
//****************************************************
class CUIText
{
public:
	// === 特殊関数 ===

	// コンストラクタ
	CUIText(
	    std::string Str, // 文字
		int PosX, // 位置X
		int PosY, // 位置Y
		int nFontSize, // フォントサイズ
		int nDispFrame, // 表示フレーム数
		D3DXCOLOR Col, // 色合い
		D3DXCOLOR BgColor // テキスト背景の色合い
	);  
	~CUIText();	// デストラクタ

	// === ライフサイクルメンバ関数 ===

	void Update();  // 更新処理
	void Draw();    // 描画処理

	// === メンバ関数 ===

	// テキストの横幅
	const int& GetTextWidth() const; // 取得

	// テキストの高さ
	const int& GetTextHeight() const; // 取得

	// 位置X
	const int& GetPosX() const; // 取得

	// 位置Y
	const int& GetPosY() const; // 取得

	// 表示フレーム数
	const int& GetDispFrame() const; // 取得

private:
	int m_nPosX;        // 位置X
	int m_nDispFrame;   // 表示フレーム数
	int m_nPosY;        // 位置Y
	int m_nFontSize;    // フォントサイズ
	LPD3DXFONT m_pFont; // フォント
	std::string m_Str;  // 文字
	D3DXCOLOR m_Col;    // 色合い
	int m_nTextWidth;   // テキストの横幅
	int m_nTextHeight;  // テキストの高さ
	CObject_2D* m_pTextBG; // テキスト背景
};

#endif // _PALYER_H_