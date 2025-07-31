//============================================================================
// 
// ゲーム [game.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "game.h"
#include "sound.h"

// シーン遷移用
#include "manager.h"
#include "object.h"
#include "result.h"

// カメラ取得用
#include "camera_manager.h"

// デバッグ用
#include "renderer.h"

// オブジェクト生成用
#include "GamePlayer.h" // ゲームプレイヤー
#include "obstacle.h"     // 障害物
#include "HoleObstacle.h" // 穴あき障害物
#include "hole.h"     // 穴
#include "zone.h"
#include "number.h"   // 数字
#include "block.h"    // ブロック
#include "target.h"   // ターゲット
#include "logger.h"   // ログ
#include "particle.h" // パーティクル
#include "bgmodel.h"  // 背景モデル
#include "StencilMask.h" // ステンシルマスク

#include "motion_set.h"		// モーションセット用
#include "object_parts.h"		
#include "space.h"			// 空間制御処理用
#include "swaying_player.h"	// 揺れ再生処理用

// その他ステート
#include "summon_manager_state.h" // 召喚状態
#include "summon_manager_state_start.h" // 召喚開始状態
#include "PlayerState_Thrusting.h" // プレイヤー突き状態

// カメラ状態
#include "camera_state_targetzoom.h" // カメラズーム状態

// 情報
#include "move_info.h" // 移動情報にアクセスして動きを止めたい
#include "transparent_info.h"

// マネージャー関連
#include "camera.h"  // カメラ
#include "manager.h" // マネージャー

// スポナー
#include "VisionBlockSpawner.h" // 視野妨害物を召喚するオブジェクト

#include "SummonGroupSelector.h" // 難易度を取得

#include <fstream>
//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 静的メンバ定数の定義
//****************************************************
const D3DXVECTOR3 CGame::POS_HARISENBON = { 0.0f, 0.0f, 500.0f };	// ハリセンボンの座標

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CGame::Update()
{
	// シーンクラスの更新処理
	CScene::Update();

	/* 仮 - 空間切断テスト */
	if (CManager::GetKeyboard()->GetTrigger(DIK_X))
	{
		// このフレームでの空間描画を許可
		CRenderer::GetRenderer()->SetDrawSpace();
	}
	
	/**
	 * Summon_Manager_State派生クラスへの操作を行うためにダウンキャストしているが
	 * 最終的には下の箇所を消去し、直接m_pSuummonManagerを参照してください
	 */
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));
	auto* pTmp = pSummonManager->GetState();
	CSummon_Manager_State_Start* pSummonState = hws::DownCast<CSummon_Manager_State_Start, CSummon_Manager_State>(pTmp);

	// クールタイム時の演出用更新処理
	UpdateInCooltime(*pSummonState);

	// ゲームオーバー条件処理
	GameOver();

#ifdef _DEBUG
	// フェーズ可否判定確認用
	if (pSummonState)
	{
		if (pSummonState->GetPhaseJudgmentFrame())
		{
			if (pSummonState->GetSuccessPhase())
			{
				CLogger::GetInstance()->AddDuration(u8"今回のフェーズは成功！", 10);
			}
			else
			{
				CLogger::GetInstance()->AddDuration(u8"今回のフェーズは失敗・・・", 10);
			}
		}
	}

	// リザルト画面へ
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
	{
		To_Next();
	}
#endif // _DEBUG
	
	// フレーム数をカウントする
	++m_nCntFrame;

#ifdef _DEBUG
	// 戦闘BGMへトランジション
	CSound::GetInstance()->Transition(CSound::LABEL::BGM_0, CSound::LABEL::BGM_1);
#endif // _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CGame::Draw()
{
	// 基底クラスの描画処理
	CScene::Draw();
}

//============================================================================
// 次のシーンへ
//============================================================================
void CGame::To_Next()
{
	/* オブジェクト解放前に召喚マネージャーからState_Startのポインタを取得 */
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));
	auto* pStateTmp = pSummonManager->GetState();
	CSummon_Manager_State_Start* pSummonStateStart = hws::DownCast<CSummon_Manager_State_Start, CSummon_Manager_State>(pStateTmp);

	/* 連速成功回数をコピーしておく */
	int nPhaseComboCnt = pSummonStateStart->GetPhaseComboCnt();

	// 全オブジェクトを解放予約
	CObject::AllSetRelease();

	/* リザルトシーンを生成し、いくつかのデータ引き継ぐ */
	CResult* pResult = CResult::Create();
	pResult->SetPhaseComboCnt(nPhaseComboCnt);	// 連続成功回数
	pResult->SetEnemy(m_pEnemy);				// 敵のモーションセット
	pResult->SetFakeField(m_pFakeField);		// 疑似床のモーションセット
	pResult->SetMountain(m_pMountain);			// 山のモデル

	// リザルトシーンへ変更
	CScene_Manager::ChangeScene(pResult);
}

