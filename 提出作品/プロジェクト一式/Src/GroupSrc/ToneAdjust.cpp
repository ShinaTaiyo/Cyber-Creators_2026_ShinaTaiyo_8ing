//==============================================
// 
// ポストエフェクト[色調変化] <ToneAdjust.cpp>
// Auther 木田威吹
// 
//==============================================

//****************************************************
// インクルードファイル
//****************************************************

#include "ToneAdjust.h"   // 元ヘッダーを使う
#include "manager.h"    // マネージャーの使用
#include "renderer.h"  // デバイスの使用
#include "game.h"    // ゲームを使用
#include "GamePlayer.h"   // ゲームプレイヤーを使用
#include "SummonGroupSelector.h"    // サモングループマネージャーの関数を使用
#include "summon_manager_state_start.h"   // 召喚マネージャー状態クラスを使用
#include "PlayerState_Thrusting.h"   // プレイヤーが障害物に当たったかを使用

//====================================
// 静的メンバ宣言
//====================================
bool CToneAdjust::s_bUseDraw = false; // 描画するかどうか


//================================
// コンストラクタ
//================================
CToneAdjust::CToneAdjust()
{
    m_pEffect = nullptr;

    // 色調の色
    m_colorR = 1.0f;
    m_colorG = 1.0f;
    m_colorB = 1.0f;

    m_enableEffect = false;  // エフェクトの表示状態
    m_bActive = true;  // ImGuiの切り替え
    m_bChange = false;
    m_Alpha = 0.0f;  // 透明度
    m_nCountFrame = 0;
    m_fadeProgress = 0.0f;  // フェードの進捗
    m_FadeActive = false;  // フェードの切り替え
    m_FadeDirection = 0.0f;  // フェードの方向

    m_totalTime = 0.0f;  // 時間を入れる
    m_bLevelBarEnabled = false; // 棒の起動
    s_bUseDraw = true; // 描画を使用する
}
//================================
// デストラクタ
//================================
CToneAdjust::~CToneAdjust()
{
}
//================================
// 初期化処理
//================================
HRESULT CToneAdjust::Init()
{
    // デバイスの取得
    auto pDevice = CRenderer::GetDeviece();

    CObject_2D::Init();

    // タイプ設定
    SetType(CObject::TYPE::TONE);

    // シェーダーファイルの読み込み
#ifdef _DEBUG
    InitToneAdjust(pDevice, "Src/Shader/fx/ToneAdjust.fx");
#else
    InitToneAdjust(pDevice, "Src/Shader/fxo/ToneAdjust.fxo");
#endif // _DEBUG


    return S_OK;
}
//================================
// 終了処理
//================================
void CToneAdjust::Uninit()
{
    // エフェクトの破棄
    if (m_pEffect)
    {
        m_pEffect->Release();
        m_pEffect = nullptr;
    }

    CObject_2D::Uninit();
}
//================================
// 更新処理
//================================
void CToneAdjust::Update()
{
    CObject_2D::Update();

    // ImGuiのやつ
    ImGui::SetNextWindowSize({ -1, -1 });
    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

    if (ImGui::Begin(u8"色調変化"))
    {
        ImGui::Checkbox(u8"Guiを開く", &m_bActive);
        if (!m_bActive) {
            ImGui::Text(u8"色調の色調整");
            ImGui::SliderFloat("Color R", &m_colorR, 0.0f, 1.0f);
            ImGui::SliderFloat("Color G", &m_colorG, 0.0f, 1.0f);
            ImGui::SliderFloat("Color B", &m_colorB, 0.0f, 1.0f);

            ImGui::Text(u8"色調の透明度");
            ImGui::SliderFloat(u8"透明度", &m_Alpha, 0.0f, 1.0f);

            ImGui::Checkbox(u8"エフェクトの表示状態", &m_enableEffect);

            ImGui::Text(u8"フェードの設定");

            ImGui::SliderFloat(u8"フェードの進捗", &m_fadeProgress, 0.0f, 1.0f);
            ImGui::Checkbox(u8"フェードの切り替え", &m_FadeActive);

        }
        ImGui::End();
    }
    // フレーム間の時間取得に使う
    DWORD currentTime = timeGetTime(); // ミリ秒
    static DWORD prevTime = currentTime;
    float deltaTime = (currentTime - prevTime) / 1000.0f;
    prevTime = currentTime;

    m_totalTime += deltaTime;

    UpdateToneAdjust(m_totalTime);  // シェーダーの更新
}
//================================
// 描画処理
//================================
void CToneAdjust::Draw()
{
    if (s_bUseDraw)
    {
        // デバイスの取得
        auto pDevice = CRenderer::GetDeviece();

        RenderToneAdjust(pDevice);  // シェーダーの描画
    }
}

