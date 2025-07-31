//============================================================================
// 
// ２０２５年７月１０日：召喚関係のバグの改善とプレイヤー条件分岐をステートパターンにする [PlayerState_BeforeThrust.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルード
//****************************************************

// === オブジェクト ===

#include "player.h" // プレイヤー
#include "target.h" 
#include "obstacle.h" // 障害物
#include "HoleObstacle.h" // 穴あき障害物
#include "zone.h" // ゾーン
#include "summon_manager.h" // 召喚マネージャー
#include "object.h"
#include "SummonGroupSelector.h" // 召喚グループ選択
#include "particle.h" // ターゲット位置確認用

// === 状態 ===
#include "PlayerState_BeforeThrust.h" // 突きの前状態
#include "PlayerState_Thrusting.h" // 突き
#include "PlayerState_Returning.h" // 戻る
#include "summon_manager_state.h" // 召喚マネージャー状態
#include "summon_manager_state_start.h" // 召喚マネージャー召喚開始状態
#include "camera_state_targetzoom.h" // カメラズーム状態
#include "camera_state_normal.h" // カメラ通常状態

// === 情報 ===

#include "transparent_info.h" // 透明度情報
#include "game.h" // ゲームシーン
#include "manager.h" // マネージャー
#include "input.h" // 入力
#include "renderer.h" // 描画
#include "logger.h" // ログ
#include "camera.h" // カメラ
#include "move_info.h" // 移動情報
#include "calculation.h" // 計算
#include "sound.h" // サウンド

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CPlayerState_BeforeThrust::CPlayerState_BeforeThrust(CPlayer* pPlayer, int nAnticipationFrame) :
	CPlayerState(pPlayer),
    m_nAnticipationFrame{ nAnticipationFrame },
	m_bStartAnticipation{false}
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CPlayerState_BeforeThrust::~CPlayerState_BeforeThrust()
{
	// 何もなし
}

//============================================================================
// 初期化処理
//============================================================================
void CPlayerState_BeforeThrust::Init(CPlayer* pPlayer)
{
	// プレイヤーのターゲットを操作可能にし、カメラの位置をターゲット基準にする
	CTarget* pTarget = pPlayer->GetTarget(); // ターゲットへのポインタ
	pTarget->SetUseControl(true);
	pTarget->SetUseAdjustCameraPos(true);
	pTarget->SetUseMove(true);

	// ゲームのカメラの状態を通常にする
	CCamera_Manager::GetInstance()->RefSelectCamera().ChangeState(DBG_NEW CCamera_State_Normal());

	// プレイヤー状態をログで表示
	CLogger::GetInstance()->AddDuration(u8"プレイヤーが「突き前状態」になりました！", 1);
}

//============================================================================
// 終了処理
//============================================================================
void CPlayerState_BeforeThrust::Uninit(CPlayer* pPlayer)
{
	// 何もなし
}

//============================================================================
// 更新処理
//============================================================================
void CPlayerState_BeforeThrust::Update(CPlayer* pPlayer)
{
	bool bDecisionGoal = false; // 目的地を決めたかどうか

	if (m_bStartAnticipation)
	{
		// 目的地を決める操作
		bDecisionGoal = ControlDecisionGoal();
	}

	// 位置固定処理
	FixedPos(pPlayer);

	// 召喚されたオブジェクトをスロー状態にする
	JudgeLerpPosToSummonObj(pPlayer);

	// 見切りが始まった時に手刀を可能にする
	if (m_bStartAnticipation)
	{
		if (HandSword(pPlayer)) // 手刀処理
		{
			// ステートが変わるので処理を終了
			return;
		}
	}

	// 目的地を決定するなら（trueになった瞬間)
	if (bDecisionGoal)
	{
		DecisionGoal(pPlayer); // 目的地決定処理

		return; // 目標位置を決めたので処理を終了する
	}

	// 見切りが開始したらそのフレーム数を減らす
	if (m_bStartAnticipation)
	{
		// 見切りフレーム数を減らす
		if (m_nAnticipationFrame > 0)
		{
			m_nAnticipationFrame--;
		}
	}

	// デバッグ表示
	CRenderer::SetDebugString("プレイヤーの状態：突く前");
	CRenderer::SetDebugString("見切りフレーム数：" + to_string(m_nAnticipationFrame));
}

