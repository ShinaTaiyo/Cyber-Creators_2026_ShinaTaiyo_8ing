//============================================================================
// 
// ロガー、ヘッダファイル [logger.h]
// Author : 福田歩希
// 
//============================================================================

// インクルードガード
#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "singleton.hpp"

//****************************************************
// ロガークラスの定義
//****************************************************
class CLogger final : public CSingleton<CLogger>
{
	//****************************************************
	// フレンド宣言
	//****************************************************
	friend struct std::default_delete<CLogger>;
	friend CLogger* CSingleton<CLogger>::GetInstance();

public:

	//****************************************************
	// function
	//****************************************************

	/**
	 * 毎フレーム更新 & 出力、即時消去されるログを追加します
	 * 
	 * ptr, str - 出力したいテキスト
	 */
	void AddSynchronization(const char* ptr);			// - ①
	void AddSynchronization(const std::string& str);	// - ②

	/**
	 * 指定期間の間のみ出力されるログを追加します
	 *
	 * ptr, str - 出力したいテキスト
	 * duration - 出力期間、秒数で指定してください
	 */
	void AddDuration(const char* ptr, int duration);		// - ①
	void AddDuration(const std::string& str, int duration);	// - ②

	/**
	 * プロセスの終了まで保持されるログを追加します
	 * 
	 * ptr, str - 出力したいテキスト
	 */
	void AddPersistence(const char* ptr);			// - ①
	void AddPersistence(const std::string& str);	// - ②
	
	/**
	 * モーダルウィンドウにより処理を停止させる警告用ログを追加します
	 * 
	 * ptr, str - 出力したいテキスト
	 */
	[[ deprecated ]] void AddWarning(const char* ptr);			// - ①
	[[ deprecated ]] void AddWarning(const std::string& str);	// - ②

	/**
	 * 周波数の計測を開始します
	 */
	void StartFreqCount();

	/**
	 * 周波数の計測を停止します
	 */
	void StopFreqCount();

	/**
	 * 計測をラップします
	 */
	void LapFreqCount();

	/**
	 * 集積したデータを出力します、この関数は更新内で毎フレーム呼び出します
	 */
	void DisplayLog();

private:

	//****************************************************
	// 静的メンバ定数の定義 (非公開)
	//****************************************************

	// JSONオブジェクトをエクスポートするためのパス
	static constexpr const char* RELATIVE_PATH = "Data\\JSON\\Debug\\Persistence_Log ";
	static constexpr const char* EXTENSION = ".json";

	//****************************************************
	// LOG_BASE構造体を定義
	//****************************************************
	struct LOG_BASE
	{
		// <special function>
		LOG_BASE() = default;			// デフォルトコンストラクタ
		virtual ~LOG_BASE() = default;	// デストラクタ

		// <function>
		virtual bool Log() = 0;	// ログ出力

		// <data>
		std::string m_Log;	// ログ
	};

	//****************************************************
	// LOG_SYNCHRONIZATION構造体を定義
	//****************************************************
	struct LOG_SYNCHRONIZATION final : public LOG_BASE
	{
		// <special function>
		LOG_SYNCHRONIZATION(const char* ptr);			// コンストラクタ - ①
		LOG_SYNCHRONIZATION(const std::string& str);	// コンストラクタ - ②
		~LOG_SYNCHRONIZATION() override = default;		// デストラクタ

		// <function>
		bool Log() override;	// ログ出力
	};

	//****************************************************
	// LOG_DURATION構造体を定義
	//****************************************************
	struct LOG_DURATION final : public LOG_BASE
	{
		// <special function>
		LOG_DURATION(const char* ptr, int duration);		// コンストラクタ - ①
		LOG_DURATION(const std::string& str, int duration);	// コンストラクタ - ②
		~LOG_DURATION() override = default;					// デストラクタ

		// <function>
		bool Log() override;	// ログ出力

		// <data>
		int m_nDuration;	// 出力期間
	};

	//****************************************************
	// special function
	//****************************************************
	CLogger();				// デフォルトコンストラクタ
	~CLogger() override;	// デストラクタ

	//****************************************************
	// function
	//****************************************************
	bool Initialize();				// 初期化処理
	void Finalize();				// 終了処理
	void ControlMenuBar();			// メニューバー操作
	void WindowStyleTab();			// ウィンドウ形式 - タブ 
	bool DisplayLogCurrent();		// 現行ログ出力
	bool DisplayLogPersistence();	// 永続ログ出力
	bool DisplayLogWarning();		// 警告ログ出力

	//****************************************************
	// data
	//****************************************************
	std::vector<std::unique_ptr<LOG_BASE>> m_vupLogData;		// ログ集積データ
	std::vector<std::string>			   m_vLogPersistence;	// 永続ログ集積データ
	std::string							   m_LogWarning;		// 警告ログ

	bool m_bSwitchSize;	// サイズ切り替え
	bool m_bAutoScroll;	// 自動スクロール切り替え
	bool m_bFreqCount;	// 周波数カウント

	LARGE_INTEGER m_StartFreq;	// 計測開始点
	LARGE_INTEGER m_StopFreq;	// 計測終了点
	LARGE_INTEGER m_FreqBase;	// ベース周波数
};