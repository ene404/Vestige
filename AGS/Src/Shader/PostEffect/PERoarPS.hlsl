//#include "../Common/Pixel/PixelShader2DHeader.hlsli"

#define PI 3.14159265358979323846   // PI�̒�`

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
    float g_flash_intensity;    //�t���b�V�����x(0.0�`1.0)
    float3 padding1;             //�p�f�B���O1
    float4 g_flash_color;       //�t���b�V���̐F
    float g_distortion_intensity;       //�c�݋��x(0.0�`1.0)
    float2 g_roar_center_screen_pos;    //  ���K���S�_(�X�N���[�����)  
    float g_chromatic_aberration_intensity;     //�F�������x
    float g_bloom_intensity;    //�u���[�����x
    float g_bloom_threshold;    //�u���[����臒l
    float g_grainIntensity;     //�O���C���̋��x
    float g_grain_time;         //���Ԍo�߂Ńm�C�Y�p�^�[����ω�������ׂ̎��Ԓl
    float g_glare_Intensity;    //�O���A���x
    float2 g_glare_origin_screen_pos;    //�O���A�̔����n�_
}



float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // ���̉�ʂ̐F���擾
    float4 color = tex.Sample(texSampler,PSInput.uv);

    // ��ʂ̘c�� (Screen Distortion)
    float2 dist = PSInput.uv - g_roar_center_screen_pos;
    float len = length(dist);
    float distortionFactor = pow(len, 2.0) * g_distortion_intensity;

    float2 distortionOffset = normalize(dist) * distortionFactor * 0.05;

    float2 distortedUV = PSInput.uv + distortionOffset;
    color = tex.Sample(texSampler, distortedUV);

    // �F�̎���
    float2 ab_offset = distortionOffset * g_chromatic_aberration_intensity * 0.5;
    float4 red_color = tex.Sample(texSampler, PSInput.uv + ab_offset);
    float4 green_color = tex.Sample(texSampler, PSInput.uv);
    float4 blue_color = tex.Sample(texSampler, PSInput.uv - ab_offset);

    color.r = red_color.r;
    color.g = green_color.g;
    color.b = blue_color.b;

    // ��ʃt���b�V��
    color = lerp(color, g_flash_color, g_flash_intensity);

    // �ȈՃu���[��
    float brightness = dot(color.rgb, float3(0.299, 0.587, 0.114));
    if (brightness > g_bloom_threshold)
    {
        float bloomFactor = (brightness - g_bloom_threshold) * g_bloom_intensity;
        color.rgb += color.rgb * bloomFactor;
    }

    // �t�B�����O���C�� / �m�C�Y
    float3 grain = float3(
        frac(sin(dot(PSInput.uv + g_grain_time, float2(12.9898, 78233))) * 437558.5453),
        frac(sin(dot(PSInput.uv + g_grain_time + 0.1, float2(12.9898, 78.233))) * 43758.5453),
        frac(sin(dot(PSInput.uv + g_grain_time + 0.2, float2(12.9898, 78.233))) * 43758.5453)
        );

    // �����_���ȃm�C�Y�l��-0.5����0.5�͈̔͂ɒ���
    grain = grain * 2.0 - 1.0;

    // ���̐F�Ƀm�C�Y�����Z
    color.rgb += grain * g_grainIntensity;

    // �ȈՃO���A(������X�g���[�N)
    // �������猻�݂̃s�N�Z���܂ł̃x�N�g��
    float2 glareVec = PSInput.uv - g_glare_origin_screen_pos;
    // �x�N�g���̒����ɉ��������x
    float glareLen = length(glareVec);
    // ������̃O���f�[�V����
    float glareFactor = 1.0 - saturate(glareLen * 2.0); // ���S�ŋ����A�O���Ŏキ(2.0�͍L����)

    // ����̊p�x�Ɍ��̋؂��o��(�����ł�4�����j
    // atan2(y,x)�Ŋp�x�����߁A����̊p�x�͈͂�����������
    float angle = atan2(glareVec.y, glareVec.x);
    float streakFactor = 0.0;
    // �O�x�A90�x�A180�x�A270�x�t�߂ɋ���
    streakFactor += saturate(1.0 - abs(fmod(angle, PI / 2.0) / (PI / 4.0)));// �e�ی��̒��S�t�߂�����
    streakFactor *= glareFactor;    // ���S����̋������l��

    color.rgb += g_glare_Intensity * streakFactor * color.rgb;  // ���邢����������

    // �ŏI�I�ȏo��
    return color;
}
