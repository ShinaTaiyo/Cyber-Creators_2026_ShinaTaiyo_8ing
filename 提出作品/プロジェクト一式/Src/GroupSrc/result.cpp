//============================================================================
// 
// リザルト [result.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "result.h"
#include "sound.h"

// 遷移先のシーン情報
#include "title.h"

// 入力取得用
#include "manager.h"

// フォグ設定用
#include "renderer.h"

// カメラ取得用
#include "camera_manager.h"

// オブジェクト管理用
#include "texture_manager.h"
#include "object_HUD.h"
#include "object_X.h"
#include "object_parts.h"
#include "motion_set.h"

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
// 更新処理
//============================================================================
void CResult::Update()
{
	// 基底クラスの更新処理
	CScene::Update();

	// フェーズの更新処理
	UpdatePhase();

	// タイトル画面へ
	auto* pPad = CManager::GetPad();
	if ((CManager::GetManager()->GetKeyboard()->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputPad::JOYKEY::A) ||
		pPad->GetTrigger(CInputPad::JOYKEY::START)) && m_nPhase == 7)
	{
		To_Next();
	}
}

//============================================================================
// 描画処理
//============================================================================
void CResult::Draw()
{
	// 基底クラスの描画処理
	CScene::Draw();
}

//============================================================================
// 次のシーンへ
//============================================================================
void CResult::To_Next()
{
	// 敵の解放予約
	if (m_pEnemy)
	{
		m_pEnemy->SetRelease();
	}

	// 疑似床の解放予約
	if (m_pFakeField)
	{
		m_pFakeField->SetRelease();
	}

	// 山モデルの解放予約
	if (m_pMountain)
	{
		m_pMountain->SetRelease();
	}

	// 全オブジェクトの死亡フラグを発動する
	CObject::AllSetRelease();

	// タイトルシーンへ変更
	CScene_Manager::ChangeScene(CTitle::Create());
}

//============================================================================
// リザルトを生成
//============================================================================
CResult* CResult::Create()
{
	// リザルトのインスタンスを生成
	CResult* pScene = DBG_NEW CResult();

	// 生成失敗
	if (pScene == nullptr)
	{
		throw std::runtime_error("Failed to create result scene");
	}

	// 初期化処理
	if (FAILED(pScene->Init()))
	{
		throw std::runtime_error("Failed to initialize result scene");
	}

	// インスタンスのポインタを返す
	return pScene;
}

//============================================================================
// 山モデルを引継ぎ
//============================================================================
void CResult::SetMountain(CObject_X* p)
{
	if (!p)
	{
		return;
	}

	// ポインタを引き継ぐ
	m_pMountain = p;

	// 山(可壊)モーションセット生成
	if (!m_pMountBreakable)
	{
		const JSON& ParamMountain = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\MOTIONSET\\Mount_Breakable.json");
		m_pMountBreakable = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, ParamMountain);

		// 初期値の一部を、引き継いだ山モデルのもので上書き
		m_pMountBreakable->SetNowMotion(1);
		m_pMountBreakable->SetPos(m_pMountain->GetPos());
		m_pMountBreakable->GetParentParts()->SetScaleOffset(m_pMountain->GetScale());
	}

	// 山モデルの表示をとりやめ
	m_pMountain->SetScale(VEC3_INIT);
}

//============================================================================
// 
// protectedメンバ
// 
//============================================================================

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CResult::CResult() :
	CScene(),
	m_nPheseComboCntKeep(-1),
	m_pAtk(nullptr),
	m_pTY(nullptr),
	m_pEnemy(nullptr),
	m_pHuman(nullptr),
	m_pFakeField(nullptr),
	m_pMountain(nullptr),
	m_pMountBreakable(nullptr),
	m_nPhase(0),
	m_nFrameCounter(0)
{}

//============================================================================
// デストラクタ
//============================================================================
CResult::~CResult()
{}

