Texture2D	 textureInput	: register(t0);
SamplerState samplerLinear	: register(s0);

cbuffer Constant : register (b3)
{
	float frame;
}

float3 RGBtoHSV(float3 rgb);
float3 HSVtoRGB(float3 hsv);

float4 main(float2 texCoord : TEXCOORDO) : SV_TARGET
{
	float4 sum = float4(0.0f, 0.0f, 0.0f, 0.0f);
}