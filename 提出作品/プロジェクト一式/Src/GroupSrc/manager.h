//============================================================================
// 
// マネージャー、ヘッダファイル [managaer.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "light.h"
#include "input_keyboard.h"
#include "input_mouse.h"
#include "input_pad.h"

//****************************************************
// マネージャークラス
//****************************************************
class CManager final
{
public:

	// <special function>
	CManager(const CManager&) = delete;				// コピーコンストラクタ
	CManager& operator=(const CManager&) = delete;	// コピー代入演算子
	CManager(CManager&&) = delete;					// ムーブコンストラクタ
	CManager& operator=(CManager&&) = delete;		// ムーブ代入演算子

	// <function>
	void Update();	// 更新処理
	void Draw();	// 描画処理

	// <getter>
	CLight*	GetLight() const;	// ライトを取得

	// <static function>
	static HRESULT Create(HINSTANCE hInstance, HWND hWnd);	// 生成
	static void	Release();									// 解放

	// <static getter>
	static CManager*		GetManager();	// マネージャーを取得
	static CInputKeyboard*	GetKeyboard();	// キーボードを取得
	static CInputMouse*		GetMouse();		// マウスを取得
	static CInputPad*		GetPad();		// パッドを取得

private:

	// <special function>
	CManager();		// コンストラクタ
	~CManager();	// デストラクタ

	// <function>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期設定
	void	InitImGui(HWND hWnd);					// ImGuiの初期設定
	void	Uninit();								// 終了処理
	void	UninitImGui();							// ImGuiの終了処理

	// <datas>
	CInputKeyboard*	m_pKeyboard;	// キーボード
	CInputMouse*	m_pMouse;		// マウス
	CInputPad*		m_pPad;			// パッド
	CLight*			m_pLight;		// ライト

	// <static data>
	static CManager* m_pManager;	// マネージャーの本体
};

#endif // _MANAGER_H_
