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
	float4 color	: SV_TARGET0;
};

SamplerState sam;
Texture2D tex : register(t0);

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

	const int kPosX = 640;
	const int kPosY = 360;

	// 揺らぎ計算
	float dist = sqrt(pow(psInput.pos.x - kPosX, 2) + pow(psInput.pos.y - kPosY, 2));
	float sinTheta = (psInput.pos.y - kPosY) / dist;
	float cosTheta = (psInput.pos.x - kPosX) / dist;

	// ゆがみ作成用
	float angle2 = psInput.pos.y - kPosY + psInput.pos.y - kPosX + angle;
	angle2 = radians(angle2);

	// ゆがみ半径計算
	float waveRadius = 256 + 256 / 16 * (-1 + sin(angle2));

	// 中心から離れるほど影響ダウン
	float powerRatio = (waveRadius - dist) / waveRadius;
	if (powerRatio < 0)
	{
		powerRatio = 0;
	}

	// 色情報を取得する位置
	float biasRadius = waveRadius * powerRatio;
	float biasX = biasRadius * cosTheta;
	float biasY = biasRadius * sinTheta;

	float2 texUV;
	texUV.x = -biasX / 1280 + psInput.uv.x;
	texUV.y = -biasY / 720 + psInput.uv.y;

	float4 color = tex.Sample(tex, texUV) * psInput.dif;

	psOutput.color = color;

	return psOutput;
}