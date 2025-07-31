//================================
//
//「気」の蓄積(EffectAccumulation.cpp)
// Auther kida ibuki
// 
//================================
#include "renderer.h"  // デバイスを使用するため
#include "EffectAccumulation.h"   // 元ヘッダー
#include "manager.h"   // マネージャーの仕様
#include "game.h"      // ゲームのポインタを使用
#include "GamePlayer.h"   // 気の最大パワーを取得するのに使用
#include "summon_manager_state_start.h"  // 召喚マネージャー状態クラスのポインタを使用


//======================================
// コンストラクタ
//======================================
CEffectAccumulation::CEffectAccumulation()
{
	m_sceneRenderTarget = nullptr;   // レンダラーターゲット
	m_fireTexture = nullptr;   // テクスチャ
	m_fireEffect = nullptr;   // エフェクト
    m_fireTime = 0.0f;  // アニメーションの速度
    m_sceneSurface = nullptr;   // サーフェス
    m_fullscreenVB = nullptr;  // フルスクリーンクアッド
    // テクスチャの色
    m_colorR = 1.0f;
    m_colorG = 1.0f;
    m_colorB = 1.0f;

    // 円と尖りの強弱
    m_circlePowor = 0.3f;
    m_sharpnessPowor = 0.1f;
    m_totalDeformPower = 1.2f;

    m_circleSize = 2.0f;  // エフェクトのサイズ(Update内で加算したら徐々に増える。キーを押したときなど)2.0~6.0が理想かも

    m_enableEffect = true;  // エフェクトの表示状態
    m_bActive = true;
    m_bSwitch = true;

    m_pSuummonManager = nullptr;

    // 気のパラメーター用変数の初期化
    baseValue = 1.0f;
    pressCount = 0;
    powerPerPress = 0.05f;   // 押すごとの増加量
    maxPower = 2.0f;


}
//======================================
// デストラクタ
//======================================
CEffectAccumulation::~CEffectAccumulation()
{
}
//======================================
// 初期化処理
//======================================
HRESULT CEffectAccumulation::Init()
{
    SetType(CObject::TYPE::SPIRIT);  // タイプの設定

    // デバイスの取得
    auto pDevice = CRenderer::GetDeviece();

    if (FAILED(CObject_2D::Init()))
    {// 2Dオブジェクトの初期化
        return E_FAIL;
    }

    // シェーダーの読み込み
#ifdef _DEBUG
    InitPostEffect(pDevice, "Src/Shader/fx/EffectAccumulation.fx");  // シェーダーの初期化 & 呼び出し
#else
    InitPostEffect(pDevice, "Src/Shader/fxo/EffectAccumulation.fxo");  // シェーダーの初期化 & 呼び出し
#endif // !_DEBUG

	return S_OK;
}
//======================================
// 終了処理
//======================================
void CEffectAccumulation::Uninit()
{
    ShutdownPostEffect();   // シェーダーの変数破棄

    CObject_2D::Uninit();  // 2Dオブジェクトの破棄

    // 召喚マネージャー終了
    if (m_pSuummonManager != nullptr)
    {
        delete m_pSuummonManager;    // 動的メモリの解放
        m_pSuummonManager = nullptr; // ポインタの初期化
    }

}
//======================================
// 更新処理
//======================================
void CEffectAccumulation::Update()
{
    //CObject_2D::Update();

#ifdef _DEBUG
    // ImGuiのやつ
    ImGui::SetNextWindowSize({ -1, -1 });
    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
    if (ImGui::Begin(u8"気のシェーダー"))
    {
        ImGui::Checkbox(u8"Guiを開く", &m_bActive);
        if (!m_bActive) {

            ImGui::Text(u8"色の調整");
            ImGui::SliderFloat("Color R", &m_colorR, 0.0f, 1.0f);
            ImGui::SliderFloat("Color G", &m_colorG, 0.0f, 1.0f);
            ImGui::SliderFloat("Color B", &m_colorB, 0.0f, 1.0f);

            ImGui::Text(u8"気の強さ");
            ImGui::SliderFloat(u8"円の強さ", &m_circlePowor, 0.0f, 0.5f);
            ImGui::SliderFloat(u8"尖りの強さ", &m_sharpnessPowor, 0.0f, 0.5f);
            ImGui::SliderFloat(u8"全体の強さ", &m_totalDeformPower, 0.0f, 2.0f);

            ImGui::Text(u8"気の大きさ");
            ImGui::SliderFloat(u8"円の大きさ", &m_circleSize, 2.0f, 10.0f);
            ImGui::Checkbox(u8"表示状態", &m_enableEffect);
        }
        ImGui::End();
    }
#endif

    UpdatePostEffect(0.05f);  // シェーダーの更新(引数:アニメーションの速度)
}
//======================================
// 描画処理
//======================================
void CEffectAccumulation::Draw()
{
    // デバイスの取得
    auto pDevice = CRenderer::GetDeviece();

    RenderEffect(pDevice);  // シェーダーの描画
}
//======================================
// 生成処理
//======================================
CEffectAccumulation* CEffectAccumulation::Create()
{
    CEffectAccumulation* pEffect = DBG_NEW CEffectAccumulation();
	pEffect->Init();
	return pEffect;
}
//======================================
// シェーダーの初期化 & 呼び出し
//======================================
bool CEffectAccumulation::InitPostEffect(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
    m_fxFilePath = fxFile; // パス保存

    // テクスチャの作成
    device->CreateTexture(
        TEX_SIZE_X, TEX_SIZE_Y, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
        &m_sceneRenderTarget, nullptr
    );
    m_sceneRenderTarget->GetSurfaceLevel(0, &m_sceneSurface);  // サーフェスレベルの取得

    // 炎テクスチャの読み込み
    D3DXCreateTextureFromFile(device, "Data/TEXTURE/fireNoice.png", &m_fireTexture);

    LPD3DXBUFFER pErr = nullptr;

#ifdef _DEBUG
    // デバック時
    HRESULT hr = D3DXCreateEffectFromFileA(
        device, fxFile, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_fireEffect, &pErr);
#else	
    // リリース時にfxoを読み込む
    HRESULT hr = D3DXCreateEffectFromFileA(
    	device,
        fxFile, // .fx ではなく .fxo を指定
    	nullptr,
    	nullptr,
    	D3DXFX_NOT_CLONEABLE,  // バイナリから読み込むときのフラグ(意味：このエフェクトは複製不可である)
    	nullptr,
    	&m_fireEffect,
    	&pErr);

#endif	
    if (FAILED(hr)){
        if (pErr){
            // fxファイルがコンパイルエラーだったらエラー表示を出す
            MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Compile Error", MB_OK);
            pErr->Release();
        }
        return false;
    }

    // フルスクリーン四角形の作成
    device->CreateVertexBuffer(4 * sizeof(ScreenVertex), 0, FVF_SCREENVERTEX, D3DPOOL_MANAGED, &m_fullscreenVB, nullptr);

    // テクスチャのサイズを指定 + テクスチャ座標も
    ScreenVertex* vertices;
    m_fullscreenVB->Lock(0, 0, (void**)&vertices, 0);
    vertices[0] = { { 0.0f,       0.0f,    0.0f, 1.0f }, { 0.0f, 0.0f } };
    vertices[1] = { { TEX_F_SIZE_X, 0.0f ,   0.0f, 1.0f }, { 1.0f, 0.0f } };
    vertices[2] = { { 0.0f,       TEX_F_SIZE_Y,  0.0f, 1.0f }, { 0.0f, 1.0f } };
    vertices[3] = { { TEX_F_SIZE_X, TEX_F_SIZE_Y,  0.0f, 1.0f }, { 1.0f, 1.0f } };
    m_fullscreenVB->Unlock();

    return true;
}
//======================================
// シェーダーに関する変数の破棄
//======================================
void CEffectAccumulation::ShutdownPostEffect()
{
    // シーンのレンダラーターゲットの破棄
    if (m_sceneRenderTarget)
    {
        m_sceneRenderTarget->Release();
        m_sceneRenderTarget = nullptr;
    }
    // テクスチャの破棄
    if (m_fireTexture)
    {
       m_fireTexture->Release();
       m_fireTexture = nullptr;
    }
    // エフェクトの破棄
    if (m_fireEffect) 
    {
        m_fireEffect->Release();
        m_fireEffect = nullptr;
    }
    // ポリゴンのバッファの破棄
    if (m_fullscreenVB)
    {
        m_fullscreenVB->Release();
        m_fullscreenVB = nullptr;
    }
    // サーフェスの破棄
    if (m_sceneSurface)
    {
        m_sceneSurface->Release();
        m_sceneSurface = nullptr;
    }

}
//======================================
// シェーダーの更新
//======================================
void CEffectAccumulation::UpdatePostEffect(float deltaTime)
{
    m_fireTime += deltaTime;   // アニメーションの速度
    
    
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER)); // ゲームのプレイヤーのポインタを取得

    if (pPlayer == nullptr)
    {// nullチェック
        return;
    }
    int nMaxAirPowor = pPlayer->GetMaxAirPower();  // 気の最大パワー取得
    int nAirPowor = pPlayer->GetAirPower();  // 気の現在のパワー取得
    
    float rawRatio = static_cast<float>(nAirPowor) / static_cast<float>(nMaxAirPowor);  // 割合を求める
    rawRatio = min(rawRatio, 1.0f);  // 上限は1.0以下に制限

    // 線形補間
    float ratio = START_RATIO + (1.0f - START_RATIO) * rawRatio;

    m_circleSize = MAX_SIZE * ratio;  // 気を増やす

    // 召喚マネージャーへのポインタ取得
    CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));

    // サモンマネージャーステートスタートのポインタ情報を取得
    CSummon_Manager_State_Start* pStart = dynamic_cast<CSummon_Manager_State_Start*>(pSummonManager->GetState());

    if (pStart)
    {
        if (pStart->GetPhaseJudgmentFrame())
        {
            if (pStart->GetSuccessPhase())
            {// 成功
                // カウントを増やす
                ++pressCount;

                // 初期値
                currentWave.time = 0.0f;
                currentWave.duration = 0.5f;

                // 波のピーク強度を算出
                float peakPower = baseValue + powerPerPress * (pressCount - 1);
                peakPower = min(peakPower, maxPower);  // 最大値を設定

                // 波の強さの反映
                currentWave.power = peakPower - baseValue;

                // 時間スケールをカウントの回数分増加、最大2.0に制限
                currentWave.timeScale = min(0.4f + pressCount * 0.025f, 2.0f);
                currentWave.active = true;   // 有効化

            }
            else
            {
                // 失敗
            }
        }
    }


    // 有効化かどうか
    if (currentWave.active) {
        // 気のタイムに倍率を加算
        currentWave.time += 0.01f * currentWave.timeScale;

        float t = currentWave.time / currentWave.duration; // 割合を求める 
        if (t >= 1.0f)
        {
            t = 1.0f;  // 元の位置に戻す
            currentWave.active = false;
        }

        // 強さを代入
        float strength = GetWaveStrength(t);

        m_totalDeformPower = baseValue + currentWave.power * strength;  // 強弱を増やす

    }
    if (m_circleSize >= VALUE) {
        // 値が一定値以上になったら強弱を増やす
        m_circlePowor = MAX_POWOR;
        m_sharpnessPowor = MAX_POWOR;

        // 色を指定(現：赤っぽい)
        m_colorR = 0.862f;
        m_colorG = 0.078f;
        m_colorB = 0.235f;
    }
    else {// 一定値以外だったら通常の強弱

        // 色を指定(現：青っぽい色)
        m_colorR = 0.031f;
        m_colorG = 0.968f;
        m_colorB = 0.968f;

        // 通常の強弱
        m_circlePowor = NORMAL_POWOR_CIRCLE;
        m_sharpnessPowor = NORMAL_POWOR_SHARPNESS;
    }

    D3DXVECTOR4 color = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);  // 色の設定

    // エフェクトパラメーターの設定
    m_fireEffect->SetFloat("circlePowor", m_circlePowor);   // 円の強弱をimguiで変更可
    m_fireEffect->SetFloat("sharpnessPowor", m_sharpnessPowor);   // 尖りの強弱
    m_fireEffect->SetFloat("totalDeformPower", m_totalDeformPower);  // 全体の強弱を増やす

    m_fireEffect->SetVector("flameColor", &color);  // テクスチャの色
    m_fireEffect->SetFloat("size", m_circleSize);  // ポストエフェクトのサイズ(増減を変えれる)

}
//======================================
// シェーダーの描画
//======================================
void CEffectAccumulation::RenderEffect(LPDIRECT3DDEVICE9 device)
{
    if (!m_enableEffect)
    {// エフェクトの表示状態
        return;  // falseの時は描画しない
    }
    // バックバッファ取得
    LPDIRECT3DSURFACE9 backBuffer = nullptr;
    device->GetRenderTarget(0, &backBuffer);

    device->SetRenderTarget(0, m_sceneSurface);  // シーンサーフェスをセット

    // 画面のクリア
    device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

    CObject_2D::Draw();  // 2D描画

    // アルファブレンド設定
    device->SetRenderState(D3DRS_ZENABLE, FALSE); // 深度値で描画順を変えないようにし、完全に描画された順番にする
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); //  アルファブレンドを加算合成に
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // 描画する色のブレンド係数を「ソースのアルファ値」に設定
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // ブレンド係数を１ーアルファ値に

    // テクスチャのアルファも適応
    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); // 引数１のアルファ値をそのまま使います
    device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);   // テクスチャ画像のアルファ値を引数１とします。

    // バックバッファの設定
    device->SetRenderTarget(0, backBuffer);


    D3DXVECTOR4 color = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);
    // エフェクトパラメーターの設定
    m_fireEffect->SetTexture("screenTex", m_sceneRenderTarget);  // シーンのレンダラーターゲット
    m_fireEffect->SetTexture("fireTex", m_fireTexture);   // テクスチャ
    m_fireEffect->SetFloat("time", m_fireTime);    // アニメーションタイム
    m_fireEffect->SetFloat("circlePowor", m_circlePowor);   // 円の強弱
    m_fireEffect->SetFloat("sharpnessPowor", m_sharpnessPowor);  // 尖りの強弱
    m_fireEffect->SetVector("flameColor", &color);   // テクスチャの色
    m_fireEffect->SetFloat("size", m_circleSize);    // エフェクトのサイズ
    m_fireEffect->SetFloat("totalDeformPower", m_totalDeformPower);  // 全体の強弱を増やす


    m_fireEffect->SetTechnique("TechMain");  // テクニック宣言
    UINT passCount = 0;
    m_fireEffect->Begin(&passCount, 0);
    for (UINT i = 0; i < passCount; ++i){
        m_fireEffect->BeginPass(i);

        // ポリゴン描画
        device->SetFVF(FVF_SCREENVERTEX);
        device->SetStreamSource(0, m_fullscreenVB, 0, sizeof(ScreenVertex));
        device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

        m_fireEffect->EndPass();
    }
    m_fireEffect->End();

    // バックバッファのリセット
    if (backBuffer){
        backBuffer->Release();
        backBuffer = nullptr;
    }

    // 設定を元に戻す
    device->SetRenderState(D3DRS_ZENABLE, TRUE);
    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}
//==========================================
// 波の強さの取得
//==========================================
float CEffectAccumulation::GetWaveStrength(float t)
{
    if (t < 0.5f) {
        float up = t * 0.00002f;
        return EaseInSmooth(up);
    }
    else {
        float down = (t - 0.5f) * 2.0f;
        return 1.0f - down;
    }
}
//==========================================
// イージング関数
//==========================================
float CEffectAccumulation::EaseInSmooth(float t)
{
    return t * t * (2.0f - t);
}