//============================================================================
// ゲームを生成
//============================================================================
CGame* CGame::Create()
{
	// ゲームを生成
	CGame* pNew = DBG_NEW CGame();

	if (pNew == nullptr)
	{
		assert(false && "ゲームの生成に失敗");
	}

	// ゲームの初期設定
	if (FAILED(pNew->Init()))
	{
		assert(false && "ゲームの初期設定に失敗");
	}

	return pNew;
}

//============================================================================
// オブジェクトの動きを止めるかどうか取得
//============================================================================
//const bool& CGame::GetStopObject()
//{
//	return s_bSTOPOBJECT;
//}
//
//============================================================================
// 
// protectedメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CGame::CGame() :
	CScene{},
	m_bEnd{ false },
	m_nCntFrame{ 0 },
	m_bEditSummon{ true },
	m_pToneAdjust{ nullptr },
	m_pEffectAccumulation{ nullptr },
	m_pSmokeEffect{ nullptr },
	m_pEnemy(nullptr),
	m_pFakeField(nullptr),
	m_pMountain(nullptr),
	m_upSwaying(nullptr)
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CGame::~CGame()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CGame::Init()
{
	// 今回のカメラを選択
	CCamera_Manager::GetInstance()->SetTransition("Game");

	// ゲームのカメラへのポインタを取得
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game");
	pCamera->SetUseDecayState(false);
	m_pToneAdjust = CToneAdjust::Create();
	m_pSmokeEffect = CSmokeEffect::Create();

	CSummon_Manager* pSummonManager = CSummon_Manager::Create(); // 召喚マネージャー生成
	pSummonManager->SetChengeState(DBG_NEW CSummon_Manager_State_Start()); // 召喚マネージャーのステートを「召喚開始状態」にする

	CGamePlayer::Create(D3DXVECTOR3(30.0f, -30.0f, -330.0f)); // ゲームプレイヤー生成

	// ステンシルバッファ用のマスクを生成
	CStencilMask::Create(D3DXVECTOR3(FSCREEN_WIDTH / 2, FSCREEN_HEIGHT / 2, 0.0f));

	// === 変数 ===

	m_pEffectAccumulation = CEffectAccumulation::Create();

	CStencilMask::SetUseStencil(false); // ステンシルバッファは一旦使用しません
	
	/*----------------------------------------------------------------------------------------*/

	// 天球を生成
	const JSON& ParamSkyDome = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Game_SkyDome.json");
	CObject_X::Create(ParamSkyDome);

	// 敵モーションセットを生成
	if (!m_pEnemy)
	{
		const JSON& EnemyMotionSet = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\MOTIONSET\\Karate_Enemy.json");
		m_pEnemy = CMotion_Set::Create(CObject::LAYER::BG, CObject::TYPE::NOALLRELEASE, EnemyMotionSet);
		m_pEnemy->SetPos({ POS_HARISENBON.x, POS_HARISENBON.y, POS_HARISENBON.z * 4.0f });
		m_pEnemy->GetParentParts()->SetScaleOffset({ 0.35f, 0.35f, 0.35f });
		m_pEnemy->SetNowMotion(1);
		m_pEnemy->SetRot({ 0.0f, D3DX_PI, 0.0f });
	}

	// 疑似床モーションセットを生成
	if (!m_pFakeField)
	{
		const JSON& FakeFieldMotionSet = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\MOTIONSET\\Fake_Field.json");
		m_pFakeField = CMotion_Set::Create(CObject::LAYER::BG, CObject::TYPE::NOALLRELEASE, FakeFieldMotionSet);
		m_pFakeField->SetPos({ 0.0f, -200.0f, 0.0f });
		m_pFakeField->SetNowMotion(0x0F);
		m_pFakeField->Update();
		m_pFakeField->SetNowMotion(0x0);
	}

	// 山モデルを生成
	if (!m_pMountain)
	{
		const JSON& ParamMountain = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Mountain.json");
		m_pMountain = CObject_X::Create(ParamMountain);

		// この山モデルはリザルトに引き継ぐため全予約解放の対象外
		m_pMountain->SetType(CObject::TYPE::NOALLRELEASE);
	}

	// レンダラーのポインタ
	CRenderer* pRenderer = CRenderer::GetRenderer();

	// フォグの始点をセット
	pRenderer->SetFogStart(30000.0f);

	// フォグの終点をセット
	pRenderer->SetFogEnd(60000.0f);

	// フォグのカラーをセット
	pRenderer->SetFogCol({ 0.0f, 0.0f, 0.0f ,0.0f });

#ifndef _DEBUG
	// 戦闘BGMをかける
	CSound::GetInstance()->Stop(CSound::LABEL::BGM_0);
	CSound::GetInstance()->Play(CSound::LABEL::BGM_1);
#endif

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CGame::Uninit()
{
	if (m_pEffectAccumulation)
	{
		m_pEffectAccumulation->SetRelease();
		m_pEffectAccumulation = nullptr;
	}
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game"); // カメラへのポインタ
	pCamera->OverwriteView(nullptr); // デフォルト挙動に戻す
}

//============================================================================
// ゲームオーバー
//============================================================================
void CGame::GameOver()
{
	// 召喚マネージャー生成
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));

	// 召喚開始状態へのポインタ取得
	CSummon_Manager_State_Start* pStart = dynamic_cast<CSummon_Manager_State_Start*>(pSummonManager->GetState());

	// 全てのフェーズが終了していたら終わらせる
	if (pStart)
	{
		if (pStart->GetAllPhaseComplete())
		{
			m_bEnd = true;
		};
	}

	// ゲームシーン終了
	if (m_bEnd)
	{
		To_Next();
	}
}

