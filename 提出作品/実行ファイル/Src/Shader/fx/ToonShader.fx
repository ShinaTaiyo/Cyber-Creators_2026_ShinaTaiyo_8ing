//=====================================
// 
// リムライト＆トゥーンシェーダー
// Auther 木田威吹
// 
//=====================================

// -----------------------------
// グローバル変数宣言
// -----------------------------
float4x4 World;  // ワールド行列
float4x4 WorldViewProj;   // ワールドビュープロジェクション行列
float3 LightDirection;   // ライトの位置
float3x3 gWorldInverseTranspose;    // 法線変換用の逆転置行列

float4x4 gView;
float4x4 gWorldView;
float4x4 gProj;
float OutlineThickness = 1.05f;



float4 MaterialDiffuse;   // モデルのディフューズ色
texture DiffuseTexture;   // モデルのテクスチャ
bool UseTexture;   // テクスチャを使用しているか

float3 gCameraPos;   // カメラの位置
float RimPower = 4.0f;   // リムライトの強さ
float RimIntensity = 0.6f;   // リムライトの鋭さ
float RimThickness = 0.05f;  // リムライトの太さ

// ----------------------------
// テクスチャサンプラーブロック
// ----------------------------
sampler2D TexSampler = sampler_state {
    Texture = <DiffuseTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

// ----------------------------
// 頂点/ピクセル構造体
// ----------------------------
struct VS_IN {
    float4 Pos    : POSITION;
    float3 Normal : NORMAL;
    float2 Tex    : TEXCOORD0;
    float4 color : COLOR0; // ディフューズカラー（モデル側から渡す）
};

struct VS_OUT {
    float4 Pos             : POSITION;
    float  DiffuseIntensity : TEXCOORD0;
    float2 Tex             : TEXCOORD1;
};
// リムライト専用
struct VS_OUT_RIM {
    float4 Pos:        POSITION;
    float3 worldPos:   TEXCOORD0;
    float3 normal:     TEXCOORD1;
    float2 Tex:        TEXCOORD2;
    float4 color:      COLOR0;
};

// アウトライン描画用の頂点シェーダー出力
struct VS_OUTPUT_OUTLINE {
    float4 Pos : POSITION;    // 位置
    float2 Tex : TEXCOORD0;   // テクスチャ座標
    float3 LightIntensity : COLOR0;      // ライティング結果
};

// ----------------------------
// 頂点シェーダー
// ----------------------------
VS_OUT VS(VS_IN input) {
    VS_OUT output;
    output.Pos = mul(input.Pos, WorldViewProj);
    float3 normal = normalize(input.Normal);
    float diffuse = saturate(dot(normal, -LightDirection));
    output.DiffuseIntensity = diffuse;
    output.Tex = input.Tex;
    return output;
}

// ----------------------------
// トゥーン風ピクセルシェーダー（マテリアル + テクスチャ）
// ----------------------------
float4 PS(VS_OUT input) : COLOR{
    float intensity = input.DiffuseIntensity;

    float4 toonColor;

    // 段階的にライティングを変える
    if (intensity > 0.75) toonColor = float4(1.0, 1.0, 1.0, 1.0);
    else if (intensity > 0.5) toonColor = float4(0.8, 0.8, 0.8, 1.0);
    else if (intensity > 0.25) toonColor = float4(0.4, 0.4, 0.4, 1.0);
    else toonColor = float4(0.1, 0.1, 0.1, 1.0);

    // テクスチャやマテリアルに色を付ける
    float4 texColor = tex2D(TexSampler, input.Tex);
    float4 matColor = float4(MaterialDiffuse.rgb, 1.0);

    // フラグによって分岐
    float4 resultColor = UseTexture
        ? (toonColor * texColor)
        : (toonColor * matColor);

    return resultColor;
}

//----------------------------------
// リムライトの頂点シェーダー
//----------------------------------
VS_OUT_RIM VS_Rim(VS_IN input) {
    VS_OUT_RIM output;

    float3 worldPos = input.Pos.xyz + input.Normal * RimThickness;  // 外に押し出す
    output.Pos = mul(float4(worldPos, 1.0f), WorldViewProj);
    output.worldPos = worldPos.xyz;
    output.normal = normalize(input.Normal);
    output.color = input.color;

    output.Tex = input.Tex;

    return output;
}

//----------------------------------
// リムライトのピクセルシェーダー
//----------------------------------
float4 PS_Rim(VS_OUT_RIM input) : COLOR{
    float3 normal = normalize(input.normal);
    float3 viewDir = normalize(gCameraPos - input.worldPos);
    float3 lightDir = normalize(LightDirection);

    // リムライトの色を視線方向とライトの角度で変える
    float VdotL = dot(viewDir, -lightDir);        // カメラ視線と光の方向

    // カメラが光に対してどれくらい逆光かをチェック（-1:背面光, 1:順光）
    float backlightFactor = saturate((1.0 - VdotL) * 0.5);

    // 色の線形補間（逆光時は warmColor、順光時は coolColor）
    float3 warmColor = float3(1.0, 1.0, 1.0);  // オレンジ
    float3 coolColor = float3(0.8, 0.8, 0.8);  // 青
    float3 dynamicRimColor = lerp(coolColor, warmColor, backlightFactor);

    // リムライト係数（輪郭に向かって強くなる）
    float rim = 1.0f - saturate(dot(normal, viewDir));
    rim = pow(rim, RimPower) * RimIntensity;

    // ディフューズ色 + アンビエント
    float3 ambient = float3(0.1f, 0.1f, 0.1f);

    // 最終リムカラー
    float3 rimColor = dynamicRimColor * rim;

    // 元の色（マテリアル or テクスチャ）
    float4 baseColor = UseTexture
        ? tex2D(TexSampler, input.Tex)
        : MaterialDiffuse;

    // リムカラー合成
    float3 finalColor = baseColor.rgb * ambient + rimColor;

    return float4(saturate(finalColor), baseColor.a);
}

// -----------------------------
// アウトライン用 頂点シェーダー
// -----------------------------
VS_OUTPUT_OUTLINE VS_Outline(VS_IN input)
{
    //VS_OUTPUT_OUTLINE output;

    //// ワールド空間の法線ベクトルに変換
    //float3 normalWS = normalize(mul(input.Normal, gWorldInverseTranspose)); // ワールド空間の法線

    //// 法線方向に少し押し出して輪郭線を作る
    //float3 offset = normalWS * OutlineThickness;

    //// 元の頂点位置にオフセットを加える
    //float4 localPos = input.Pos;
    //localPos.xyz += offset; // ローカル空間でオフセット

    //// ローカル座標から最終スクリーン座標へ変換
    //output.Pos = mul(localPos, WorldViewProj);

    //output.LightIntensity = float3(0.0, 0.0, 0.0);
    //output.Tex = input.Tex;
    //return output;



    VS_OUTPUT_OUTLINE output;

    // ローカル空間の原点からスケーリングして膨らませる
    float4 scaledLocalPos = input.Pos;
    scaledLocalPos.xyz *= OutlineThickness; // 原点から拡大

    // ワールド → ビュー → プロジェクション
    output.Pos = mul(scaledLocalPos, WorldViewProj);

    // 黒一色なので LightIntensity は 0 に
    output.LightIntensity = float3(0.0, 0.0, 0.0);

    // テクスチャは使わい
    output.Tex = input.Tex;

    return output;
}
// -----------------------------
// アウトライン ピクセルシェーダー
// -----------------------------
float4 PS_Outline(VS_OUTPUT_OUTLINE input) : COLOR{
    return float4(0.0, 0.0, 0.0, 1.0); // 完全な黒で描画（輪郭線用）
}


// ----------------------------
// テクニック
// ----------------------------
technique ToonShading {
    pass P0 {
        CullMode = CCW; // 表向き（三角形の通常面）だけ描画
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS();
    }
}
technique RimLightOnly {
    pass P0 {
        CullMode = CW;  // 裏面（三角形の逆向き）だけ描画
        VertexShader = compile vs_2_0 VS_Rim();
        PixelShader = compile ps_2_0 PS_Rim();
    }
}

// アウトライン
technique Outline {
    pass P0 {
        CullMode = CW;  // 裏面（三角形の逆向き）だけ描画
        VertexShader = compile vs_3_0 VS_Outline();
        PixelShader = compile ps_3_0 PS_Outline();
    }
}
