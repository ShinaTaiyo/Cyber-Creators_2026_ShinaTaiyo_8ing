//=====================================
// 
// 「気」の蓄積
// Auther 木田威吹
// 
//=====================================

// -----------------------------
// グローバル変数宣言
// -----------------------------
float time;                  // アニメーション用の時間
texture screenTex;           // シーン全体のテクスチャ
texture fireTex;             // 炎のテクスチャ
float4 flameColor = float4(1.0, 0.5, 0.2, 1.0);   // ノイズの初期色
float circlePowor = 0.4;     // 円の強さの初期値
float sharpnessPowor = 0.1;  // 尖りの強さの初期値
float size = 2.0;     // 円のサイズ
float totalDeformPower; // ノイズ変形の全体倍率

// -----------------------------
// テクスチャサンプラブロック
// -----------------------------
sampler screenSamp = sampler_state {
    Texture = <screenTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler fireSamp = sampler_state {
    Texture = <fireTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

// -----------------------------
// ピクセルシェーダー
// -----------------------------
float4 PS_Energy(float2 uv : TEXCOORD0) : COLOR{

    //=====================================================================
    //// ラウンドスクエア(丸みを帯びた四角)

    float2 centeredUV = uv - 0.5;  // UV座標を中心（0,0）に移動
    
    float2 r = abs(centeredUV);
    float corner = 0.2;

    // 辺の補正スケーリング（辺を少し伸ばす）
    float2 stretch = float2(1.12, 1.12); // XとY方向にスケール（1.0〜1.3くらいで調整）

    // スケーリングを加味した q を使う
    float2 q = r * stretch - 0.5 + corner;

    // ラウンドスクエア距離（変形後）
    float roundedSquareDist = length(max(q, 0.0)) + min(max(q.x, q.y), 0.0);

    // サイズスケール
    float dist = roundedSquareDist * size;
    
    // ノイズによる変形
    float angle = atan2(centeredUV.y, centeredUV.x);  // 方向角
    float angleNorm = (angle + 3.14159) / 6.28318;  // [0,1]に正規化
    
    // ---- 丸み：やや滑らかなノイズ ----
    float smoothNoise = tex2D(fireSamp, float2(angleNorm * 2.0 + time * 0.1, time * 0.2)).r;
    smoothNoise = (smoothNoise - 0.5) * circlePowor;  // ノイズ値を-0.5〜+0.5にして強さを調整
    
    // ---- 尖り：高周波ノイズ ----
    float sharpNoise = tex2D(fireSamp, float2(angleNorm * 10.0 + time * 0.2, time * 0.4)).r;
    sharpNoise = (sharpNoise - 0.5) * sharpnessPowor;

    // ---- 合成：丸み + 尖り * 全体の強さ----
    float deform = (smoothNoise + sharpNoise) * totalDeformPower;

    // 揺らぎ全体の強度を一括で制御
    deform *= totalDeformPower;

    // 追時間で周期的に揺らぎを変化
    float timeOscillation = sin(time * 2.0) * 0.5 + 0.5;
    deform *= lerp(0.8, 1.5, timeOscillation);  // 時間に応じて強さを変動 lerp(最小倍率,最大倍率,時間)

    //  UV中心からの距離で揺らぎ強調
    float radius = length(centeredUV);
    deform *= lerp(0.5, 1.5, radius);

    dist += deform;

    // エッジフェード
    float edgeFade = smoothstep(0.6, 1.0, dist);  // フェード範囲を調整

    // 合成
    float4 sceneColor = tex2D(screenSamp, uv);  // 元のシーンカラー

    // アニメーション付きの炎テクスチャ
    float4 fireColor = tex2D(fireSamp, uv * 3.0 + float2(time * 0.2, time * 0.1));
    fireColor.rgb *= flameColor.rgb;  // C++側から受け取る色で着色
    float fireAlpha = fireColor.a * edgeFade;  // アルファ合成

    float4 result = lerp(sceneColor, fireColor, fireAlpha);  // アルファブレンド
    result.a = fireAlpha; // αを維持
    return result;

}
// -----------------------------
// テクニック定義
// -----------------------------
technique TechMain {
    pass P0 {
        PixelShader = compile ps_2_0 PS_Energy();
    }
}