//============================================================================
// 見切りフレーム数取得
//============================================================================
const int& CPlayerState_BeforeThrust::GetAnticipationFrame() const
{
	return m_nAnticipationFrame;
}

//============================================================================
// 見切りフレーム数設定
//============================================================================
void CPlayerState_BeforeThrust::SetAnticipationFrame(int nFrame)
{
	m_nAnticipationFrame = nFrame;
}

//============================================================================
// 手刀のコストを取得する
//===========================================================================
const int& CPlayerState_BeforeThrust::GetHandSwordCost()
{
	return s_nHandSwordCost;
}

//============================================================================
// 通常見切りフレーム数を取得する
//===========================================================================
const int& CPlayerState_BeforeThrust::GetNormalAnticipationFrame()
{
	return s_nNormalAnticipationFrame;
}

//============================================================================
// 手刀処理
//============================================================================
bool CPlayerState_BeforeThrust::HandSword(CPlayer* pPlayer)
{
	CInputKeyboard* pInputKeyboard = CManager::GetManager()->GetKeyboard(); // キー入力情報
	auto* pPad = CManager::GetManager()->GetPad(); // パッド入力情報
	int nAirPower = pPlayer->GetAirPower(); // 気
	int nHandSwordCost = s_nHandSwordCost; // 手刀コスト

	// 手刀パワーが規定値に達していなければ無視
	if (nAirPower < nHandSwordCost)
	{
		return false;
	}

	// 手刀パワーが一定以上存在している状態でXを押す、または右スティックを倒すと手刀を開始する
	if (pInputKeyboard->GetTrigger(DIK_X) ||
		pPad->GetTrigger(CInputPad::JOYKEY::X) ||
		abs(pPad->GetJoyStickR().X) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		abs(pPad->GetJoyStickR().Y) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
#if 1
		// 攻撃SE再生
		WORD wRand = rand() % 3;
		CSound::LABEL Label = CSound::LABEL::ATK_0;
		switch (wRand)
		{
		case 0:		Label = CSound::LABEL::ATK_0;	break;
		case 1:		Label = CSound::LABEL::ATK_1;	break;
		case 2:		Label = CSound::LABEL::ATK_2;	break;
		default:	Label = CSound::LABEL::ATK_0;	break;
		}
		CSound::GetInstance()->Play(Label);
#endif
		m_nAnticipationFrame = 0; // もう見切る必要がないので見切りフレーム数を０にする
		pPlayer->ExecuteUpSpace(); // 手刀演出させる
		CTarget* pTarget = pPlayer->GetTarget(); // ターゲットへのポインタ
		CTransparent_Info* pTargetTransparentInfo = pTarget->GetTransparent_Info(); // ターゲットの透明度情報
		pTargetTransparentInfo->SetSpeed(-pTargetTransparentInfo->GetValue() / 10); // 残りの透明度を１０フレームで全て減らす

		// 召喚マネージャーへのポインタ取得
		CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));

		// 召喚マネージャーの召喚開始状態へのポインタ
		CSummon_Manager_State_Start* pSummonManagerState_Start = dynamic_cast<CSummon_Manager_State_Start*>(pSummonManager->GetState());

		if (pSummonManagerState_Start)
		{
			CSummonGroupSelector* pSummonGroupSelector = pSummonManagerState_Start->GetSummonGroupSelector(); // 召喚グループ選択へのポインタ
			int nDifficultyScore = pSummonGroupSelector->GetDifficultyScore(); // 難易度スコア
			pSummonManagerState_Start->GetSummonGroupSelector()->SetDifficultyScore(nDifficultyScore - 4);
		}
		else
		{
			// 召喚開始状態がないのでなにもしない
		}

		pPlayer->SetAirPower(nAirPower - nHandSwordCost); // 気を減らす

		CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // デフォルトレイヤーの先頭リストを取得

		// 障害物とゾーンをリストから検索し消す
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();// 次のオブジェクトを取得

			// 種類が障害物なら
			if (pObj->GetType() == CObject::TYPE::OBSTACLE || pObj->GetType() == CObject::TYPE::HOLEOBSTACLE)
			{
				CObstacle* pObstacle = dynamic_cast<CObstacle*>(pObj);

				// 障害物がなかったので次にリストを進める
				if (pObstacle == nullptr)
				{
					pObj = pNext;
					continue;
				}
				// 障害物が破壊可能なら破棄
				if (pObstacle->GetCanbeDestroy())
				{
					pObstacle->SetRelease();
				}
			}
			// 種類がゾーンなら
			else if (pObj->GetType() == CObject::TYPE::ZONE)
			{
				pObj->SetRelease(); // 破棄する
			}

			pObj = pNext;//リストを次に進める
		}

		// 戻る状態に変更
		pPlayer->SetState(DBG_NEW CPlayerState_Returning(pPlayer));

		return true;
	}

	return false;
}

