//============================================================================
// 
// ベジェ曲線マネージャー [bezier_manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "bezier_manager.h"

// デバイス取得用
#include "renderer.h"

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
std::string CBezier_Manager::RELOAD_PATH = "";

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 応急処置
//============================================================================
void CBezier_Manager::ExplicitInitialize()
{
	// 生成処理用のラムダオブジェクト作成
	std::function<std::unique_ptr<CQuadratic_Bezier>(const std::string&)> fpCreate =
		[](const std::string& path) -> std::unique_ptr<CQuadratic_Bezier>
	{
		// ベジェ曲線のインスタンスを生成
		std::unique_ptr<CQuadratic_Bezier> upElement = make_unique<CQuadratic_Bezier>();

		// 生成失敗
		if (!upElement)
		{
			return nullptr;
		}

		// 初期化処理
		if (!upElement->Initialize())
		{
			return nullptr;
		}

		// 生成用の追加パラメータを展開
		const JSON& Json = hws::SafeOpenJsonFile(path);

		// 制御点の座標を設定
		for (unsigned short wIdx = 0; wIdx < CQuadratic_Bezier::NUM_CONTROL; ++wIdx)
		{
			upElement->SetPosToControlPointForSelectedIdx(hws::JsonConvertToVec3(Json["Pos"][wIdx]), wIdx);
		}

		// 進行速度をセット
		upElement->SetSpeed(static_cast<float>(Json["Speed"]));

		// 反転使用フラグの状態をセット
		upElement->SetUseReverse(static_cast<bool>(Json["UseReverse"]));

		// インスタンスのポインタを返す
		return upElement;
	};

	// 再読み込みパスが設定されていなければ自動的に初期データを読み込む
	if (RELOAD_PATH.empty())
	{
		RELOAD_PATH = INITIALIZE_PATH;
	}

	// レジストリの初期化処理
	m_upRegistry->Initialize(RELOAD_PATH, std::move(fpCreate));

	// ベジェ曲線インスタンスの最大数を保持
	m_nMaxSizeRegistry = static_cast<int>(m_upRegistry->RefDynamicResource().size());

	return;
}

//============================================================================
// 更新処理
//============================================================================
bool CBezier_Manager::Update()
{
	// 現在更新中のベジェ曲線を検索するためのカウント
	int nCntForCurrentIdx = 0;

	// インスタンスが生成された順番に1ずつ更新を行う
	for (auto& it : m_upRegistry->RefDynamicResource())
	{
		// 現在のカウント数が、更新対象となるインデックスでなければ
		if (m_nIdxCurrentBeizier != nCntForCurrentIdx)
		{
			// カウントのインクリメントのみで再ループ
			++nCntForCurrentIdx;
			
			continue;
		}

		// 移動点が終端に到達するまで、対象のインデックスの要素のみ更新
		if (it.second->Update())
		{
			// 移動点が終端に到達したとき、インデックスをインクリメント
			++m_nIdxCurrentBeizier;
		}

		/* 更新中の座標 */
		m_CurrentPos = it.second->GetPosTracePointOnTrajectoryLine();

		// インデックス数が要素の最大数を越えてしまう場合
		if (m_nIdxCurrentBeizier >= m_nMaxSizeRegistry)
		{
			// リバースフラグも不使用であるなら更新完了の通知を返す
			if (!it.second->GetUseReverse())
			{
				return true;
			}

			// 更新順番を最初に戻す
			m_nIdxCurrentBeizier = 0;
		}
		
		// これ以上の検索を中断
		break;
	}

	return false;
}

//============================================================================
// 描画処理
//============================================================================
void CBezier_Manager::Draw()
{
	// 全インスタンスの描画処理
	for (auto& it : m_upRegistry->RefDynamicResource())
	{
		it.second->Draw();
	}
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CBezier_Manager::CBezier_Manager() :
	m_upRegistry(std::make_unique<CRegistry<std::unique_ptr<CQuadratic_Bezier>>>()),
	m_nIdxCurrentBeizier(0),
	m_nMaxSizeRegistry(0),
	m_CurrentPos{ 0.0, 0.0, 0.0 }
{}

//============================================================================
// デストラクタ
//============================================================================
CBezier_Manager::~CBezier_Manager()
{
	// 終了処理
	Finalize();
}

//============================================================================
// 初期化処理
//============================================================================
bool CBezier_Manager::Initialize()
{
	// 生成処理用のラムダオブジェクト作成
	std::function<std::unique_ptr<CQuadratic_Bezier>(const std::string&)> fpCreate = 
		[](const std::string& path) -> std::unique_ptr<CQuadratic_Bezier>
	{
		// ベジェ曲線のインスタンスを生成
		std::unique_ptr<CQuadratic_Bezier> upElement = make_unique<CQuadratic_Bezier>();

		// 生成失敗
		if (!upElement)
		{
			return nullptr;
		}

		// 初期化処理
		if (!upElement->Initialize())
		{
			return nullptr;
		}

		// 生成用の追加パラメータを展開
		const JSON& Json = hws::SafeOpenJsonFile(path);

		// 制御点の座標を設定
		for (unsigned short wIdx = 0; wIdx < CQuadratic_Bezier::NUM_CONTROL; ++wIdx)
		{
			upElement->SetPosToControlPointForSelectedIdx(hws::JsonConvertToVec3(Json["Pos"][wIdx]), wIdx);
		}

		// 進行速度をセット
		upElement->SetSpeed(static_cast<float>(Json["Speed"]));

		// 反転使用フラグの状態をセット
		upElement->SetUseReverse(static_cast<bool>(Json["UseReverse"]));

		// インスタンスのポインタを返す
		return upElement;
	};

	// 再読み込みパスが設定されていなければ自動的に初期データを読み込む
	if (RELOAD_PATH.empty())
	{
		RELOAD_PATH = INITIALIZE_PATH;
	}

	// レジストリの初期化処理
	if (!m_upRegistry->Initialize(RELOAD_PATH, std::move(fpCreate)))
	{
		return false;
	}

	// ベジェ曲線インスタンスの最大数を保持
	m_nMaxSizeRegistry = static_cast<int>(m_upRegistry->RefDynamicResource().size());

	return true;
}

//============================================================================
// 終了処理
//============================================================================
void CBezier_Manager::Finalize()
{
	// 解放処理用のラムダオブジェクト作成
	std::function<void(std::unique_ptr<CQuadratic_Bezier>&)> fpRelease =
		[](std::unique_ptr<CQuadratic_Bezier>& element) noexcept -> void
	{
		// 終了処理無後、解放処理
		if (element)
		{
			element->Finalize();
			element.reset();
			element = nullptr;
		}
	};

	// レジストリの終了処理
	m_upRegistry->Finalize(fpRelease);
}