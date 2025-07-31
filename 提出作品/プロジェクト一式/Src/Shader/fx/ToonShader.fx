//=====================================
// 
// �������C�g���g�D�[���V�F�[�_�[
// Auther �ؓc�А�
// 
//=====================================

// -----------------------------
// �O���[�o���ϐ��錾
// -----------------------------
float4x4 World;  // ���[���h�s��
float4x4 WorldViewProj;   // ���[���h�r���[�v���W�F�N�V�����s��
float3 LightDirection;   // ���C�g�̈ʒu
float3x3 gWorldInverseTranspose;    // �@���ϊ��p�̋t�]�u�s��

float4x4 gView;
float4x4 gWorldView;
float4x4 gProj;
float OutlineThickness = 1.05f;



float4 MaterialDiffuse;   // ���f���̃f�B�t���[�Y�F
texture DiffuseTexture;   // ���f���̃e�N�X�`��
bool UseTexture;   // �e�N�X�`�����g�p���Ă��邩

float3 gCameraPos;   // �J�����̈ʒu
float RimPower = 4.0f;   // �������C�g�̋���
float RimIntensity = 0.6f;   // �������C�g�̉s��
float RimThickness = 0.05f;  // �������C�g�̑���

// ----------------------------
// �e�N�X�`���T���v���[�u���b�N
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
// ���_/�s�N�Z���\����
// ----------------------------
struct VS_IN {
    float4 Pos    : POSITION;
    float3 Normal : NORMAL;
    float2 Tex    : TEXCOORD0;
    float4 color : COLOR0; // �f�B�t���[�Y�J���[�i���f��������n���j
};

struct VS_OUT {
    float4 Pos             : POSITION;
    float  DiffuseIntensity : TEXCOORD0;
    float2 Tex             : TEXCOORD1;
};
// �������C�g��p
struct VS_OUT_RIM {
    float4 Pos:        POSITION;
    float3 worldPos:   TEXCOORD0;
    float3 normal:     TEXCOORD1;
    float2 Tex:        TEXCOORD2;
    float4 color:      COLOR0;
};

// �A�E�g���C���`��p�̒��_�V�F�[�_�[�o��
struct VS_OUTPUT_OUTLINE {
    float4 Pos : POSITION;    // �ʒu
    float2 Tex : TEXCOORD0;   // �e�N�X�`�����W
    float3 LightIntensity : COLOR0;      // ���C�e�B���O����
};

// ----------------------------
// ���_�V�F�[�_�[
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
// �g�D�[�����s�N�Z���V�F�[�_�[�i�}�e���A�� + �e�N�X�`���j
// ----------------------------
float4 PS(VS_OUT input) : COLOR{
    float intensity = input.DiffuseIntensity;

    float4 toonColor;

    // �i�K�I�Ƀ��C�e�B���O��ς���
    if (intensity > 0.75) toonColor = float4(1.0, 1.0, 1.0, 1.0);
    else if (intensity > 0.5) toonColor = float4(0.8, 0.8, 0.8, 1.0);
    else if (intensity > 0.25) toonColor = float4(0.4, 0.4, 0.4, 1.0);
    else toonColor = float4(0.1, 0.1, 0.1, 1.0);

    // �e�N�X�`����}�e���A���ɐF��t����
    float4 texColor = tex2D(TexSampler, input.Tex);
    float4 matColor = float4(MaterialDiffuse.rgb, 1.0);

    // �t���O�ɂ���ĕ���
    float4 resultColor = UseTexture
        ? (toonColor * texColor)
        : (toonColor * matColor);

    return resultColor;
}

//----------------------------------
// �������C�g�̒��_�V�F�[�_�[
//----------------------------------
VS_OUT_RIM VS_Rim(VS_IN input) {
    VS_OUT_RIM output;

    float3 worldPos = input.Pos.xyz + input.Normal * RimThickness;  // �O�ɉ����o��
    output.Pos = mul(float4(worldPos, 1.0f), WorldViewProj);
    output.worldPos = worldPos.xyz;
    output.normal = normalize(input.Normal);
    output.color = input.color;

    output.Tex = input.Tex;

    return output;
}

