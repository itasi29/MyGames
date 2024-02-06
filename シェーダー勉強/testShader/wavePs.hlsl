/*こいつ自体は波紋のシェーダーを描くだけ*/



// ピクセルシェーダーの入力
struct PsInput
{
	float4 pos	: SV_POSITION;
	float4 dif	: COLOR0;
	float2 uv	: TEXCOORD0;
};

// 出力
struct PsOutput
{
	float4 output	: SV_TARGET0;
};

// おそらくmain.cppの方で
// float* gAngle = GetShader...
// のやつで同期？して
// メインループ内でgAngle[0]にangleの値を入れて
// UpdateShader...
// のやつで情報を更新してこちらでも使えるようにしている？
cbuffer Constant : register(b3)
{
	float angle;
}

PsOutput main(PsInput psInput)
{
	PsOutput psOutput;
	
	// 現在の場所から中心までの距離
	float len = length(abs(psInput.uv - 0.5));

	float h;

	h = sin(abs(len) - angle);

	// 色は0x000000 = 黒, 0xffffff = 白　と同じ

	// 色の計算
	psOutput.output.rgb = h;
	// α値は1のままで
	psOutput.output.a = 1;

	return psOutput;
}