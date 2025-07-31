//============================================================================
// 
// ロガー [logger.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "logger.h"

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 同期ログ追加 - ①
//============================================================================
void CLogger::AddSynchronization(const char* ptr)
{
	std::unique_ptr<LOG_SYNCHRONIZATION> upSync = std::make_unique<LOG_SYNCHRONIZATION>(ptr);
	m_vupLogData.push_back(std::move(upSync));
}

//============================================================================
// 同期ログ追加 - ②
//============================================================================
void CLogger::AddSynchronization(const std::string& str)
{
	std::unique_ptr<LOG_SYNCHRONIZATION> upSync = std::make_unique<LOG_SYNCHRONIZATION>(str);
	m_vupLogData.push_back(std::move(upSync));
}

//============================================================================
// 時限ログ追加 - ①
//============================================================================
void CLogger::AddDuration(const char* ptr, int duration)
{
	std::unique_ptr<LOG_DURATION> upSync = std::make_unique<LOG_DURATION>(ptr, duration * 60);
	m_vupLogData.push_back(std::move(upSync));
}

//============================================================================
// 時限ログ追加 - ②
//============================================================================
void CLogger::AddDuration(const std::string& str, int duration)
{
	std::unique_ptr<LOG_DURATION> upSync = std::make_unique<LOG_DURATION>(str, duration * 60);
	m_vupLogData.push_back(std::move(upSync));
}

//============================================================================
// 永続ログ追加 - ①
//============================================================================
void CLogger::AddPersistence(const char* ptr)
{
	const std::string& str = ptr;
	m_vLogPersistence.push_back(str);
}

//============================================================================
// 永続ログ追加 - ②
//============================================================================
void CLogger::AddPersistence(const std::string& str)
{
	m_vLogPersistence.push_back(str);
}

//============================================================================
// 警告ログ追加 - ①
//============================================================================
void CLogger::AddWarning(const char* ptr)
{
	m_LogWarning = ptr;
}

//============================================================================
// 警告ログ追加 - ②
//============================================================================
void CLogger::AddWarning(const std::string& str)
{
	m_LogWarning = str;
}

//============================================================================
// 計測開始
//============================================================================
void CLogger::StartFreqCount()
{
	if (m_bFreqCount)
	{
		return;
	}

	m_bFreqCount = true;

	QueryPerformanceCounter(&m_StartFreq);
}

//============================================================================
// 計測停止
//============================================================================
void CLogger::StopFreqCount()
{
	if (!m_bFreqCount)
	{
		return;
	}

	m_bFreqCount = false;

	QueryPerformanceCounter(&m_StopFreq);

	// 経過した時間を割り出す
	double dTime = static_cast<double>(m_StopFreq.QuadPart - m_StartFreq.QuadPart) / m_FreqBase.QuadPart;

	// 永続ログに追加
	AddPersistence(std::to_string(dTime) + " : [s]");

	// 計測の開始点・終了の情報を初期化
	m_StartFreq = {};
	m_StopFreq = {};
}

//============================================================================
// ラップ
//============================================================================
void CLogger::LapFreqCount()
{
	if (!m_bFreqCount)
	{
		return;
	}

	StopFreqCount();
	StartFreqCount();
}

