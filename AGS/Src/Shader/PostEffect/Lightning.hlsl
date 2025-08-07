struct PS_INPUT
{
    float4 svPos    : SV_POSITION;
    float4 diffuse  : COLOR0;
    float4 specular : COLOR1;
    float2 uv       : TEXCOORD0;
    float2 suv      : TEXCOORD1;
};

Texture2D tex : register(t0);
SamplerState texSampler : register(s0);

// �����o�p �萔�o�b�t�@�ib4�Ƀo�C���h�j
cbuffer cbLightning : register(b4)
{
    float lightningTime;     // �o�ߎ��ԁi�b�j�Ȃǁi�f�B�X�g�[�V�����p�j
    float lightningStrength; // ���̋����i0.0�`1.0�j
    float2 pad;
}

// �^���m�C�Y�����֐�
float rand(float2 co)
{
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;

    float power = saturate(lightningStrength);
    float time = lightningTime;

    // ==== �f�B�X�g�[�V�����i�����j ====
    float distortion = 0.5 * power; // ����
    float noise = rand(uv * 20.0 + time);
    uv += (noise - 0.5) * distortion;

    // ==== RGB����i�����j ====
    float2 offset = float2(0.9, 0.0) * power; // ����

    float r = tex.Sample(texSampler, uv + offset).r;
    float g = tex.Sample(texSampler, uv).g;
    float b = tex.Sample(texSampler, uv - offset).b;

    float4 col = float4(r, g, b, 1.0);

    // ==== �t���b�V�� ====
    float flash = power * 0.9;
    col.rgb = lerp(col.rgb, float3(1.0, 1.0, 1.0), flash);

    return col;
}