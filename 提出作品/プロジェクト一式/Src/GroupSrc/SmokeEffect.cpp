//==============================================
// 
// ポストエフェクト[煙] <SmokeEffect.cpp>
// Auther 木田威吹
// 
//==============================================

#include "SmokeEffect.h"   // 元ヘッダー使用
#include "manager.h"    // マネージャーを使用
#include "renderer.h"   // デバイス取得
#include "game.h"       // ゲームのポインタを使用
#include "SummonGroupSelector.h"  // 召喚グループ管理クラスを使用
#include "summon_manager_state_start.h"  // 召喚マネージャー状態クラスの仕様

//====================================
// 静的メンバ宣言
//====================================
bool CSmokeEffect::s_bUseDraw = false; // 描画するかどうか

//====================================
// コンストラクタ
//====================================
CSmokeEffect::CSmokeEffect()
{
	m_pEffect = nullptr;   // エフェクトの初期化
	m_noiseTex = nullptr;  // テクスチャの初期化
	m_quadVB = nullptr;    // バッファの初期化
    m_time = 0.0f;    // タイムの初期化
	m_Opacity = 1.0f; // 濃度の初期化
	m_enableEffect = false;  // エフェクトの表示状態
	m_bActive = true;
	s_bUseDraw = true; // 描画を使用する
}
//====================================
// デストラクタ
//====================================
CSmokeEffect::~CSmokeEffect()
{
}
//====================================
// 初期設定
//====================================
HRESULT CSmokeEffect::Init()
{
	// デバイスの取得
	auto pDevice = CRenderer::GetDeviece();

	if (FAILED(CObject_2D::Init()))
	{
		return E_FAIL;
	}

	// シェーダーの読み込み
#ifdef _DEBUG
	InitSmokeEffect(pDevice, "Src/Shader/fx/SmokeEffect.fx");
#else
	InitSmokeEffect(pDevice, "Src/Shader/fxo/SmokeEffect.fxo");
#endif // _DEBUG


	return S_OK;
}
//====================================
// 終了処理
//====================================
void CSmokeEffect::Uninit()
{
	// エフェクトの破棄
	if (m_pEffect) {
		m_pEffect->Release();
		m_pEffect = nullptr;
	}
	// テクスチャの破棄
	if (m_noiseTex) {

		m_noiseTex->Release();
		m_noiseTex = nullptr;
	}
	// バッファの破棄
	if (m_quadVB) {
		m_quadVB->Release();
		m_quadVB = nullptr;
	}

}
//====================================
// 更新処理
//====================================
void CSmokeEffect::Update()
{
#ifdef _DEBUG
	// ImGuiのやつ
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin(u8"霧の設定"))
	{
		ImGui::Checkbox(u8"シェーダーを表示", &m_enableEffect);

		ImGui::Text(u8"霧の濃さの設定");

		ImGui::SliderFloat(u8"濃さ", &m_Opacity, 1.0f, 5.5f);

		ImGui::End();
	}
#endif

	UpdateSmokeEffect(0.05f);  // シェーダーの更新
}
//====================================
// 描画処理
//====================================
void CSmokeEffect::Draw()
{
	if (s_bUseDraw)
	{
		// デバイスの取得
		auto pDevice = CRenderer::GetDeviece();

		RenderSmokeEffect(pDevice);
	}
}
//====================================
// 生成処理
//====================================
CSmokeEffect* CSmokeEffect::Create()
{
	CSmokeEffect* pSmoke = DBG_NEW CSmokeEffect();
	pSmoke->Init();
	return pSmoke;
}

//====================================
// 描画するかどうか設定
//====================================
void CSmokeEffect::SetUseDraw(bool bUse)
{
	s_bUseDraw = bUse;
}

