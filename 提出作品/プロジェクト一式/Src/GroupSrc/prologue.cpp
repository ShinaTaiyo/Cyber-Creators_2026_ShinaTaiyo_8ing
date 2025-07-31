//============================================================================
// 
// プロローグ [prologue.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "prologue.h"
#include "sound.h"

// カメラマネージャー取得用
#include "camera_manager.h"
#include "bezier_manager.h"

// シーン遷移用
#include "manager.h"
#include "object.h"
#include "tutorial.h"

// オブジェクト生成用
#include "bezier_manager.h"
#include "texture_manager.h"
#include "object_HUD.h"
#include "object_X.h"
#include "object_parts.h"
#include "motion_set.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 無名名前空間を定義
//****************************************************
namespace
{
	// 汎用位置
	float g_fFloatTest = 0.0f;
	Vec3 g_PosTest = VEC3_INIT;

	// デバッグ操作用
	void PrologueTest()
	{
		//MIS::MyImGuiShortcut_BeginWindow(u8"プロローグデバッグ", { -1.0f, -1.0f });
		//ImGui::Text(u8"あああああああああああああああああああああああああああああああああああああああああ");
		//ImGui::DragFloat(u8"なんか浮動小数", &g_fFloatTest, 0.1f);
		//MIS::MyImGuiShortcut_DragVector(u8"位置", g_PosTest, 0.1f);
		//MIS::MyImGuiShortcut_ResetVector(u8"位置", g_PosTest);
		//ImGui::End();
	}
}

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CPrologue::Update()
{
	// シーンクラスの更新処理
	CScene::Update();

	// テキストの更新処理
	UpdateText();

#if 1
	// フェーズの更新処理
	UpdatePhase();
#endif

	// チュートリアル画面へ
	auto* pPad = CManager::GetPad();
	if (CManager::GetManager()->GetKeyboard()->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputPad::JOYKEY::A) ||
		pPad->GetTrigger(CInputPad::JOYKEY::START))
	{
		To_Next();
	}
}

//============================================================================
// 描画処理
//============================================================================
void CPrologue::Draw()
{
	// 基底クラスの描画処理
	CScene::Draw();
}

//============================================================================
// 次のシーンへ
//============================================================================
void CPrologue::To_Next()
{
	// 全オブジェクトを解放
	CObject::ReleaseAll();

	// チュートリアルシーンへ変更
	CScene_Manager::ChangeScene(CTutorial::Create());

	// ベジェ曲線マネージャーの明示的な破棄
	CBezier_Manager::GetInstance()->ExplicitRelease();
}

//============================================================================
// プロローグを生成
//============================================================================
CPrologue* CPrologue::Create()
{
	// プロローグを生成
	CPrologue* pNew = DBG_NEW CPrologue();

	if (pNew == nullptr)
	{
		assert(false && "プロローグの生成に失敗");
	}

	// プロローグの初期設定
	if (FAILED(pNew->Init()))
	{
		assert(false && "プロローグの初期設定に失敗");
	}

	return pNew;
}

//============================================================================
// 
// protectedメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CPrologue::CPrologue() :
	CScene(),
	m_nPhase(0),
	m_nFrameCounter(0),
	m_pText(nullptr),
	m_pEnemy(nullptr),
	m_pHuman(nullptr),
	m_pGold(nullptr),
	m_vpLeaves{},
	m_fAlphaRate(0.025f)
{}

