//============================================================================
// 
// Untitled_Project [main.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "main.h"
#include "manager.h"

// デバッグ表示用
#include "renderer.h"

// メモリリーク検出用
#include <crtdbg.h>
#include <stdlib.h>

//****************************************************
// マクロ定義
//****************************************************
#define CLASS_NAME	"WindowClass"		// ウィンドウクラスの名前
#define WINDOW_NAME	"Untitled_Project"	// ウィンドウの名前

// メモリリーク検出用
#define _CRTDBG_MAP_ALLOC

//***************************************************
// プロトタイプ宣言
//****************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウィンドウプロシージャ
void ChangeWindowSize(HWND hWnd);													// ウィンドウサイズの変更

//****************************************************************************
// メイン関数
//****************************************************************************
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定
	// CRTメモリリーク検出用
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						// ウィンドウのスタイル
		WindowProc,						// ウィンドウプロシージャ
		0,								// 何かを0にする
		0,								// 何かを0にする
		hInstance,						// インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW),			// クライアント領域の背景色
		NULL,							// メニューバー
		CLASS_NAME,						// ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	// ファイルのアイコン
	};

	HWND hWnd;	// ウィンドウハンドル(識別子)
	MSG  msg;	// メッセージを格納する変数

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域の設定用
	RECT rect = { 0, 0, WSCREEN_WIDTH, WSCREEN_HEIGHT };

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウを生成
	hWnd = CreateWindowEx(0,	// 拡張ウィンドウのスタイル
		CLASS_NAME,				// ウィンドウクラスの名前
		WINDOW_NAME,			// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,	// ウィンドウのスタイル
		125,					// ウィンドウの左上のX座標
		50,						// ウィンドウの左上のY座標
		rect.right - rect.left,	// ウィンドウの幅
		rect.bottom - rect.top,	// ウィンドウの高さ
		NULL,					// 親ウィンドウのハンドル
		NULL,					// メニューバーまたは子ウィンドウID
		hInstance,				// インスタンスハンドル
		NULL);					// ウィンドウ作成データ

	// ウィンドウの表示
	ShowWindow(hWnd, SW_NORMAL);	// ウィンドウの表示状態を設定
	//ChangeWindowSize(hWnd);		// ウィンドウサイズの変更

	// マネージャーの生成
	if (FAILED(CManager::Create(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// FPS計測用
	int   nCountFPS = 0;					// 現在のFPS
	DWORD dwCurrentTime = 0;				// 現在時刻用
	DWORD dwFrameCount = 0;					// 最後に処理した時間
	DWORD dwExecLastTime = timeGetTime();	// フレームカウント格納
	DWORD dwFPSLastTime = timeGetTime();	// 最後にFPSを計測した時刻格納

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			// Windowsの処理
			if (msg.message == WM_QUIT)
			{
				// WM_QUITのメッセージを受け取ると、メッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);	// 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);	// ウィンドウプロシージャへメッセージを創出
			}
		}
		else
		{
			// 現在時刻を取得
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				// FPSを計測
				nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				// FPSを計測した時間を保存
				dwFPSLastTime = dwCurrentTime;

				// フレームカウントをクリア
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				// 現在時刻を保存
				dwExecLastTime = dwCurrentTime;

				try
				{
					// マネージャーの更新処理
					CManager::GetManager()->Update();
				}
				catch (const std::bad_alloc& error_alloc)
				{
					MessageBox(NULL, error_alloc.what(), "new使用時にエラー", MB_OK | MB_ICONERROR);
					break;
				}
				catch (const std::bad_cast& error_cast)
				{
					MessageBox(NULL, error_cast.what(), "キャスト時にエラー", MB_OK | MB_ICONERROR);
					break;
				}
				catch (const std::runtime_error& error_runtime)
				{
					MessageBox(NULL, error_runtime.what(), "不正な動作が発生", MB_OK | MB_ICONERROR);
					break;
				}
				catch (const std::exception& error)
				{
					MessageBox(NULL, error.what(), "その他エラー発生", MB_OK | MB_ICONERROR);
					break;
				}

#ifdef _DEBUG	// FPS表示
				CRenderer::SetDebugString("FPS:" + std::to_string(nCountFPS));
#endif	// _DEBUG

				// マネージャーの描画処理
				CManager::GetManager()->Draw();

				// フレームカウントを加算
				++dwFrameCount;
			}
		}
	}

	// 分解能を戻す
	timeEndPeriod(1);

	// マネージャーの破棄
	CManager::Release();

	return static_cast<int>(msg.wParam);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//****************************************************************************
// ウィンドウプロシージャ関数
//****************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	// ImGuiの入力を受け付けるメッセージハンドラー
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}
#endif // _DEBUG

	// 返り値格納先
	int nID = 0 ;

	switch (uMsg)
	{
		// ウィンドウ破棄のメッセージを送る
	case WM_DESTROY:

		PostQuitMessage(0);

		break;

		// ×マークを押しているメッセージを送る
	case WM_CLOSE:

		// メッセージボックスを表示
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);	// 「WM_DESTROYというメッセージを送る」
		}
		else
		{
			return 0;
		}

		break;

		// カーソル変更のメッセージを送る
	case WM_SETCURSOR:

		// カーソルがクライアント領域内にあるときイメージを変更
		if (LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(LoadCursorFromFile("Data\\TEXTURE\\cursor.cur"));

			return TRUE;
		}

		break;

		// キーが押されているメッセージを送る
	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_ESCAPE:

			// メッセージボックスを表示
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);	// 「WM_DESTROYというメッセージを送る」
			}

			break;

		case VK_F11:

			// ウィンドウサイズの変更
			ChangeWindowSize(hWnd);

			break;
		}

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 既定の処理を返す
}

//****************************************************************************
// ウィンドウサイズの変更
//****************************************************************************
void ChangeWindowSize(HWND hWnd)
{
	// モード変更フラグ
	static bool bSetFullScreen = false;

	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	// モード変更
	bSetFullScreen = !bSetFullScreen;

	if (bSetFullScreen)
	{ // フルスクリーンモードに切り替え

		// ウィンドウの表示スタイルを変更する
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);

		// クライアント領域・ウィンドウサイズを決定
		RECT rect = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };

		// クライアント領域を反映
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		// 位置とサイズを反映
		SetWindowPos(hWnd, HWND_TOP,
			0,
			0,
			rect.right - rect.left,
			rect.bottom - rect.top,
			SWP_FRAMECHANGED | SWP_NOZORDER);
	}
	else
	{ // ウィンドウモードに切り替え
		
		// ウィンドウの表示スタイルを変更する
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);

		// クライアント領域・ウィンドウサイズを決定
		RECT rect = { 0, 0, WSCREEN_WIDTH, WSCREEN_HEIGHT };

		// クライアント領域を反映
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		// 位置とサイズを反映
		SetWindowPos(hWnd, HWND_TOP,
			125,
			50,
			rect.right - rect.left,
			rect.bottom - rect.top,
			SWP_FRAMECHANGED | SWP_NOZORDER);
	}

	// ウィンドウを更新
	UpdateWindow(hWnd);
}