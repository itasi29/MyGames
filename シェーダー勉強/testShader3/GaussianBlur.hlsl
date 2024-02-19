Texture2D<float4> TextureInput : register(t0);
SamplerState sam

float2 texelSize;
float weights[9];

float4 main(float2 texCoord : TEXCOORD) : SV_TARGET
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float h;

	for (int i = 0; i < 9; i++)
	{
		h += TextureInput.Sample(sam, );
	}
}