//----------------------------------
// �������C�g�̃s�N�Z���V�F�[�_�[
//----------------------------------
float4 PS_Rim(VS_OUT_RIM input) : COLOR{
    float3 normal = normalize(input.normal);
    float3 viewDir = normalize(gCameraPos - input.worldPos);
    float3 lightDir = normalize(LightDirection);

    // �������C�g�̐F�����������ƃ��C�g�̊p�x�ŕς���
    float VdotL = dot(viewDir, -lightDir);        // �J���������ƌ��̕���

    // �J���������ɑ΂��Ăǂꂭ�炢�t�������`�F�b�N�i-1:�w�ʌ�, 1:�����j
    float backlightFactor = saturate((1.0 - VdotL) * 0.5);

    // �F�̐��`��ԁi�t������ warmColor�A�������� coolColor�j
    float3 warmColor = float3(1.0, 1.0, 1.0);  // �I�����W
    float3 coolColor = float3(0.8, 0.8, 0.8);  // ��
    float3 dynamicRimColor = lerp(coolColor, warmColor, backlightFactor);

    // �������C�g�W���i�֊s�Ɍ������ċ����Ȃ�j
    float rim = 1.0f - saturate(dot(normal, viewDir));
    rim = pow(rim, RimPower) * RimIntensity;

    // �f�B�t���[�Y�F + �A���r�G���g
    float3 ambient = float3(0.1f, 0.1f, 0.1f);

    // �ŏI�����J���[
    float3 rimColor = dynamicRimColor * rim;

    // ���̐F�i�}�e���A�� or �e�N�X�`���j
    float4 baseColor = UseTexture
        ? tex2D(TexSampler, input.Tex)
        : MaterialDiffuse;

    // �����J���[����
    float3 finalColor = baseColor.rgb * ambient + rimColor;

    return float4(saturate(finalColor), baseColor.a);
}

// -----------------------------
// �A�E�g���C���p ���_�V�F�[�_�[
// -----------------------------
VS_OUTPUT_OUTLINE VS_Outline(VS_IN input)
{
    //VS_OUTPUT_OUTLINE output;

    //// ���[���h��Ԃ̖@���x�N�g���ɕϊ�
    //float3 normalWS = normalize(mul(input.Normal, gWorldInverseTranspose)); // ���[���h��Ԃ̖@��

    //// �@�������ɏ��������o���ė֊s�������
    //float3 offset = normalWS * OutlineThickness;

    //// ���̒��_�ʒu�ɃI�t�Z�b�g��������
    //float4 localPos = input.Pos;
    //localPos.xyz += offset; // ���[�J����ԂŃI�t�Z�b�g

    //// ���[�J�����W����ŏI�X�N���[�����W�֕ϊ�
    //output.Pos = mul(localPos, WorldViewProj);

    //output.LightIntensity = float3(0.0, 0.0, 0.0);
    //output.Tex = input.Tex;
    //return output;



    VS_OUTPUT_OUTLINE output;

    // ���[�J����Ԃ̌��_����X�P�[�����O���Ėc��܂���
    float4 scaledLocalPos = input.Pos;
    scaledLocalPos.xyz *= OutlineThickness; // ���_����g��

    // ���[���h �� �r���[ �� �v���W�F�N�V����
    output.Pos = mul(scaledLocalPos, WorldViewProj);

    // ����F�Ȃ̂� LightIntensity �� 0 ��
    output.LightIntensity = float3(0.0, 0.0, 0.0);

    // �e�N�X�`���͎g�킢
    output.Tex = input.Tex;

    return output;
}
// -----------------------------
// �A�E�g���C�� �s�N�Z���V�F�[�_�[
// -----------------------------
float4 PS_Outline(VS_OUTPUT_OUTLINE input) : COLOR{
    return float4(0.0, 0.0, 0.0, 1.0); // ���S�ȍ��ŕ`��i�֊s���p�j
}


// ----------------------------
// �e�N�j�b�N
// ----------------------------
technique ToonShading {
    pass P0 {
        CullMode = CCW; // �\�����i�O�p�`�̒ʏ�ʁj�����`��
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS();
    }
}
technique RimLightOnly {
    pass P0 {
        CullMode = CW;  // ���ʁi�O�p�`�̋t�����j�����`��
        VertexShader = compile vs_2_0 VS_Rim();
        PixelShader = compile ps_2_0 PS_Rim();
    }
}

// �A�E�g���C��
technique Outline {
    pass P0 {
        CullMode = CW;  // ���ʁi�O�p�`�̋t�����j�����`��
        VertexShader = compile vs_3_0 VS_Outline();
        PixelShader = compile ps_3_0 PS_Outline();
    }
}