//============================================================================
// 位置固定処理
//============================================================================
void CPlayerState_BeforeThrust::FixedPos(CPlayer* pPlayer)
{
	D3DXVECTOR3 SelfPos = pPlayer->GetPos(); // 自身の位置
	// ゲームカメラへのポインタを取得
	CCamera* pCamera = &CCamera_Manager::GetInstance()->RefSelectCamera();
	Calculation::Pos::ScreenToWorld(&SelfPos, FSCREEN_WIDTH + 100.0f, FSCREEN_HEIGHT - 100.0f, 0.75f, 1280, 720,
		pCamera->GetMtxView(), pCamera->GetMtxProj());

	pPlayer->SetPos(SelfPos); // 位置設定
}

//============================================================================
// 召喚オブジェクトを線形補間させる処理をするかどうか判断する
//============================================================================
void CPlayerState_BeforeThrust::JudgeLerpPosToSummonObj(CPlayer* pPlayer)
{
	// 見切りが開始しているなら処理をしない
	if (m_bStartAnticipation)
	{
		return;
	}

	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // 先頭オブジェクトを取得
	CTarget* pTarget = pPlayer->GetTarget(); // ターゲットへのポインタ

	// スロー状態にするオブジェクトをリストから検索する
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // リストの次のポインタ
		D3DXVECTOR3 OtherTargetPos = VEC3_INIT; // 相手の目標位置
		float fOtherVtxMin_Z = 0.0f; // 相手の最小頂点Z
		CObject::TYPE OtherType = pObj->GetType(); // 相手のタイプ
		CSummonCommonData* pSummonCommonData = nullptr; // 召喚オブジェクト共通データへのポインタ

		// 召喚マネージャーから召喚されるオブジェクト以外ならリストを次に進める
		if (OtherType != CObject::TYPE::OBSTACLE &&
			OtherType != CObject::TYPE::HOLEOBSTACLE &&
			OtherType != CObject::TYPE::ZONE)
		{
			pObj = pNext;
			continue;
		}

		// タイプが「障害物」又は「穴あき障害物」ならその位置を取得する
		if (OtherType == CObject::TYPE::OBSTACLE ||
			OtherType == CObject::TYPE::HOLEOBSTACLE)
		{
			CObstacle* pObstacle = dynamic_cast<CObstacle*>(pObj); // 障害物へのポインタ
			OtherTargetPos = pObstacle->GetPos(); // 相手の位置取得
			fOtherVtxMin_Z = pObstacle->GetVtxMin().z; // 相手の最小頂点取得
			pSummonCommonData = &pObstacle->GetSummonCommonData(); // 召喚オブジェクト共通データへのポインタ取得
		}
		// タイプが「ゾーン」ならその位置を取得する
		else if (OtherType == CObject::TYPE::ZONE)
		{
			CZone* pZone = dynamic_cast<CZone*>(pObj); // ゾーンへのポインタ
			OtherTargetPos = pZone->GetPos(); // 相手の位置取得
			pSummonCommonData = &pZone->GetSummonCommonData(); // 召喚オブジェクト共通データへのポインタ取得
		}

		// 召喚されたオブジェクトの手前側がターゲットのZ値の近くまで来たら見切り開始
		if (OtherTargetPos.z + fOtherVtxMin_Z < pTarget->GetPos().z + s_fSummonObjectSlowRange)
		{
			m_bStartAnticipation = true;
		}

		pObj = pNext; // リストを次に進める
	}

	if (m_bStartAnticipation)
	{
		SetLerpPosToSummonObj(pPlayer);
	}
}

