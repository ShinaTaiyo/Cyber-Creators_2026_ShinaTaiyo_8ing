//============================================================================
// 
// タイトル [title.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "title.h"
#include "sound.h"

// 遷移先のシーン情報
#include "prologue.h"
#include "TestScene.h"

// 入力取得用
#include "manager.h"

// カメラ取得用
#include "camera_manager.h"

// テクスチャ取得用
#include "texture_manager.h"

// オブジェクト管理用
#include "object_HUD.h"
#include "object_X.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 静的メンバ変数
//****************************************************
bool CTitle::s_bNextSceneIsTest = false; // 次のシーンをテストシーンにするかどうか

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CTitle::Update()
{
	// 基底クラスの更新処理
	CScene::Update();

	// 葉っぱ発生
	GenerateLeaf();

#ifdef _DEBUG
	// 次のウインドウの位置をサイズを決める
	ImGui::SetNextWindowSize(ImVec2(300, 150));
	ImGui::SetNextWindowPos(ImVec2(900, 150));

	// ガイドなしの時は召喚グループを選べるようにする
	ImGui::Begin(u8"次のシーンをテストシーンにする？");

	// チェックボックスでフラグのオンオフ
	ImGui::Checkbox(u8"しますか？", &s_bNextSceneIsTest);

	ImGui::End();
#endif // _DEBUG

#ifdef _DEBUG
	// 戦闘BGMへトランジション
	CSound::GetInstance()->Transition(CSound::LABEL::BGM_1, CSound::LABEL::BGM_0);
#endif

	// プロローグシーンへ
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
void CTitle::Draw()
{
	// 基底クラスの描画処理
	CScene::Draw();
}

//============================================================================
// 次のシーンへ
//============================================================================
void CTitle::To_Next()
{
	// ロゴを見えない位置へ
	m_pLogo->SetPosTarget({ 640.0f, -300.0f, 0.0f });;

	// 隠れ草を見えない位置へ
	for (unsigned char wCntLoop = 0; wCntLoop < 2; ++wCntLoop)
	{
		Vec3 tmpPosTarget = m_apHideGrass[wCntLoop]->RefVectorInterpolator()->GetPosTarget();
		tmpPosTarget.y = 30.0f;
		m_apHideGrass[wCntLoop]->RefVectorInterpolator()->SetPosTarget(tmpPosTarget);
	}

	// 全ての葉っぱを自然に消す
	for (auto& it : m_vpLeaf)
	{
		it->RefVectorInterpolator()->SetScaleTarget(VEC3_INIT);
	}

	// 全オブジェクトを解放
	//CObject::ReleaseAll();

	// プロローグシーンへ変更
	if (s_bNextSceneIsTest == false)
	{
		CScene_Manager::ChangeScene(CPrologue::Create());
	}
	// テストシーンへ変更
	else
	{
		// 全オブジェクトを解放
		CObject::ReleaseAll();

		CScene_Manager::ChangeScene(CTestScene::Create());
	}
}

//============================================================================
// タイトルを生成
//============================================================================
CTitle* CTitle::Create()
{
	// タイトルのインスタンスを生成
	CTitle* pScene = DBG_NEW CTitle();

	// 生成失敗
	if (pScene == nullptr)
	{
		throw std::runtime_error("Failed to create title scene");
	}

	// 初期化処理
	if (FAILED(pScene->Init()))
	{
		throw std::runtime_error("Failed to initialize title scene");
	}

	// インスタンスのポインタを返す
	return pScene;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CTitle::CTitle() :
	CScene(),
	m_pLogo(nullptr),
	m_apHideGrass{ nullptr, nullptr },
	m_vpLeaf{},
	m_nFrameCounter(0)
{}

//============================================================================
// デストラクタ
//============================================================================
CTitle::~CTitle()
{}

//============================================================================
// 初期設定
//============================================================================
HRESULT CTitle::Init()
{
	s_bNextSceneIsTest = false; // 次のシーンをテストシーンにするかどうかを初期化

	// ロゴHUDを生成、テクスチャのためにポインタコピー
	const JSON& ParamHUD = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\HUD\\Logo.json");
	m_pLogo = CObject_HUD::Create(ParamHUD);

	// ロゴにテクスチャを設定
	const std::string& Key = "Logo";
	m_pLogo->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey(Key));

	// 風景の配置セットを読み込む
	const JSON& LayoutSetForTitle = hws::OpenJsonFile("Data\\JSON\\EXPORT\\Layout_Side.json");

	// 読み込めたときだけ
	if (LayoutSetForTitle != nullptr)
	{
		// オブジェクトを生成
		for (unsigned short wCntLoop = 0; wCntLoop < static_cast<short int>(LayoutSetForTitle["NumElement"]); ++wCntLoop)
		{
			CObject_X::Create(LayoutSetForTitle["Set"][wCntLoop]);
		}
	}
	else
	{
		throw std::runtime_error("Failed to Import 'Layout_Side.json'");
	}

	// 隠れ草を生成
	m_apHideGrass[0] = CObject_X::Create(hws::OpenJsonFile("Data\\JSON\\ObJECT\\X\\Export_HideGrass_0.json"));
	m_apHideGrass[1] = CObject_X::Create(hws::OpenJsonFile("Data\\JSON\\ObJECT\\X\\Export_HideGrass_1.json"));

	// 今回のカメラを設定
	CCamera_Manager::GetInstance()->SetSelectKey("Title");

#ifndef _DEBUG
	// さえずりBGMをかける
	CSound::GetInstance()->Stop(CSound::LABEL::BGM_1);
	CSound::GetInstance()->Play(CSound::LABEL::BGM_0);
#endif

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTitle::Uninit()
{}

//============================================================================
// 葉っぱ発生
//============================================================================
void CTitle::GenerateLeaf()
{
	++m_nFrameCounter;

	if (m_nFrameCounter >= 20)
	{
		m_nFrameCounter = 0;

		// オブジェクトを生成し
		CObject_X* tmpPtr = CObject_X::Create(hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\Generate_Leaf.json"));
		
		// 向きを適当に決定
		Vec3 tmpRot = VEC3_INIT;
		float fRand = hws::GetRandomValue<float>() * 0.1f;
		tmpRot += { fRand, fRand, fRand };
		tmpPtr->SetRot(tmpRot);

		// 位置を適当に決定
		Vec3 tmpPos = 
		{
			hws::GetRandomValue<float>(),
			100.0f + hws::GetRandomValue<float>() * 0.1f,
			50.0f + hws::GetRandomValue<float>() * 0.25f
		};
		tmpPtr->SetPos(tmpPos);
		tmpPtr->RefVectorInterpolator()->SetPosTarget(tmpPos);

		// 描画前に更新適応
		tmpPtr->Update();

		// コンテナに追加
		m_vpLeaf.push_back(tmpPtr);
	}

	// 全ての位置を動かす
	for (auto& it : m_vpLeaf)
	{
		// クルクル
		Vec3 tmpRot = it->GetRot();
		float fAdderRot = 0.05f;
		tmpRot.x += fAdderRot;
		tmpRot.y += fAdderRot;
		tmpRot.z += fAdderRot;
		it->SetRot(tmpRot);

		// ゆらゆら
		Vec3 tmpPos = it->GetPos();
		tmpPos += 
		{
			-3.0f + (3.0f * cosf(tmpRot.y)),
			-3.0f + (3.0f * cosf(tmpPos.x * 0.1f)),
			0.0f
		};
		it->RefVectorInterpolator()->SetPosTarget(tmpPos);
	}

	// 消滅
	for (auto it = m_vpLeaf.begin(); it != m_vpLeaf.end(); )
	{
		if ((*it)->GetPos().y < -10.0f)
		{
			(*it)->SetRelease();
			it = m_vpLeaf.erase(it);
			continue;
		}

		++it;
	}
}