//====================================
// シェーダーの初期化
//====================================
bool CSmokeEffect::InitSmokeEffect(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
	m_fxFilePath = fxFile; // パス保存

	LPD3DXBUFFER pErr = nullptr;
#ifdef _DEBUG
	// デバック時
	HRESULT hr = D3DXCreateEffectFromFileA(
		device, fxFile, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_pEffect, &pErr);
#else	
	// リリース時にfxoを読み込む
	HRESULT hr = D3DXCreateEffectFromFileA(
		device,
		fxFile, // .fx ではなく .fxo を指定
		nullptr,
		nullptr,
		D3DXFX_NOT_CLONEABLE,  // バイナリから読み込むときのフラグ(意味：このエフェクトは複製不可である)
		nullptr,
		&m_pEffect,
		&pErr);
#endif	

	// 読み込みエラーがあった場合エラー表示を出す
	if (FAILED(hr)){
		if (pErr){
			MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Compile Error", MB_OK);
			pErr->Release();
			pErr = nullptr;
		}
		return false;
	}

    // ノイズテクスチャ読み込み
    D3DXCreateTextureFromFile(device, "Data/TEXTURE/fireNoice.png", &m_noiseTex);

    // スクリーンクアッド作成
    InitScreenQuad(device); 

	return true;
}
//====================================
// シェーダーの更新
//====================================
void CSmokeEffect::UpdateSmokeEffect(float deltaTime)
{
	m_time += deltaTime;  // アニメーションの速度を変数に入れる

	// 召喚マネージャーへのポインタ取得
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));

	// サモンマネージャーステートスタートのポインタ情報を取得
	CSummon_Manager_State_Start* pStart = dynamic_cast<CSummon_Manager_State_Start*>(pSummonManager->GetState());

	if (pStart)
	{
		// サモングループセレクターのポインタを取得
		CSummonGroupSelector* pSelecter = dynamic_cast<CSummonGroupSelector*>(pStart->GetSummonGroupSelector());

		if (pStart->GetStartPhase()) {  // フェーズ中の時
			m_enableEffect = true;  // 霧を起動

			// 召喚グループ選択へのポインタが存在しているなら霧の濃さの設定を行う
			if (pSelecter)
			{
				// 難易度によって霧の濃さが変わる
				if (pSelecter->GetDifficultyRank() == CSummon_Manager::DIFFICULTY::EASY) {
					m_Opacity = 2.0f;
				}
				else if (pSelecter->GetDifficultyRank() == CSummon_Manager::DIFFICULTY::NORMAL) {
					m_Opacity = 3.5f;
				}
				else if (pSelecter->GetDifficultyRank() == CSummon_Manager::DIFFICULTY::HARD) {
					m_Opacity = 5.5f;
				}
				else {

				}
			}
		}
		else {  // フェーズ中ではないとき
			m_enableEffect = false;
		}
	}
	// 召喚開始状態へのポインタが存在していないので霧の濃さを固定にする
	else{
		m_Opacity = 0.0f;
	}

	// パラメーターの設定
	m_pEffect->SetFloat("g_Time", m_time);  // アニメーションタイム
	m_pEffect->SetFloat("g_Opacity", m_Opacity);  // 濃さ


}
//====================================
// シェーダーの描画
//====================================
void CSmokeEffect::RenderSmokeEffect(LPDIRECT3DDEVICE9 device)
{
	if (!m_enableEffect)
	{// エフェクトの表示状態
		return;  // falseの時は描画しない
	}

    CObject_2D::Draw(); // 背景描画

	// 各ブレンド適応
	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); 

	// エフェクトの設定
    m_pEffect->SetTechnique("SmokeEffect");
    m_pEffect->SetTexture("g_NoiseTex", m_noiseTex);   // テクスチャ
    m_pEffect->SetFloat("g_Time", m_time);   // アニメーションタイム
    m_pEffect->SetFloat("g_Scale", 1.5f);    // スケール
    m_pEffect->SetFloat("g_Speed", 0.1f);    // 速度
    m_pEffect->SetFloat("g_Opacity", m_Opacity);  // 濃さ
    m_pEffect->SetFloatArray("g_SmokeColor", D3DXVECTOR3(1.0f, 1.0f, 1.0f), 3);  // 色

    m_pEffect->Begin(nullptr, 0);
    m_pEffect->BeginPass(0);

    // スクリーンクアッド描画
    device->SetStreamSource(0, m_quadVB, 0, sizeof(Vertex));
    device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

    m_pEffect->EndPass();
    m_pEffect->End();

	// ブレンドを戻す
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    device->SetRenderState(D3DRS_ZENABLE, TRUE);

}

//====================================
// ポリゴンの初期化
//====================================
void CSmokeEffect::InitScreenQuad(LPDIRECT3DDEVICE9 device)
{
	device->CreateVertexBuffer(4 * sizeof(Vertex), 0, SMOKE_FVF,
		D3DPOOL_MANAGED, &m_quadVB, nullptr);

	Vertex* v;
	m_quadVB->Lock(0, 0, (void**)&v, 0);

	v[0] = { 0,                (float)SCR_HEIGHT, 0, 1.0f, 0.0f, 1.0f }; // 左下
	v[1] = { 0,                0,                 0, 1.0f, 0.0f, 0.0f }; // 左上
	v[2] = { (float)SCR_WIDTH, (float)SCR_HEIGHT, 0, 1.0f, 1.0f, 1.0f }; // 右下
	v[3] = { (float)SCR_WIDTH, 0,                 0, 1.0f, 1.0f, 0.0f }; // 右上

	m_quadVB->Unlock();
}