//============================================================================
// 召喚オブジェクトの位置を線形補間させる
//============================================================================
void CPlayerState_BeforeThrust::SetLerpPosToSummonObj(CPlayer* pPlayer)
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // 先頭オブジェクトを取得

	// ターゲットの透明度
	CTransparent_Info* pTargetTransParentInfo = pPlayer->GetTarget()->GetTransparent_Info();
	pTargetTransParentInfo->SetMaxValue(m_nAnticipationFrame);
	pTargetTransParentInfo->SetValue(m_nAnticipationFrame);
	pTargetTransParentInfo->SetSpeed(-1); // 値を減らす速度
	pTargetTransParentInfo->SetUse(true); // ターゲットの透明度を使用する

	// スロー状態にするオブジェクトをリストから検索する
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // リストの次のオブジェクト
		const CObject::TYPE OtherType = pObj->GetType();     // 相手のタイプ
		CSummonCommonData* pOtherSummonCommonData = nullptr; // 召喚オブジェクト共通データへのポインタ
		D3DXVECTOR3 OtherPos = VEC3_INIT; // 相手の位置

		// 召喚マネージャーから召喚されるオブジェクト以外ならリストを次に進める
		if (OtherType != CObject::TYPE::OBSTACLE &&
			OtherType != CObject::TYPE::HOLEOBSTACLE &&
			OtherType != CObject::TYPE::ZONE)
		{
			pObj = pNext;
			continue;
		}

		// タイプが「障害物」又は「穴あき障害物」ならその位置を取得する
		if (OtherType == CObject::TYPE::OBSTACLE ||
			OtherType == CObject::TYPE::HOLEOBSTACLE)
		{
			CObstacle* pObstacle = dynamic_cast<CObstacle*>(pObj); // 障害物へのポインタ
			pOtherSummonCommonData = &pObstacle->GetSummonCommonData(); // 召喚オブジェクト共通データへのポインタ取得
			OtherPos = pObstacle->GetPos(); // 相手の位置取得
		}
		// タイプが「ゾーン」ならその位置を取得する
		else if (OtherType == CObject::TYPE::ZONE)
		{
			CZone* pZone = dynamic_cast<CZone*>(pObj); // ゾーンへのポインタ
			pOtherSummonCommonData = &pZone->GetSummonCommonData(); // 召喚オブジェクト共通データへのポインタ取得
			OtherPos = pZone->GetPos(); // 相手の位置手刀区
		}

		pOtherSummonCommonData->GetMoveInfo().SetUseMove(false);
		CVec3LerpInfo& LerpPosInfo = pOtherSummonCommonData->GetLerpPosInfo();   // 召喚オブジェクト共通データ位置線形補間情報取得
		D3DXVECTOR3 OtherMove = pOtherSummonCommonData->GetMoveInfo().GetMove(); // 召喚オブジェクト共通データ移動量取得
		D3DXVec3Normalize(&OtherMove, &OtherMove); // 移動量を正規化

		// 移動方向に向かって指定した距離まで線形補間させます
		LerpPosInfo.SetInitialize(m_nAnticipationFrame, OtherPos, OtherPos + OtherMove * s_fSummonObjLerpLength);

		pObj = pNext; // リストを次に進める
	}
}