//============================================================================
// ログ出力
//============================================================================
void CLogger::DisplayLog()
{
	/* サイズのフラグに応じて変動 (雑な実装) */
	ImVec2 Size = { 0, 0 };
	!m_bSwitchSize ? Size = { 200, 400 } : Size = { 500, 400 };

	// ウィンドウのサイズと位置を設定
	ImGui::SetNextWindowSize(Size);
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Debug Window", nullptr, ImGuiWindowFlags_MenuBar))
	{
		// メニューバーの操作
		ControlMenuBar();

		// タブ形式でのログ出力
		WindowStyleTab();

		// ウィンドウの表示終了
		ImGui::End();
	}
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// LOG_SYNCHRONIZATION構造体のコンストラクタ - ①
//============================================================================
CLogger::LOG_SYNCHRONIZATION::LOG_SYNCHRONIZATION(const char* ptr)
{
	this->m_Log = ptr;
}

//============================================================================
// LOG_SYNCHRONIZATION構造体のコンストラクタ - ②
//============================================================================
CLogger::LOG_SYNCHRONIZATION::LOG_SYNCHRONIZATION(const std::string& str)
{
	this->m_Log = str;
}

//============================================================================
// LOG_SYNCHRONIZATION構造体のログ出力
//============================================================================
bool CLogger::LOG_SYNCHRONIZATION::Log()
{
	ImGui::Text(m_Log.c_str());

	// Synchronizationなログは即時消去するため必ずtrueを返す
	return true;
}

//============================================================================
// LOG_DURATION構造体のコンストラクタ - ①
//============================================================================
CLogger::LOG_DURATION::LOG_DURATION(const char* ptr, int duration) :
	m_nDuration(duration)
{
	this->m_Log = ptr;
}

//============================================================================
// LOG_DURATION構造体のコンストラクタ - ②
//============================================================================
CLogger::LOG_DURATION::LOG_DURATION(const std::string& str , int duration) :
	m_nDuration(duration)
{
	this->m_Log = str;
}

//============================================================================
// LOG_DURATION構造体のログ出力
//============================================================================
bool CLogger::LOG_DURATION::Log()
{
	ImGui::TextColored({ 255, 255, 0, 255 }, m_Log.c_str());

	// 出力期間をデクリメント
	--m_nDuration;

	// Durationなログは期間が0になると消去するためtrueを、出力のみの場合はfalseを返す
	return m_nDuration <= 0 ? true : false;
}

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CLogger::CLogger() :
	m_vupLogData{},
	m_vLogPersistence{},
	m_LogWarning{},
	m_bSwitchSize(false),
	m_bAutoScroll(false),
	m_bFreqCount(false),
	m_StartFreq{},
	m_StopFreq{},
	m_FreqBase{}
{}

//============================================================================
// デストラクタ
//============================================================================
CLogger::~CLogger()
{
	// 終了処理
	Finalize();
}

//============================================================================
// 初期設定
//============================================================================
bool CLogger::Initialize()
{
	// コンテナクラスのクリア
	m_vupLogData.clear();		// ログ集積データ
	m_vLogPersistence.clear();	// 永続ログ集積データ
	m_LogWarning.clear();		// 警告ログ

	// 1秒あたりの周波数を取得
	QueryPerformanceFrequency(&m_FreqBase);

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CLogger::Finalize()
{
	// usingディレクティブ
	using std::to_string;

	// システム時刻を取得
	SYSTEMTIME Time = {};
	GetLocalTime(&Time);

	// 書き出しログ用のパスを作成
	std::string Path = RELATIVE_PATH + to_string(Time.wYear) + "_" + to_string(Time.wMonth) + "_" + to_string(Time.wDay) + " " + to_string(Time.wHour) + "-" + to_string(Time.wMinute) + "-" + to_string(Time.wSecond) + EXTENSION;

	// ファイルを書き出し展開
	std::ofstream Ofs(Path, std::ios::out);

	// 書き出し用ジェイソンオブジェクトを作成
	JSON Export;

	if (Ofs.good())
	{
		unsigned short int wIdx = 0;

		for (const auto& it : m_vLogPersistence)
		{
			Export["Log"][wIdx] = it.c_str();
			++wIdx;
		}

		// ジェイソンデータをシリアライズ
		Ofs << Export.dump(1, '	');	// 第一引数 -> インデント数, 第二引数 -> インデント形式
	}
}

//============================================================================
// メニューバーの操作
//============================================================================
void CLogger::ControlMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(u8"サイズ"))
		{
			if (ImGui::MenuItem(u8"小", nullptr, !m_bSwitchSize))
			{
				m_bSwitchSize = false;
			}

			if (ImGui::MenuItem(u8"大", nullptr, m_bSwitchSize))
			{
				m_bSwitchSize = true;
			}

			ImGui::EndMenu();
		}
		ImGui::Separator();

		if (ImGui::BeginMenu(u8"ログ"))
		{
			if (ImGui::MenuItem(u8"最新ログへの自動スクロール", nullptr, m_bAutoScroll))
			{
				m_bAutoScroll = !m_bAutoScroll;
			}

			if (ImGui::MenuItem(u8"永続ログをリセット"))
			{
				m_vLogPersistence.clear();			
			}

			ImGui::EndMenu();
		}
		ImGui::Separator();

		ImGui::EndMenuBar();
	}
}