//============================================================================
// クールタイム時の更新
//============================================================================
void CGame::UpdateInCooltime(const CSummon_Manager_State_Start& rSummonState)
{
	// クールタイムの状態をコピー
	bool bIsCooltime = rSummonState.GetSummonCoolTime();
	
	// 難易度用列挙型のエイリアスを定義
	using Diff = CSummon_Manager::DIFFICULTY;

	// 難易度ランクをコピー
	Diff DifficultyRank = rSummonState.GetSummonGroupSelector()->GetDifficultyRank();

	// 状態に応じて処理分岐
	if (bIsCooltime)
	{
#ifdef _DEBUG
		CLogger::GetInstance()->AddSynchronization(u8"クールタイム中");
#endif // _DEBUG

		// 召喚クールタイムの期間に入るたびに、揺れ再生処理を都度再生成し実行します
		if (!m_upSwaying)
		{
			m_upSwaying = make_unique<CSwayingPlayer>();
		}
		else
		{
			// 召喚クールタイムカウントを渡し続ける
			int nCoolTimeCount = rSummonState.GetSummonCoolTimeCnt();
			m_upSwaying->Sway(nCoolTimeCount);
		}

		/* この時、難易度に応じて指定するモーションを切り替える */
		WORD 
			wPower = 0,		// 振りかぶり具合
			wCharge = 0;	// 力の溜め具合

		switch (DifficultyRank)
		{
		case Diff::EASY:	wPower = 9;	wCharge = 10;	break;
		case Diff::NORMAL:	wPower = 2;	wCharge = 11;	break;
		case Diff::HARD:	wPower = 2;	wCharge = 3;	break;
		default:			wPower = 9;	wCharge = 10;	break;
		}

		// 敵のモーション制御 ～ 振りかぶる -> 力を溜める
		if (m_pEnemy->GetNowMotion() != wCharge)
		{
			if (m_pEnemy->GetNowMotion() != wPower)
			{
				// 振りかぶるモーションに変更
				m_pEnemy->SetNowMotion(wPower);

				// 同時に、疑似床を1マス移動させる
				WORD wNowMotion = m_pFakeField->GetNowMotion();

				++wNowMotion;

				// 疑似床の移動制限
				if (wNowMotion >= 0x0F)
				{
					wNowMotion = 0;
				}

				m_pFakeField->SetNowMotion(wNowMotion);
			}
			else if (m_pEnemy->GetStopState())
			{
				// 振りかぶるモーション完了後、溜めモーションに変更
				m_pEnemy->SetNowMotion(wCharge);
			}

			// 敵の向きの補正 ～ 視点の正面へ
			Vec3 Rot = m_pEnemy->GetRot();
			useful::ExponentialDecay(Rot.y, 0.0f, 0.1f);
			m_pEnemy->SetRot(Rot);
		}

		// ① 敵の位置の補正 ～ 初期位置に、特別な溜めモーションの時のみ上昇
		Vec3 Pos = m_pEnemy->GetPos();

		// ② 位置の補正 ～ 全体の補間
		Pos = useful::Lerp(
			Pos,											// (変更対象 - 位置)
			{ POS_HARISENBON.x, Pos.y, POS_HARISENBON.z },	// 昇降を考慮し、y軸以外初期値固定
			0.035f);										// 疑似床の移動速度に沿う割合に固定

		// ③ 位置の補正 ～ 特殊上昇
		if (m_pEnemy->GetNowMotion() == 3)
		{
			useful::ExponentialDecay(
				Pos.y,		// (変更対象 - y軸)
				150.0f,		// 目標の上昇位置
				0.025f);	// 上昇速度
		}

		// ④ 位置の補正 ～ 反映
		m_pEnemy->SetPos(Pos);
	}
	else
	{
		// カメラのビュー上書きを停止するため、解放してしまいデストラクタを呼ぶ
		if (m_upSwaying)
		{
			m_upSwaying.reset();
			m_upSwaying = nullptr;
		}

		// 敵のモーション制御 ～ 物を投げる -> ダッシュして逃げる
		if (m_pEnemy->GetNowMotion() != 1)
		{
			if (m_pEnemy->GetNowMotion() != 4)
			{
				// 投げるモーションに変更
				m_pEnemy->SetNowMotion(4);

				// ヒュン音
				CSound::GetInstance()->Play(CSound::LABEL::HYUN);
			}
			else if (m_pEnemy->GetStopState())
			{
				// 投げるモーション完了後、ダッシュモーションに変更
				m_pEnemy->SetNowMotion(1);
			}
		}
		else
		{
			// 敵の向きの補正 ～ 画面の奥へ
			Vec3 Rot = m_pEnemy->GetRot();
			useful::ExponentialDecay(Rot.y, D3DX_PI, 0.1f);
			m_pEnemy->SetRot(Rot);

			// ① 敵の位置の補正 ～ 少し遠巻きに、かつ浮いていたら下降
			Vec3 Pos = m_pEnemy->GetPos();

			// ② 位置の補正 ～ 全体の補間
			Pos = useful::Lerp(
				Pos,																// (変更対象 - 位置)
				{ POS_HARISENBON.x, POS_HARISENBON.y, POS_HARISENBON.z * 4.0f },	// 逃走を考慮し、z軸以外初期値固定
				0.05f);																// 逃走速度
			
			// ③ 位置の補正 ～ 反映
			m_pEnemy->SetPos(Pos);

			// 召喚回数を参照し、山の縮尺と位置を調整する
			UpdateMountain(rSummonState.GetNowPhase());
		}
	}
}