//============================================================================
// 目的地決定処理
//============================================================================
void CPlayerState_BeforeThrust::DecisionGoal(CPlayer* pPlayer)
{
	CCamera * pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game"); // 現在のカメラへのポインタ
	CTarget* pTarget = pPlayer->GetTarget(); // ターゲットへのポインタ
	D3DXVECTOR3 TargetPos = pPlayer->GetTarget()->GetPos(); // ターゲットの位置

	ClearLerpPosToSummonObj(); // 召喚オブジェクト位置線形補間を解除する
	StopObstacleAddRot();      // 障害物の向きの加算を止める

    // カメラを目標位置へズーム状態にする
	pCamera->ChangeState(
		DBG_NEW CCamera_State_TargetZoom(TargetPos, 50.0f, s_fCameraZoom_PosAdjust, s_fCameraZoom_DistanceAdjust, false, pCamera));

	// プレイヤーのターゲットを操作不可能にし、カメラをターゲット基準にしない
	pTarget->SetUseControl(false);
	pTarget->SetUseAdjustCameraPos(false);
	pTarget->SetUseMove(false);

	// ターゲットの位置を狙って突き状態に変える
	pPlayer->SetState(DBG_NEW CPlayerState_Thrusting(pPlayer, TargetPos));

	// ターゲットの残りの透明度を１０フレームで全て減らす
	CTransparent_Info* pTargetTransparentInfo = pTarget->GetTransparent_Info();
	pTargetTransparentInfo->SetSpeed(-pTargetTransparentInfo->GetValue() / 10);
}

//============================================================================
// 目的地を決める操作を行う
//============================================================================
bool CPlayerState_BeforeThrust::ControlDecisionGoal()
{
	// キー入力情報へのポインタ取得
	CInputKeyboard* pInputKeyboard = CManager::GetManager()->GetKeyboard();
	auto* pPad = CManager::GetManager()->GetPad(); // パッド入力情報

	// トリガーボタンを押したら目的地を決定
	if (pInputKeyboard->GetTrigger(DIK_J) ||
		pPad->GetTrigger(CInputPad::JOYKEY::A))
	{
#if 0
		// 攻撃SE再生
		WORD wRand = rand() % 3;
		CSound::LABEL Label = CSound::LABEL::ATK_0;
		switch (wRand)
		{
		case 0:		Label = CSound::LABEL::ATK_0;	break;
		case 1:		Label = CSound::LABEL::ATK_1;	break;
		case 2:		Label = CSound::LABEL::ATK_2;	break;
		default:	Label = CSound::LABEL::ATK_0;	break;
		}
		CSound::GetInstance()->Play(Label);
#endif
		return true;
	}

	// 見切りフレーム数がなくなったら自動的に目的地を決定
	if (m_nAnticipationFrame == 0)
	{
		return true;
	}

	return false;
}