//============================================================================
// ウィンドウ形式 - タブ
//============================================================================
void CLogger::WindowStyleTab()
{
	if (ImGui::BeginTabBar("StyleTab - Tab Bar"))
	{
		if (ImGui::BeginTabItem(u8"現在のログ"))
		{
			if (ImGui::BeginChild("StyleTab - Tab Bar - ChildA", { 0, 300 }, false, ImGuiWindowFlags_HorizontalScrollbar))
			{
				// 現行ログ出力
				DisplayLogCurrent();

				// 上下自動スクロール
				if (m_bAutoScroll)
				{
					ImGui::SetScrollHereY(1.0f);
				}
			}
			ImGui::EndChild();	// https://github.com/ocornut/imgui/issues/6683

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"過去のログ"))
		{
			if (ImGui::BeginChild("StyleTab - Tab Bar - ChildB", { 0, 300 }, false, ImGuiWindowFlags_HorizontalScrollbar))
			{
				// 永続ログ出力
				DisplayLogPersistence();

				// 上下自動スクロール
				if (m_bAutoScroll)
				{
					ImGui::SetScrollHereY(1.0f);
				}
			}
			ImGui::EndChild();	// https://github.com/ocornut/imgui/issues/6683

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}

//============================================================================
// 現行ログ出力
//============================================================================
bool CLogger::DisplayLogCurrent()
{
	// Currentなログが1つも無ければfalseを返す
	if (m_vupLogData.empty())
	{
		return false;
	}

	// テーブルの表示幅
	float fWidth = 0.0f;

	// 先行して全テキスト内の最大サイズを取得
	for (const auto& it : m_vupLogData)
	{
		fWidth = max(fWidth, ImGui::CalcTextSize(it->m_Log.c_str()).x);
	}

	// 子ウィンドウ内のテーブルを必要に応じ水平スクロールさせるため、横幅の指定を最大テキストサイズにする
	if (ImGui::BeginTable("Current Table", 1, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders, { fWidth + 10, 0 }))
	{
		// 集積データの全てのインスタンスでログ出力を行う
		for (auto it = m_vupLogData.begin(); it != m_vupLogData.end(); )
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			// ログ出力とともに、消去フラグを受け取る
			if (it->get()->Log())
			{
				// このインスタンスを削除し、次の要素のイテレータを受け取る
				it = m_vupLogData.erase(it);

				// イテレータをインクリメントせずに続行
				continue;
			}

			// 消去を行わなかった場合、そのままイテレータをインクリメント
			++it;
		}

		ImGui::EndTable();
	}

	return true;
}

//============================================================================
// 永続ログ出力
//============================================================================
bool CLogger::DisplayLogPersistence()
{
	// Persistenceなログが1つも無ければfalseを返す
	if (m_vLogPersistence.empty())
	{
		return false;
	}

	// テーブルの表示幅
	float fWidth = 0.0f;

	// 先行して全テキスト内の最大サイズを取得
	for (const auto& it : m_vLogPersistence)
	{
		fWidth = max(fWidth, ImGui::CalcTextSize(it.c_str()).x);
	}

	// 子ウィンドウ内のテーブルを必要に応じ水平スクロールさせるため、横幅の指定を最大テキストサイズにする
	if (ImGui::BeginTable("Persistence Table", 1, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders, { fWidth + 10, 0 }))
	{
		//// テキストフィルターを作成
		//static ImGuiTextFilter Filter;
		//Filter.Draw();

		// 消去を行わないため、全ての要素をログ出力する
		for (const auto& it : m_vLogPersistence)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			// この要素テキストをコピー
			/* コピーしなくてもstring型のコンテナだしイテレータ使え */
			/* なんかstring型のイテレータってエレメント指定出来ない…？ */
			//const char* Lines[] = { it.c_str() };

			//for (unsigned short wCntLoop = 0; wCntLoop < it.size(); ++wCntLoop)
			//{
			//	if (Filter.PassFilter(Lines[wCntLoop]))
			//	{
					//ImGui::TextColored({ 0, 255, 255, 255 }, "%s", Lines[wCntLoop]);
			//	}
			//}
			ImGui::TextColored({ 0, 255, 255, 255 }, it.c_str());
		}

		ImGui::EndTable();
	}

	return true;
}

//============================================================================
// 警告ログ出力
//============================================================================
bool CLogger::DisplayLogWarning()
{
	// モーダルウィンドウの場所を中央に固定
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, { 0.5, 0.5 });

	ImGui::OpenPopup("Warning");

	if (ImGui::BeginPopupModal("Warning", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::TextColored({ 128, 0, 0, 255 }, m_LogWarning.c_str());

		if (ImGui::Button(u8"確認"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	return false;
}