// VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// OUT
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// �萔�o�b�t�@ : �X���b�g�l�Ԗ�
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