//============================================================================
// デストラクタ
//============================================================================
CPrologue::~CPrologue()
{
	// 念のためクリーンアップ
	Uninit();
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CPrologue::Init()
{
	// ベジェ曲線マネージャーの明示的な生成
	CBezier_Manager::GetInstance();

	// ランウェイを生成
	CObject_X* tmpRunway = CObject_X::Create(hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Export_Runway.json"));
	tmpRunway->Update();

	// でかい樹を生成
	CObject_X* tmpBigTree = CObject_X::Create(hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\BigTree.json"));
	tmpBigTree->Update();

	// ロゴHUDを生成
	if (!m_pText)
	{
		const JSON& ParamHUD = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\HUD\\Skip.json");
		m_pText = CObject_HUD::Create(ParamHUD);

		// ロゴにテクスチャを設定
		const std::string& Key = "Skip";
		m_pText->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey(Key));
	}

	// 主人公モーションセットを生成
	if (!m_pHuman)
	{
		m_pHuman = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\MOTIONSET\\Karate_Human.json"));
		m_pHuman->SetNowMotion(0);
		m_pHuman->SetPos({ 0.0f, -20.0f, 0.0f });
		m_pHuman->SetRot({ 0.0f, 0.0f, 0.0f });
	}

	// 金インゴットを生成
	if (!m_pGold)
	{
		const JSON& GoldParam = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Gold.json");
		m_pGold = CObject_X::Create(GoldParam);

		// 画面中央奥に配置
		m_pGold->SetPos({ 0.0f, 150.0f, 30.0f });

		// 描画前に更新
		m_pGold->Update();

		// ドカ音
		CSound::GetInstance()->Play(CSound::LABEL::DOKA);
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CPrologue::Uninit()
{}

//============================================================================
// テキスト更新
//============================================================================
void CPrologue::UpdateText()
{
	float fAlpha = m_pText->GetAlpha();

	fAlpha += m_fAlphaRate;

	if (fabsf(fAlpha) > 1.0f)
	{
		m_fAlphaRate *= -1.0f;
	}

	m_pText->SetAlpha(fAlpha);
}

//============================================================================
// フェーズ更新
//============================================================================
void CPrologue::UpdatePhase()
{
	if (m_nPhase == 0)
	{
		Vec3 GoldPos = m_pGold->GetPos();

		// ゴールドが落ちてくる
		if (GoldPos.y > 0.0f)
		{
			GoldPos.y += -5.0f;
			m_pGold->SetPos(GoldPos);
		}
		else
		{
			// 下降制限
			GoldPos.y += 3.0f;
			m_pGold->SetPos(GoldPos);

			// 落下しきったら次のフェーズへ
			++m_nPhase;
		}
	}
	else if (m_nPhase == 1)
	{
		// カウントを使用
		++m_nFrameCounter;
	
		// 0.5秒経過までカウント
		if (m_nFrameCounter <= 30)
		{
			// その間、振動
			CCamera_Manager::GetInstance()->RefSelectCamera().SetVibration(0.005f);
		}
		else
		{
			// リセットし次のフェーズへ
			m_nFrameCounter = 0;
			++m_nPhase;

			// 敵モーションセットを生成
			if (!m_pEnemy)
			{
				m_pEnemy = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\MOTIONSET\\Karate_Enemy.json"));
				m_pEnemy->SetNowMotion(1);
				m_pEnemy->SetPos({ -150.0f, 13.0f, 0.0f });
				m_pEnemy->SetRot({ 0.0f, D3DX_PI * -0.5f, 0.0f });
			}
		}
	}
	else if (m_nPhase == 2)
	{
		Vec3 EnemyPos = m_pEnemy->GetPos();

		if (EnemyPos.x < 0.0f)
		{
			// 敵を中央まで移動させる
			EnemyPos.x += 1.5f;
			m_pEnemy->SetPos(EnemyPos);
		}
		else
		{
			// 金塊の方向を向く
			m_pEnemy->SetRot({0.0f, D3DX_PI, 0.0f});

			// はみ出さないよう座標固定
			EnemyPos.x += 0.0f;
			m_pEnemy->SetPos(EnemyPos);

			// 中央に到達でモーション変更
			m_pEnemy->SetNowMotion(4);

			// 次のフェーズへ
			++m_nPhase;
		}
	}
	else if (m_nPhase == 3)
	{
		// モーションが停止したら
		if (m_pEnemy->GetStopState())
		{
			// 次のフェーズへ
			++m_nPhase;

			// 向きを進行方向に戻す
			m_pEnemy->SetRot({0.0f, D3DX_PI * -0.5f, 0.0f});

			// 敵のモーションを泥棒ダッシュに変更
			m_pEnemy->SetNowMotion(5);

			// 金塊を消滅させる
			m_pGold->SetScale(VEC3_INIT);
		}
	}
	else if (m_nPhase == 4)
	{
		Vec3 EnemyPos = m_pEnemy->GetPos();

		if (EnemyPos.x < 150.0f)
		{
			// 敵を右端まで移動させる
			EnemyPos.x += 1.5f;
			m_pEnemy->SetPos(EnemyPos);
		}
		else
		{
			// 右へ走り抜けたらフェーズ進行
			++m_nPhase;

			// 敵をランウェイ上に移動
			m_pEnemy->SetPos({ -250.0f, 13.0f, -500.0f });

			// 使用するカメラを変更
			CCamera_Manager::GetInstance()->SetSelectKey("Runway");
		}
	}
	else if (m_nPhase == 5)
	{
		// カウントを使用
		++m_nFrameCounter;

		// 2秒経過までカウント
		if (m_nFrameCounter <= 120)
		{
			Vec3 EnemyPos = m_pEnemy->GetPos();

			if (EnemyPos.x < 250.0f)
			{
				// ランウェイ上で敵が移動
				EnemyPos.x += 1.5f;
				m_pEnemy->SetPos(EnemyPos);
			}

			// カメラを敵の位置へ同期
			CCamera_Manager::GetInstance()->RefSelectCamera().SetPos(EnemyPos);
			CCamera_Manager::GetInstance()->RefSelectCamera().SetPosTarget(EnemyPos);
		}
		else
		{
			// リセットし次のフェーズへ
			m_nFrameCounter = 0;
			++m_nPhase;

			// 使用するカメラを変更
			CCamera_Manager::GetInstance()->SetSelectKey("HumanAppeal");

			// ベジェマネージャーを再生成
			CBezier_Manager::ExplicitRelease();
			CBezier_Manager::SetReloadPath("Data\\JSON\\INITIALIZE\\Bezier_HumanAppeal.json");
			CBezier_Manager::GetInstance();
		}
	}
	else if (m_nPhase == 6)
	{
		// ベジェマネージャーの更新状況に応じて
		if (!CBezier_Manager::GetInstance()->Update())
		{
			/* 更新中 */

			// 軌跡の座標を取得
			const Vec3& TrajectoryPos = CBezier_Manager::GetInstance()->GetPosTracePointOnCurrentBeizier();

			/* HumanAppealカメラテスト */
			CCamera_Manager::GetInstance()->RefSelectCamera().SetPosTarget(TrajectoryPos);
		}
		else
		{
			/* 更新が完了した */

			// 主人公の位置をでかい樹の内部に変更
			m_pHuman->SetPos({ 0.0f, 420.0f, 0.0f });

			// フェーズを進行
			++m_nPhase;
		}
	}
	else if (m_nPhase == 7)
	{
		Vec3 HumanPos = m_pHuman->GetPos();

		// でかい樹から這い出てくる
		if (HumanPos.y < 432.5f)
		{
			HumanPos.y += 0.5f;
			m_pHuman->SetPos(HumanPos);
		}
		else
		{
			// 仮表記を出力
			const JSON& TmpParam = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\HUD\\Tmp.json");
			CObject_HUD* pTmp = CObject_HUD::Create(TmpParam);
			pTmp->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("Tmp"));

			// 上昇制限
			HumanPos.y = 432.5f;
			m_pHuman->SetPos(HumanPos);

			// 主人公にジャンプモーション
			m_pHuman->SetNowMotion(6);

			// 大量に葉を生成
			SetLeaves();

			// フェーズを進行
			++m_nPhase;
		}
	}
	else if (m_nPhase == 8)
	{
		// 大量の葉っぱ更新
		UpdateLeaves();
	}
	else if (m_nPhase == -1)
	{
		// 表示消滅
		float fAlpha = m_pText->GetAlpha();
		fAlpha += -0.05f;
		m_pText->SetAlpha(fAlpha);

		// チュートリアルシーンへ
		if (fAlpha < 0.0f)
		{
			To_Next();
		}
	}
}

//============================================================================
// 葉っぱのセット
//============================================================================
void CPrologue::SetLeaves()
{
	for (unsigned char ucCnt = 0; ucCnt < 20; ++ucCnt)
	{
		// オブジェクトを生成し
		CObject_X* tmpPtr = CObject_X::Create(hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Generate_Leaf.json"));

		// 縮尺を設定
		tmpPtr->SetScale(VEC3_INIT);
		tmpPtr->RefVectorInterpolator()->SetScaleTarget({ 0.25f, 0.25f, 0.25f });

		// 向きを適当に決定
		Vec3 tmpRot = VEC3_INIT;
		float fRand = hws::GetRandomValue<float>() * 0.1f;
		tmpRot += { fRand, fRand, fRand };
		tmpPtr->SetRot(tmpRot);
		
		// 描画前に更新適応
		tmpPtr->Update();

		// コンテナに追加
		m_vpLeaves.push_back(tmpPtr);
	}
}

//============================================================================
// 葉っぱの更新処理
//============================================================================
void CPrologue::UpdateLeaves()
{
	for (auto it : m_vpLeaves)
	{
		// 縮尺を取得
		Vec3 tmpScale = it->GetScale();

		// クルクル
		Vec3 tmpRot = it->GetRot();
		float fAdderRot = 0.05f;
		tmpRot.x += fAdderRot;
		tmpRot.y += fAdderRot;
		tmpRot.z += fAdderRot;
		it->SetRot(tmpRot);

		// 位置変更
		Vec3 tmpPos = it->GetPos();
		if (tmpScale.x < 0.24f)
		{
			// ゆらゆら
			tmpPos +=
			{
				-3.0f + (3.0f * cosf(tmpRot.y)),
				-3.0f + (3.0f * cosf(tmpPos.x * 0.1f)),
				0.0f
			};
		}
		else
		{
			// 主人公と逆向きに吹き飛ぶ
			Vec3 Dir = m_pHuman->GetPos() - tmpPos;
			if (Dir.y < 0.0f) Dir.y *= -1.0f;
			tmpPos += Dir;
		}
		it->RefVectorInterpolator()->SetPosTarget(tmpPos);
	}
}