//============================================================================
// 召喚オブジェクト線形補間処理を解除する
//============================================================================
void CPlayerState_BeforeThrust::ClearLerpPosToSummonObj()
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // 先頭オブジェクトを取得

	// スロー状態にするオブジェクトをリストから検索する
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // リストの次のオブジェクト
		const CObject::TYPE OtherType = pObj->GetType();     // 相手のタイプ
		CSummonCommonData* pOtherSummonCommonData = nullptr; // 召喚オブジェクト共通データへのポインタ

		// 召喚マネージャーから召喚されるオブジェクト以外ならリストを次に進める
		if (OtherType != CObject::TYPE::OBSTACLE &&
			OtherType != CObject::TYPE::HOLEOBSTACLE &&
			OtherType != CObject::TYPE::ZONE)
		{
			pObj = pNext;
			continue;
		}

		// タイプが「障害物」又は「穴あき障害物」ならその位置を取得する
		if (OtherType == CObject::TYPE::OBSTACLE ||
			OtherType == CObject::TYPE::HOLEOBSTACLE)
		{
			CObstacle* pObstacle = dynamic_cast<CObstacle*>(pObj); // 障害物へのポインタ
			pOtherSummonCommonData = &pObstacle->GetSummonCommonData(); // 召喚オブジェクト共通データへのポインタ取得
        }
		// タイプが「ゾーン」ならその位置を取得する
		else if (OtherType == CObject::TYPE::ZONE)
		{
			CZone* pZone = dynamic_cast<CZone*>(pObj); // ゾーンへのポインタ
			pOtherSummonCommonData = &pZone->GetSummonCommonData(); // 召喚オブジェクト共通データへのポインタ取得
		}

		// 移動させる
		pOtherSummonCommonData->GetMoveInfo().SetUseMove(true);

		// 位置の線形補間を解除
		pOtherSummonCommonData->GetLerpPosInfo().SetUse(false);

		pObj = pNext; // リストを次に進める
	}

	SummonObjSlow(0);     // 召喚オブジェクトをスロー状態にする
	SummonObjClearSlow(); // 召喚オブジェクトスロー状態をこの関数で徐々に解除
}

//============================================================================
// 障害物の回転を止める
//============================================================================
void CPlayerState_BeforeThrust::StopObstacleAddRot()
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // 先頭オブジェクトを取得
    // スロー状態にするオブジェクトをリストから検索する
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // リストの次のポインタ
		CObject::TYPE OtherType = pObj->GetType(); // 相手のタイプ

		// タイプが「障害物」「穴あき障害物」以外ならリストを次に進める
		if (OtherType != CObject::TYPE::OBSTACLE &&
			OtherType != CObject::TYPE::HOLEOBSTACLE)
		{
			pObj = pNext;
			continue;
		}

		// 障害物にダウンキャスト
		CObstacle* pObstacle = dynamic_cast<CObstacle*>(pObj);

		// 回転を止める
		pObstacle->SetUseAddRot(false);

		pObj = pNext; // リストを次に進める
	}
}

//============================================================================
// 召喚オブジェクトをスロー状態にする
//============================================================================
void CPlayerState_BeforeThrust::SummonObjSlow(int nSlowFrame)
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // 先頭オブジェクトを取得

// リストを検索する
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // リストの次のオブジェクトを取得

		// タイプが召喚された物だったら６０フレーム遅くする
		if (pObj->GetType() == CObject::TYPE::OBSTACLE ||
			pObj->GetType() == CObject::TYPE::ZONE ||
			pObj->GetType() == CObject::TYPE::HOLEOBSTACLE)
		{
			if (nSlowFrame > 0)
			{
				pObj->GetTimeScaleInfo()->SetTimeScaleChangeFrame(0.02f, nSlowFrame);
			}
			else
			{
				pObj->GetTimeScaleInfo()->SetTimeScale(0.02f);
			}
		}

		pObj = pNext; // リストを次に進める
	}
}

//============================================================================
// 召喚オブジェクトのスロー状態を解除
//============================================================================
void CPlayerState_BeforeThrust::SummonObjClearSlow()
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // 先頭オブジェクトを取得

    // リストを検索し、召喚オブジェクトのタイムスケールを戻す
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // リストの次のオブジェクトを取得

		// 徐々に元のタイムスケールに戻す
		if (pObj->GetType() == CObject::TYPE::OBSTACLE ||
			pObj->GetType() == CObject::TYPE::ZONE ||
			pObj->GetType() == CObject::TYPE::HOLEOBSTACLE)
		{
			pObj->GetTimeScaleInfo()->SetGoalTimeScale(1.0f, 0.1f);
		}

		pObj = pNext; // リストを次に進める
	}
}
