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

cbuffer cbParam : register(b4)
{
    float4 g_color;
    float hpRate;       // 0.0�i�m���j�`1.0�i���^���j
    float3 pad;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;

    // === 1. �I���W�i���J���[�擾 ===
    float4 srcCol = tex.Sample(texSampler, uv);

    // === 2. HP�ɉ������u���[���ʁi��ɂ��j ===
    float blurAmount = saturate((0.5 - hpRate) / 0.5); // HP50%��������ő�Ɂi0�`1�j
    float2 texelSize = float2(1.0 / 512.0, 1.0 / 512.0);

    float4 blurColor = float4(0, 0, 0, 0);
    int blurSamples = 0;
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float2 offset = float2(x, y) * texelSize * 2.0;
            blurColor += tex.Sample(texSampler, uv + offset);
            blurSamples++;
        }
    }
    blurColor /= blurSamples;

    // �u���[�ƃI���W�i�����ԁi��ɂڂ��������j
    float4 baseColor = lerp(srcCol, blurColor, blurAmount);

    // === 3. HP�����ɂ��ԃG�t�F�N�g�i���Ƃ���̂���j ===
    float2 center = float2(0.5, 0.5);
    float dist = distance(uv, center);
    float effectStrength = 0.0;

    if (hpRate < 0.8)
    {
        float lowHpRatio = saturate((0.8 - hpRate) / 0.8);
        lowHpRatio = pow(lowHpRatio, 4.0);
        float edgeFade = smoothstep(0.25, 0.75, dist);
        effectStrength = edgeFade * lowHpRatio;
    }

    float4 bloodColor = float4(0.8, 0.0, 0.0, 1.0);
    float4 finalColor = lerp(baseColor, bloodColor, effectStrength);

    return finalColor;

}
