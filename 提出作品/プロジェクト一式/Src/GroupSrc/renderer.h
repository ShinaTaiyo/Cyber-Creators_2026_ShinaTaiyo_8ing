//============================================================================
// 
// レンダラー、ヘッダファイル [renderer.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_	// 二重インクルード防止

//****************************************************
// インクルード
//****************************************************
#include "UiTextManager.h"

//****************************************************
// 前方宣言
//****************************************************
class CObject;

//****************************************************
// レンダラークラス
//****************************************************
class CRenderer final
{
public:

	// <special function>
	CRenderer(const CRenderer&) = delete;				// コピーコンストラクタ
	CRenderer& operator=(const CRenderer&) = delete;	// コピー代入演算子
	CRenderer(CRenderer&&) = delete;					// ムーブコンストラクタ
	CRenderer& operator=(CRenderer&&) = delete;			// ムーブ代入演算子

	// <function>
	void Update();		// 更新処理
	void Draw();		// 描画処理
	void PrintDebug();	// デバッグ表示

	// UIテキストマネージャー
	CUITextManager& GetUiTextManager(); // 取得

	// フォグの始点をセット
	inline void SetFogStart(float start)
	{
		m_fFogStart = start;
		m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fFogStart));
	}

	// フォグの終点をセット
	inline void SetFogEnd(float end)
	{
		m_fFogEnd = end;
		m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fFogEnd));
	}

	// フォグカラーをセット
	inline void SetFogCol(D3DXCOLOR col) { m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, col); }

	// 空間描画フラグのセット
	inline void SetDrawSpace() { m_bDrawSpace = true; }

	// <static function>
	static HRESULT	Create(HWND hWnd, BOOL bWindiw);	// 生成
	static void		Release();							// 破棄
	static void		CorrectFogStart(float fStart);		// フォグの始点を補間
	static void		CorrectFogEnd(float fEnd);			// フォグの終点を補間

	// <static getter>
	static CRenderer*			GetRenderer();	// レンダラーを取得
	static LPDIRECT3DDEVICE9	GetDeviece();	// デバイスを取得

	// <static setter>
	static void	SetDebugString(std::string Str);			// デバッグ文字列に追加
	static void	SetTimeString(std::string Str, int nCnt);	// 時限式デバッグ文字列に追加

private:

	//****************************************************
	// 静的定数を定義
	//****************************************************
	static constexpr float FOG_SRART = 1500.0f;	// フォグの初期始点
	static constexpr float FOG_END = 2500.0f;	// フォグの初期終点

	// <special function>
	CRenderer();	// コンストラクタ
	~CRenderer();	// デストラクタ

	// <function>
	HRESULT Init(HWND hWnd, BOOL bWindiw);	// 初期設定
	void	Uninit();						// 終了処理

	void DrawTextMesh(CObject& ref);	// テキストメッシュ用の描画処理
	void DrawTexture(CObject& ref);		// テクスチャ用の描画処理

	// <data>
	LPDIRECT3D9									m_pD3D;			// Direct3D
	LPDIRECT3DDEVICE9							m_pD3DDevice;	// デバイス
	float										m_fFogStart;	// フォグの始点
	float										m_fFogEnd;		// フォグの終点
	LPD3DXFONT									m_pFont;		// フォント
	LPD3DXFONT									m_pDebugFont;	// デバッグフォント
	std::string									m_DebugStr;		// 表示用文字列
	std::vector<std::pair<std::string, int>>	m_TimeStr;		// 時限式文字列
	CUITextManager                              m_UiTextManager; // UIテキストマネージャー

	bool m_bDrawSpace;	// 空間描画フラグ

	// <static data>
	static CRenderer* m_pRenderer;	// レンダラーの本体
};

#endif // _RENDERER_H_