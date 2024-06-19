SamplerState smp : register(s0);
Texture2D<float4> tex : register(t0);
Texture2D<float4> dissolve : register(t1);

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 dif : COLOR0;
//  float4 spec : COLOR1;   // ‚¤‚Ü‚­“®‚©‚È‚©‚Á‚½‚ç—LŒø‰»
    float2 uv : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    if (input.uv.y < 0.5)
    {
        discard;
    }
    
    return tex.Sample(smp, input.uv);
}