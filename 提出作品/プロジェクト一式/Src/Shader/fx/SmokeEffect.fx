//==============================================
// 
// �|�X�g�G�t�F�N�g[��]�@SmokeEffect.fx
// Auther �ؓc�А�
// 
//==============================================

//-------------------------------------------
// �p�����[�^��`
//-------------------------------------------
float g_Time;                  // �A�j���[�V�����p����
float g_Scale = 1.5f;          // �m�C�Y�X�P�[��
float g_Speed = 0.1f;          // ����鑬�x
float g_Opacity = 0.4f;        // ���̔Z��
float3 g_SmokeColor = float3(0.8, 0.8, 0.8);

texture g_NoiseTex;  // �e�N�X�`��

//-------------------------------------------
// �T���v���ݒ�
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
// ���_�V�F�[�_�p�\����
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
// ���_�V�F�[�_�[
//-------------------------------------------
VS_OUT VS_Smoke(VS_IN input) {
    VS_OUT o;
    o.pos = input.pos;
    o.uv = input.uv;
    return o;
}

//-------------------------------------------
// �s�N�Z���V�F�[�_�[
//-------------------------------------------
float4 PS_Smoke(VS_OUT input) : COLOR{
    float2 uv = input.uv;
    
    // �A�j���[�V�����p�I�t�Z�b�g
    float2 flow = float2(g_Time * g_Speed, g_Time * g_Speed * 0.5);
    float2 animatedUV = uv * g_Scale + flow;
    
    // �m�C�Y�e�N�X�`�����T���v�����O
    float noise = tex2D(NoiseSampler, animatedUV).r;
    
    // ���͓����A���͕s����
    float alpha = pow(noise, 2.0) * g_Opacity; 

    // RGB�͉��F�AAlpha�͋P�x�ɔ��
    return float4(g_SmokeColor, alpha);
}

//-------------------------------------------
// �e�N�j�b�N
//-------------------------------------------
technique SmokeEffect {
    pass P0 {
        VertexShader = compile vs_2_0 VS_Smoke();
        PixelShader = compile ps_2_0 PS_Smoke();
    }
}