//============================================================================
// 初期設定
//============================================================================
HRESULT CResult::Init()
{
	/* ゲーム用の天球を生成 */
	const JSON& ParamSkyDome = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Game_SkyDome.json");
	CObject_X::Create(ParamSkyDome);

	// レンダラーのポインタ
	CRenderer* pRenderer = CRenderer::GetRenderer();

	//// フォグの始点をセット
	//pRenderer->SetFogStart(450.0f);

	//// フォグの終点をセット
	//pRenderer->SetFogEnd(700.0f);

	//// フォグのカラーをセット
	//pRenderer->SetFogCol({ 1.0f, 1.0f, 1.0f ,1.0f });

	// 今回のカメラでトランジションを設定
	CCamera_Manager::GetInstance()->SetTransition("Result");

#if 0
	// このカメラに、Roll軸回転制御を合成
	Compose(CCamera_Manager::GetInstance()->RefSelectCamera().RefRotatorRoll(),
		[]() -> up<CRotatorRoll>
		{
			up<CRotatorRoll> up = make_unique<CRotatorRoll>();

			up->SetFlags(RotatorRollFlags::Rolling);
			up->SetSpeed(0.0025f);
			up->SetAngle(D3DX_PI * 0.35f);

			return up;
		});
#endif

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CResult::Uninit()
{}

//============================================================================
// フェーズの更新処理
//============================================================================
void CResult::UpdatePhase()
{
	if (m_nPhase == 0)
	{
		// 敵モーションを棒立ちに変更
		if (m_pEnemy->GetNowMotion() != 0)
		{
			m_pEnemy->SetNowMotion(0);
		}
		else
		{
			// 主人公モーションセットを生成
			if (!m_pHuman)
			{
				m_pHuman = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\MOTIONSET\\Karate_Human.json"));
				m_pHuman->SetNowMotion(1);
				m_pHuman->GetParentParts()->SetScaleOffset({ 0.35f, 0.35f, 0.35f });
				m_pHuman->SetRot({ 0.0f, D3DX_PI, 0.0f });
				m_pHuman->SetPos({ 0.0f, 0.0f, -600.0f });
			}

			//フェーズを更新
			++m_nPhase;
		}
	}
	else if (m_nPhase == 1)
	{
		// カメラが引く

		// 敵がこちらを向く
		Vec3 EnemyRot = m_pEnemy->GetRot();
		ExponentialDecay(EnemyRot.y, 0.0f, 0.1f);

		// 主人公が走ってくる
		Vec3 HumanPos = m_pHuman->GetPos();
		if (HumanPos.z < 1500.0f)
		{
			HumanPos.z += 10.0f;
		}
		else
		{
			// 移動制限を行い
			HumanPos.z = 1500.0f;

			// フェーズを進行
			++m_nPhase;
		}
		m_pHuman->SetPos(HumanPos);
	}
	else if (m_nPhase == 2)
	{
		// 主人公モーションを溜めモーションに変更
		if (m_pHuman->GetNowMotion() != 11)
		{
			m_pHuman->SetNowMotion(11);

			// モーション変更のタイミングでカメラの目標値も変更
			CCamera& refCam = CCamera_Manager::GetInstance()->RefSelectCamera();
			refCam.SetPosTarget({ 0.0f, 0.0f, 1500.0f });
			refCam.SetDistanceTarget(500.0f);

			// 攻撃催促を表示
			const JSON& AtkParam = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\HUD\\Atk.json");
			m_pAtk = CObject_HUD::Create(AtkParam);
			m_pAtk->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("Atk"));
		}
		else
		{
			// 制限時間カウント
			++m_nFrameCounter;

			// 攻撃催促を震わす
			Vec3 posAtk = m_pAtk->GetPos();
			posAtk +=
			{
				hws::GetRandomValue<float>() * 0.01f,
				hws::GetRandomValue<float>() * 0.01f,
				hws::GetRandomValue<float>() * 0.01f
			};
			m_pAtk->SetPos(posAtk);

			// イベントに合わせてボタン入力、または30秒放置で
			if (m_nFrameCounter > 1800 ||
				CManager::GetManager()->GetKeyboard()->GetTrigger(DIK_J) ||
				CManager::GetManager()->GetPad()->GetTrigger(CInputPad::JOYKEY::A))
			{
				// カウントをリセットしフェーズを進行
				m_nFrameCounter = 0;
				++m_nPhase;

				/* 仮 - 突きモーションに対してプレイヤーの位置を修正 */
				Vec3 posHuman = m_pHuman->GetPos();
				posHuman.y += -60.0f;
				m_pHuman->SetPos(posHuman);

				// 主人公を突きモーションに変更
				m_pHuman->SetNowMotion(8);
				
				// 山(可壊)を損壊モーションに変更
				m_pMountBreakable->SetNowMotion(0);

				// ドカ音
				CSound::GetInstance()->Play(CSound::LABEL::DOKA);

				// 攻撃催促を見えなくする
				m_pAtk->SetSizeTarget(VEC3_INIT);
				m_pAtk->SetRotTarget({ 0.0f, 0.0f, -10.0f });
				m_pAtk->SetColTarget(XCOL_INIT);
			}
		}
	}
	else if (m_nPhase == 3)
	{
		SetCamImpact("Charge_0");
	}
	else if (m_nPhase == 4)
	{
		SetCamImpact("Charge_1");
	}
	else if (m_nPhase == 5)
	{
		SetCamImpact("Charge_2");
	}
	else if (m_nPhase == 6)
	{
		// 感謝を表示
		const JSON& TYParam = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\HUD\\TY.json");
		m_pTY = CObject_HUD::Create(TYParam);
		m_pTY->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("TY"));

		// フェーズ進行
		++m_nPhase;
	}
}

//============================================================================
// 溜め解放に合わせたカメラのセット
//============================================================================
void CResult::SetCamImpact(const char* key)
{
	// 1秒カウント
	++m_nFrameCounter;

	if (m_nFrameCounter > 60)
	{
		// カウンターをリセットしフェーズを進行
		m_nFrameCounter = 0;
		++m_nPhase;

		// 主人公を突きモーションに変更
		m_pHuman->SetNowMotion(8);

		// 山(可壊)を損壊モーションに変更
		m_pMountBreakable->SetNowMotion(0);

		// ドカ音
		CSound::GetInstance()->Play(CSound::LABEL::DOKA);

		// 使用カメラを溜め使用時用のモノに変更
		CCamera_Manager::GetInstance()->SetSelectKey(key);
	}
}