//================================
// 生成処理
//================================
CToneAdjust* CToneAdjust::Create()
{
    CToneAdjust* pToneAdjust = DBG_NEW CToneAdjust();
    pToneAdjust->Init();

    return pToneAdjust;
}
//====================================
// 描画するかどうか設定
//====================================
void CToneAdjust::SetUseDraw(bool bUse)
{
    s_bUseDraw = bUse;
}
//================================
// シェーダーの初期化
//================================
bool CToneAdjust::InitToneAdjust(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
    m_fxFilePath = fxFile; // パス保存
    LPD3DXBUFFER pErr = nullptr;

    // エフェクト読み込み
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
    if (FAILED(hr))
    {// シェーダーが読み込まれなかったら
        if (pErr)
        {// エラー表示を出す
            MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Compile Error", MB_OK);
            pErr->Release();
            pErr = nullptr;
        }
        return false;
    }

    return true;
}

//================================
// シェーダーの破棄
//================================
void CToneAdjust::UninitToneAdjust()
{
    // 特になし
}
//=====================================
// シェーダーの更新
//=====================================
void CToneAdjust::UpdateToneAdjust(float deltaTime)
{
    // 色の設定
    D3DXVECTOR4 baseColor = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);
    D3DXVECTOR4 fadeColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

    // 召喚マネージャへのポインタを取得
    CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));

    // サモンマネージャーステートスタートのポインタ情報を取得
    CSummon_Manager_State_Start* pStart = dynamic_cast<CSummon_Manager_State_Start*>(pSummonManager->GetState());

    // プレイヤーへのポインタを取得
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER));
    // プレイヤー突き状態へのポインタを取得
    CPlayerState_Thrusting* pThrusting = dynamic_cast<CPlayerState_Thrusting*>(pPlayer->GetState());

    if (pStart)
    {
        if (pStart->GetStartPhase())
        {// 現在がフェーズ中だったら
            m_enableEffect = true;  // 色調起動

            // 突き状態で当たったかつ、当たったものが障害物ならログを出す
            if (pThrusting)
            {
                if (pThrusting->GetNowCollision() && pThrusting->GetCollisionObstacle())
                {// 障害物に当たった時

                    m_nCountFrame = 30;  // 消すフレームをセット
                    m_bChange = true;   // 色を変えるためのbool
                }
                else
                {
                }
            }

            float gradProgress = 0.0f;

            // 障害物に当たった時(失敗時)
            if (m_bChange) {  // trueの時
                --m_nCountFrame;  // カウントを減らす
                m_FadeActive = true;
                m_bLevelBarEnabled = true;

                // フェードがアクティブ状態だったら
                if (m_FadeActive) {
                    m_FadeDirection = 1.0f;  // フェードイン状態
                    // フェードイン処理
                    if (m_fadeProgress < 1.0f) {
                        m_fadeProgress += deltaTime * FADE_SPEED; // 2.0倍のスピードでイン
                        m_fadeProgress = min(m_fadeProgress, 1.0f);
                    }
                }
                // 色を変える
                m_colorR = 0.3f;
                m_colorG = 0.0f;
                m_colorB = 0.5f;
                m_Alpha = 0.8f;

                // カウントがなくなった時
                if (m_nCountFrame <= 0) {
                    m_bChange = false;  // 状態変化を元に戻す

                    m_nCountFrame = 0;  // カウントも戻す
                }
            }
            else {  // falseの時
                m_FadeActive = false;

                // エフェクトがoffの時
                if (!m_FadeActive) {
                    m_FadeDirection = -1.0f;  // フェードアウト状態
                    // フェードアウト処理
                    if (m_fadeProgress > 0.0f) {
                        m_fadeProgress -= deltaTime * FADE_SPEED_DOWN; // 2.0倍のスピードでアウト

                        m_fadeProgress = max(m_fadeProgress, 0.0f);
                    }
                }
                // 色と透明度を元に戻す
                m_colorR = 1.0f;
                m_colorG = 1.0f;
                m_colorB = 1.0f;
                m_bLevelBarEnabled = false;
            }
        }
        else{
            m_colorR = 1.0f;
            m_colorG = 1.0f;
            m_colorB = 1.0f;

            m_Alpha = 0.0f;
            m_enableEffect = false;  // 色調を止める
        }
    }

    // パラメーターを設定
    m_pEffect->SetVector("g_BaseColor", &baseColor);  // ベースの色
    m_pEffect->SetFloat("g_FadeProgress", m_fadeProgress);   // フェードを起動
    m_pEffect->SetFloat("g_Alpha", m_Alpha); // 半透明
    m_pEffect->SetBool("g_FadeEnabled", m_FadeActive);
    m_pEffect->SetVector("g_FadeStartColor", &fadeColor);
    m_pEffect->SetFloat("g_Time", deltaTime);
    m_pEffect->SetBool("g_LevelBarEnabled", m_bLevelBarEnabled);

}

