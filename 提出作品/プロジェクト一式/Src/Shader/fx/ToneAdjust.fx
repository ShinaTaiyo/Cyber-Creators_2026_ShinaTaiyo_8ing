//=====================================
// 
// 色調変化 シェーダー
// Auther 木田威吹
// 
//=====================================


// -----------------------------
// 色調調整用シェーダー（背景色向け）
// -----------------------------

// 背景のベース色（元の色）
float3 g_BaseColor = float3(1.0f, 1.0f, 1.0f); // 初期は白

// 色調補正用のパラメータ
float3 g_Tint = float3(1.0f, 1.0f, 1.0f);      // RGBごとの乗算係数（色味）
float g_Brightness = 0.0f;                    // 明るさ補正（-1.0 ～ 1.0）
float g_Contrast = 1.0f;                      // コントラスト（1.0が標準）
float g_Alpha = 1.0f;   // 透明度
float g_FadeDirection = 1.0f;  // 方向

//bool g_GradientEnabled = false;   // グラデーションの切り替え
//float g_GradientStrength = 0.5f;   // グラデーションの強さ
//float g_GradientProgress = 0.0f;  // 0.0〜1.0 に時間で増やす（外から渡す）

float g_FadeProgress = 0.0f;  // フェードの進捗具合
bool g_FadeEnabled = false;
float3 g_FadeStartColor = float3(0, 0, 0);

// 棒関係の変数
bool g_LevelBarEnabled = false;
float g_Time;
// -----------------------------
// 頂点入出力構造体
// -----------------------------
struct VS_OUTPUT {
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

// -----------------------------
// 頂点シェーダ
// -----------------------------
VS_OUTPUT VS_Tone(float4 Pos : POSITION, float2 Tex : TEXCOORD0) {
    VS_OUTPUT output;
    output.Pos = Pos;
    output.Tex = Tex;
    return output;
}

// -----------------------------
// ピクセルシェーダ
// -----------------------------
float4 PS_Tone(VS_OUTPUT input) : COLOR{
    float2 uv = input.Tex;

    // 色調補正
    float3 correctedColor = g_BaseColor * g_Tint;
    correctedColor += g_Brightness;
    correctedColor = ((correctedColor - 0.5f) * g_Contrast) + 0.5f;
    correctedColor = saturate(correctedColor);

    //// グラデーション（上から黒くなる）
    //if (g_GradientEnabled) {
    //    float fade = saturate(1.0 - uv.y);
    //    float limit = saturate(g_GradientProgress);
    //    float edgeSize = 0.3;
    //    float appear = smoothstep(limit + edgeSize, limit, uv.y);
    //    float factor = fade * g_GradientStrength * appear;
    //    correctedColor = lerp(correctedColor, float3(0, 0, 0), factor);
    //}

    // フェード処理
    float3 finalColor = correctedColor;
    float alpha = g_Alpha;
    float fadeAmount = 1.0f;

    if (g_FadeEnabled) {  // フェードを切り替えたら
        //fadeAmount = saturate(g_FadeProgress);
        //finalColor = lerp(g_FadeStartColor, correctedColor, fadeAmount);
        //alpha *= fadeAmount;
        float fadeAmount = saturate(g_FadeProgress);
            // フェードイン: 開始色→最終色
            finalColor = lerp(g_FadeStartColor, correctedColor, fadeAmount);
            alpha *= fadeAmount;
    }
    else if (!g_FadeEnabled) {
        float fadeAmount = saturate(g_FadeProgress);

        // フェードアウト: 最終色→開始色
        finalColor = lerp(correctedColor, g_FadeStartColor, fadeAmount);
        alpha *= (1.0f - fadeAmount);
    }

    // 縦線
    if (g_LevelBarEnabled && g_FadeEnabled)
    {
        // UV座標のYを動かす
        float barCount = 32.0;
        float time = g_Time * 0.05; // 速度
        float uvY = input.Tex.y;
        float barIndex = floor(uv.x * barCount);  // 棒の位置

        // 上下に動かす処理(棒を分ける処理)
        float offset = frac(sin(barIndex * 12.9898) * 43758.5453); // 固定乱数
        float barHeight = sin(time * 6.0 + offset * 6.28); // 時間は同じ、位相だけずらす

        barHeight = saturate(barHeight); // 0〜1に丸める
        barHeight = pow(barHeight, 2.0); // 細く鋭く
        barHeight *= 0.5;   // 1.0が元の長さ
        // uv.y を基準にバーの高さと比較して描画
        float level = saturate(1.0 - uv.y * 1.2); // 高さをスケーリング
        //float bar = step(level, barHeight);

        // 上からバーが降ってくる
        if (uvY > barHeight) {
        }
        else {
            // 色とアルファを上書き
            float3 barColor = float3(0.7, 0.3, 0.8)/* * bar*/; // 色指定
            finalColor = lerp(finalColor, barColor, 0.6f * fadeAmount);
        }
    }

    return float4(finalColor, alpha);

}

// -----------------------------
// テクニック定義
// -----------------------------
technique ToneAdjust {
    pass P0 {
        VertexShader = compile vs_2_0 VS_Tone();
        PixelShader = compile ps_2_0 PS_Tone();
    }
}
