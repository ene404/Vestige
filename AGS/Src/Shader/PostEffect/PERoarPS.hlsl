//#include "../Common/Pixel/PixelShader2DHeader.hlsli"

#define PI 3.14159265358979323846   // PIの定義

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
    float g_flash_intensity;    //フラッシュ強度(0.0～1.0)
    float3 padding1;             //パディング1
    float4 g_flash_color;       //フラッシュの色
    float g_distortion_intensity;       //歪み強度(0.0～1.0)
    float2 g_roar_center_screen_pos;    //  咆哮中心点(スクリーン空間)  
    float g_chromatic_aberration_intensity;     //色収差強度
    float g_bloom_intensity;    //ブルーム強度
    float g_bloom_threshold;    //ブルームの閾値
    float g_grainIntensity;     //グレインの強度
    float g_grain_time;         //時間経過でノイズパターンを変化させる為の時間値
    float g_glare_Intensity;    //グレア強度
    float2 g_glare_origin_screen_pos;    //グレアの発生地点
}



float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 元の画面の色を取得
    float4 color = tex.Sample(texSampler,PSInput.uv);

    // 画面の歪み (Screen Distortion)
    float2 dist = PSInput.uv - g_roar_center_screen_pos;
    float len = length(dist);
    float distortionFactor = pow(len, 2.0) * g_distortion_intensity;

    float2 distortionOffset = normalize(dist) * distortionFactor * 0.05;

    float2 distortedUV = PSInput.uv + distortionOffset;
    color = tex.Sample(texSampler, distortedUV);

    // 色の収差
    float2 ab_offset = distortionOffset * g_chromatic_aberration_intensity * 0.5;
    float4 red_color = tex.Sample(texSampler, PSInput.uv + ab_offset);
    float4 green_color = tex.Sample(texSampler, PSInput.uv);
    float4 blue_color = tex.Sample(texSampler, PSInput.uv - ab_offset);

    color.r = red_color.r;
    color.g = green_color.g;
    color.b = blue_color.b;

    // 画面フラッシュ
    color = lerp(color, g_flash_color, g_flash_intensity);

    // 簡易ブルーム
    float brightness = dot(color.rgb, float3(0.299, 0.587, 0.114));
    if (brightness > g_bloom_threshold)
    {
        float bloomFactor = (brightness - g_bloom_threshold) * g_bloom_intensity;
        color.rgb += color.rgb * bloomFactor;
    }

    // フィルムグレイン / ノイズ
    float3 grain = float3(
        frac(sin(dot(PSInput.uv + g_grain_time, float2(12.9898, 78233))) * 437558.5453),
        frac(sin(dot(PSInput.uv + g_grain_time + 0.1, float2(12.9898, 78.233))) * 43758.5453),
        frac(sin(dot(PSInput.uv + g_grain_time + 0.2, float2(12.9898, 78.233))) * 43758.5453)
        );

    // ランダムなノイズ値を-0.5から0.5の範囲に調整
    grain = grain * 2.0 - 1.0;

    // 元の色にノイズを加算
    color.rgb += grain * g_grainIntensity;

    // 簡易グレア(放物状ストローク)
    // 光源から現在のピクセルまでのベクトル
    float2 glareVec = PSInput.uv - g_glare_origin_screen_pos;
    // ベクトルの長さに応じた強度
    float glareLen = length(glareVec);
    // 放物状のグラデーション
    float glareFactor = 1.0 - saturate(glareLen * 2.0); // 中心で強く、外側で弱く(2.0は広がり具合)

    // 特定の角度に光の筋を出す(ここでは4方向）
    // atan2(y,x)で角度を求め、特定の角度範囲だけ強調する
    float angle = atan2(glareVec.y, glareVec.x);
    float streakFactor = 0.0;
    // ０度、90度、180度、270度付近に強調
    streakFactor += saturate(1.0 - abs(fmod(angle, PI / 2.0) / (PI / 4.0)));// 各象限の中心付近を強調
    streakFactor *= glareFactor;    // 中心からの距離も考慮

    color.rgb += g_glare_Intensity * streakFactor * color.rgb;  // 明るい部分を強調

    // 最終的な出力
    return color;
}
