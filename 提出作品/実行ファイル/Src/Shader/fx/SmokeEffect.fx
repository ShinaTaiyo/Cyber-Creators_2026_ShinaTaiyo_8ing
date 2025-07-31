//==============================================
// 
// ポストエフェクト[煙]　SmokeEffect.fx
// Auther 木田威吹
// 
//==============================================

//-------------------------------------------
// パラメータ定義
//-------------------------------------------
float g_Time;                  // アニメーション用時間
float g_Scale = 1.5f;          // ノイズスケール
float g_Speed = 0.1f;          // 流れる速度
float g_Opacity = 0.4f;        // 煙の濃さ
float3 g_SmokeColor = float3(0.8, 0.8, 0.8);

texture g_NoiseTex;  // テクスチャ

//-------------------------------------------
// サンプラ設定
//-------------------------------------------
sampler NoiseSampler = sampler_state {
    Texture = <g_NoiseTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

//-------------------------------------------
// 頂点シェーダ用構造体
//-------------------------------------------
struct VS_IN {
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

struct VS_OUT {
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

//-------------------------------------------
// 頂点シェーダー
//-------------------------------------------
VS_OUT VS_Smoke(VS_IN input) {
    VS_OUT o;
    o.pos = input.pos;
    o.uv = input.uv;
    return o;
}

//-------------------------------------------
// ピクセルシェーダー
//-------------------------------------------
float4 PS_Smoke(VS_OUT input) : COLOR{
    float2 uv = input.uv;
    
    // アニメーション用オフセット
    float2 flow = float2(g_Time * g_Speed, g_Time * g_Speed * 0.5);
    float2 animatedUV = uv * g_Scale + flow;
    
    // ノイズテクスチャをサンプリング
    float noise = tex2D(NoiseSampler, animatedUV).r;
    
    // 黒は透明、白は不透明
    float alpha = pow(noise, 2.0) * g_Opacity; 

    // RGBは煙色、Alphaは輝度に比例
    return float4(g_SmokeColor, alpha);
}

//-------------------------------------------
// テクニック
//-------------------------------------------
technique SmokeEffect {
    pass P0 {
        VertexShader = compile vs_2_0 VS_Smoke();
        PixelShader = compile ps_2_0 PS_Smoke();
    }
}
