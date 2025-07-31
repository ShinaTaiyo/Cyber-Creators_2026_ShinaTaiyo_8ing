//============================================================================
// 
// 揺れ再生 [swaying_player.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "swaying_player.h"
#include "manager.h"

// シーン
#include "game.h"

// ログ出力用
#include "logger.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 静的メンバ定数の定義
//****************************************************

// ビュー上書き用パラメータの保持
const std::array<JSON, 2> CSwayingPlayer::s_aOverWriteViewParam = {
	hws::SafeOpenJsonFile("Data\\JSON\\INITIALIZE\\BEZIER\\Play.json"),
	hws::SafeOpenJsonFile("Data\\JSON\\INITIALIZE\\BEZIER\\Piston.json")
};

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// SWAY_STATE :: 更新
//============================================================================
bool CSwayingPlayer::SWAY_STATE::UpdateState(CSwayingPlayer*& refSwayingPlayer)
{
	up<SWAY_STATE>& refState = refSwayingPlayer->m_upState;

	if (!refState)
	{
		return false;
	}

	if (refState->Execute(refSwayingPlayer))
	{
		refState->Change(refState);
	}

	return true;
}

//============================================================================
// SWAY_STATE :: 実行
//============================================================================
bool CSwayingPlayer::SWAY_STATE::Execute(CSwayingPlayer*& refSwayingPlayer)
{
	throw std::runtime_error("'CSpace::SWAY_STATE::Execute()' was Failed : " + static_cast<std::string>(typeid(refSwayingPlayer).name()));
}

//============================================================================
// SWAY_STATE :: 変更
//============================================================================
void CSwayingPlayer::SWAY_STATE::Change(useful::up<SWAY_STATE>& refState)
{
	throw std::runtime_error("'CSpace::SWAY_STATE::Change()' was Failed : " + static_cast<std::string>(typeid(refState).name()));
}

//============================================================================
// SWAY_INTRO :: 実行
//============================================================================
bool CSwayingPlayer::SWAY_INTRO::Execute(CSwayingPlayer*& refSwayingPlayer)
{
	CCamera* pCam = refSwayingPlayer->m_pMainCamera;

	// 上書き処理が未設定の場合
	if (!pCam->GetStateOverwriteView())
	{
		// 指数減衰の使用
		pCam->SetUseDecayState(true);

		// ビュー上書きをするが、カメラインスタンスのキャプチャをしない
		pCam->OverwriteView(
			[](CCamera& refCam)
			{
				// 指数減衰を使用
				refCam.SetUseDecayState(true);

				// 目標向きを作成
				Vec3 RotTarget = { 0.0f, 0.0f, 0.0f };
				refCam.SetRotTarget(RotTarget);

				// 目標位置をビュー上書きパラメータの、1つ目の制御点の座標に設定
				refCam.SetPosTarget(hws::JsonConvertToVec3(s_aOverWriteViewParam[0]["Pos"][0]));

				// 目標距離を固定
				refCam.SetDistanceTarget(400.0f);
			});
	}

	// 受け取っている召喚クールタイムカウントが、規定値に到達で状態変更
	if (refSwayingPlayer->m_nCoolTimeCnt)
	{
		// 変更後、初回更新時のみビュー上書きをするためにnullptrを指定
		pCam->OverwriteView(nullptr);

		return true;
	}

	return false;
}

//============================================================================
// SWAY_INTRO :: 変更
//============================================================================
void CSwayingPlayer::SWAY_INTRO::Change(useful::up<SWAY_STATE>& refState)
{
	ChangeUniquePtr<SWAY_STATE, SWAY_PLAY>(refState);
}

//============================================================================
// SWAY_PLAY :: 実行
//============================================================================
bool CSwayingPlayer::SWAY_PLAY::Execute(CSwayingPlayer*& refSwayingPlayer)
{
	CCamera* pCam = refSwayingPlayer->m_pMainCamera;

	// 上書き処理が未設定の場合
	if (!pCam->GetStateOverwriteView())
	{
		// ベジェ曲線の初期化処理
		m_Bezier.Initialize();

		// 各制御点座標の設定
		for (unsigned short wCntLoop = 0; wCntLoop < 3; ++wCntLoop)
		{
			Vec3 ControlPos = hws::JsonConvertToVec3(s_aOverWriteViewParam[0]["Pos"][wCntLoop]);

			m_Bezier.SetPosToControlPointForSelectedIdx(ControlPos, wCntLoop);
		}

		// 進行速度の設定
		m_Bezier.SetSpeed(static_cast<float>(s_aOverWriteViewParam[0]["Speed"]));

		// 反転使用のフラグを立てる
		m_Bezier.SetUseReverse(true);

		// 指数減衰を使用
		pCam->SetUseDecayState(true);

		// ビュー上書きをするが、カメラインスタンスのキャプチャをしない
		pCam->OverwriteView(
			[this](CCamera& refCam)
			{
				// 目標回転ベクトルを固定値で作成
				Vec3 RotTarget = VEC3_INIT;
				refCam.SetRotTarget(RotTarget);

				// ベジェ曲線の更新
				m_Bezier.Update();

				// 軌跡の座標を取得
				Vec3 TrajectoryPos = m_Bezier.GetPosTracePointOnTrajectoryLine();

				// 位置を軌跡の座標に同期
				refCam.SetPosTarget(TrajectoryPos);

				// 目標距離を固定
				refCam.SetDistanceTarget(400.0f);
			});
	}

	// 受け取っている召喚クールタイムカウントが、規定値に到達で状態変更
	if (refSwayingPlayer->m_nCoolTimeCnt <= NUM_MAXCOUNT)
	{
		// 変更後、初回更新時のみビュー上書きをするためにnullptrを指定
		pCam->OverwriteView(nullptr);

		return true;
	}

	return false;
}

