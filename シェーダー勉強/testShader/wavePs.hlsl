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
	
#if false
	float2 xy = psInput.uv - 0.5;
	float posAngle = atan2(xy.y, xy.x);

	// 現在の場所から中心までの距離
	float len = clamp(length(xy), 0.0, 0.5);

	// 内側にある小さな円の部分
	float smallCircle = pow(2 * (0.5 - len) * (0.75 + sin(posAngle + 1.57)), 3.0f);



	float h = smallCircle * (cos(abs(len - angle)) / 2.0);

	h = clamp(h + 0.5, 0.0, 0.5);
#else
	float2 xy = psInput.uv - 0.5;
	float posAngle = atan2(xy.y, xy.x);

	float len = length(xy);

	float h = 0.5 + (clamp(sin(len - angle), 0.0, 0.5) * cos((posAngle + 3.14 * 1.1))) * abs(sin(angle / 5.0));

#endif


	// 0.5が真ん中
	// 0に近づくほど右下に
	// 1に近づくほど左上に
	// ズレる

	// 色の計算
	psOutput.output.rgb = h;
	// α値は1のままで
	psOutput.output.a = 1;

	return psOutput;
}