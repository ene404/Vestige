// VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// OUT
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

Texture2D dissolveTex : register(t1);	// �f�B�]���u�}�X�N

// �萔�o�b�t�@ : �X���b�g�l�Ԗ�
cbuffer cbParam : register(b4)
{
	float g_time;
	float g_scroll_speed;
	float2 dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	// ������ɂփX�N���[��
	PSInput.uv.y -= g_time * g_scroll_speed;

	// �J��Ԃ�
	PSInput.uv.y = frac(PSInput.uv.y);

	float4 color;

	color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);

	return color;
}