//================================
// シェーダーの描画
//================================
void CToneAdjust::RenderToneAdjust(LPDIRECT3DDEVICE9 device)
{
    //if (!m_enableEffect)
    //{// エフェクトの表示状態
    //    return;  // falseの時は描画しない
    //}

    // --- ブレンド有効化 ---
    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    // --- 色パラメータ設定 ---
    D3DXVECTOR4 baseColor = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);
    D3DXVECTOR4 fadeColor = D3DXVECTOR4(0.031f, 0.968f, 0.968f, 1.0f);
    D3DXVECTOR4 tint(1.0f, 1.0f, 1.0f, 1.0f);      // 無変更
    m_pEffect->SetVector("g_BaseColor", &baseColor);  // ベースの色
    m_pEffect->SetFloat("g_FadeProgress", m_fadeProgress);   // フェードを起動
    m_pEffect->SetVector("g_Tint", &tint);    // 色味
    m_pEffect->SetFloat("g_Brightness", 0.5f);  // 明るさ
    m_pEffect->SetFloat("g_Contrast", 1.0f);  // コントラクタ
    m_pEffect->SetFloat("g_Alpha", m_Alpha); // 半透明
    m_pEffect->SetBool("g_FadeEnabled", m_FadeActive);
    m_pEffect->SetVector("g_FadeStartColor", &fadeColor); // フェードの色
    m_pEffect->SetBool("g_LevelBarEnabled", m_bLevelBarEnabled);  // 棒の起動

    m_pEffect->SetTechnique("ToneAdjust");  // テクニック定義
    // --- 描画 ---
    UINT numPasses = 0;
    m_pEffect->Begin(&numPasses, 0);
    for (UINT i = 0; i < numPasses; ++i)
    {
        m_pEffect->BeginPass(i);
        CreateFullScreenQuad(device);   // 全画面にポリゴンを描画
        m_pEffect->EndPass();
    }
    m_pEffect->End();

    // ブレンドを元に戻す
    device->SetRenderState(D3DRS_ZENABLE, TRUE);
    device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

//================================
// 全画面ポリゴンの描画
//================================
void CToneAdjust::CreateFullScreenQuad(LPDIRECT3DDEVICE9 device)
{
    // 全画面クアッド（左上から右下）
    SCREEN_VERTEX quad[] = {
        { 0.0f,     0.0f,     0.0f, 1.0f, 0.0f, 0.0f },
        { 1280.0f,  0.0f,     0.0f, 1.0f, 1.0f, 0.0f },
        { 0.0f,     720.0f,   0.0f, 1.0f, 0.0f, 1.0f },
        { 1280.0f,  720.0f,   0.0f, 1.0f, 1.0f, 1.0f },
    };

    device->SetFVF(FVF_SCREEN_VERTEX);
    device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quad, sizeof(SCREEN_VERTEX));

}

