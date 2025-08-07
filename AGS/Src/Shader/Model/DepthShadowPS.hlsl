// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// OUT
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ : スロット四番目
cbuffer cbParam : register(b4)
{
	float g_time;
	float g_scroll_speed;
	float2 dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	float4 color;

	color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);

	return color;
}