//============================================================================
// SWAY_PLAY :: 変更
//============================================================================
void CSwayingPlayer::SWAY_PLAY::Change(useful::up<SWAY_STATE>& refState)
{
	ChangeUniquePtr<SWAY_STATE, SWAY_OUTRO>(refState);
}

//============================================================================
// SWAY_OUTRO :: 実行
//============================================================================
bool CSwayingPlayer::SWAY_OUTRO::Execute(CSwayingPlayer*& refSwayingPlayer)
{
	CCamera* pCam = refSwayingPlayer->m_pMainCamera;

	// 上書き処理が未設定の場合
	if (!pCam->GetStateOverwriteView())
	{
		// ベジェ曲線の初期化処理
		m_Bezier.Initialize();

		// 各制御点座標の設定
		for (unsigned short wCntLoop = 0; wCntLoop < 3; ++wCntLoop)
		{
			Vec3 ControlPos = hws::JsonConvertToVec3(s_aOverWriteViewParam[1]["Pos"][wCntLoop]);

			m_Bezier.SetPosToControlPointForSelectedIdx(ControlPos, wCntLoop);
		}

		// 進行速度の設定
		m_Bezier.SetSpeed(static_cast<float>(s_aOverWriteViewParam[1]["Speed"]));

		// 指数減衰の使用
		pCam->SetUseDecayState(true);

		// ビュー上書きをするが、カメラインスタンスのキャプチャをしない
		pCam->OverwriteView(
			[this](CCamera& refCam)
			{
				// 指数減衰を使用
				refCam.SetUseDecayState(true);

				// ベジェ曲線の更新
				m_Bezier.Update();

				// 軌跡の座標を取得
				Vec3 TrajectoryPos = m_Bezier.GetPosTracePointOnTrajectoryLine();

				// 位置を軌跡の座標に同期
				refCam.SetPosTarget(TrajectoryPos);

				// 目標距離を固定
				refCam.SetDistanceTarget(400.0f);
			});
	}

	// 終了判定無し、外部から明示的に解放し、状態をリセットしてください
	return false;
}

//============================================================================
// SWAY_OUTRO :: 変更
//============================================================================
void CSwayingPlayer::SWAY_OUTRO::Change(useful::up<SWAY_STATE>& refState)
{}

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CSwayingPlayer::CSwayingPlayer() :
	m_upState(nullptr),
	m_pMainCamera(nullptr),
	m_nCoolTimeCnt(0)
{}

//============================================================================
// 揺れ
//============================================================================
bool CSwayingPlayer::Sway(int coolTimeCnt)
{
	// 揺れ状態が生成されていなければ
	if (!m_upState)
	{
		// イントロに変更
		m_upState = make_unique<SWAY_INTRO>();
	}

	// この時使用されているメインカメラの情報を渡す
	m_pMainCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game");

	// 召喚クールタイムをコピー
	m_nCoolTimeCnt = coolTimeCnt;

	// thisポインタから左辺値のポインタに
	CSwayingPlayer* Tmp = this;

	// 状態の更新
	SWAY_STATE::UpdateState(Tmp);

#ifdef _DEBUG
	std::string tmpStr = to_string(coolTimeCnt);
	if (typeid(*m_upState) == typeid(SWAY_INTRO))
		CLogger::GetInstance()->AddSynchronization(u8"イントロ : " + tmpStr);
	else if (typeid(*m_upState) == typeid(SWAY_PLAY))
		CLogger::GetInstance()->AddSynchronization(u8"再生 : " + tmpStr);
	else if (typeid(*m_upState) == typeid(SWAY_OUTRO))
		CLogger::GetInstance()->AddSynchronization(u8"アウトロ : " + tmpStr);
#endif // _DEBUG

	return false;
}