//============================================================================
// 山更新
//============================================================================
void CGame::UpdateMountain(int nowPhase)
{
	// 初期地点のパラメータを固定
	Vec3
		BeginScale = { 15.0f, 15.0f, 15.0f },
		BeginPos = { 0.0f, -300.0f, 3000.0f };

	// 到達地点のパラメータを固定
	Vec3
		DestScale = { 50.0f, 50.0f, 50.0f },
		DestPos = { 0.0f, -200.0f, 4000.0f };

	// 残りフェーズ回数が減ってきたら
	if (nowPhase >= 18)
	{
		// 急接近
		DestScale *= 1.1f;
		DestPos.z = 3750.0f;
	}
	if (nowPhase >= 19)
	{
		// 急接近
		DestScale *= 1.15f;
		DestPos.z = 3500.0f;
	}

	// 進行割合を算出
	float
		fTargetRate = static_cast<float>(nowPhase) / 20.0f,
		fSpeed = 0.01f;

	// 召喚回数に応じて、目標の縮尺と位置を割り出す
	Vec3
		TargetScale = BeginScale + (DestScale - BeginScale) * fTargetRate,
		TargetPos = BeginPos + (DestPos - BeginPos) * fTargetRate;

	// 最後に、現在の数値を取得する
	Vec3
		Scale = m_pMountain->GetScale(),
		Pos = m_pMountain->GetPos();

	// 現在の数値を目標の数値へ補間する
	useful::ExponentialDecay(Scale, TargetScale, fSpeed);
	useful::ExponentialDecay(Pos, TargetPos, fSpeed);

	// 補間されたパラメータを反映
	m_pMountain->SetScale(Scale);
	m_pMountain->SetPos(Pos);
}