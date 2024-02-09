// �s�N�Z���V�F�[�_�[�̓���
struct PsInput
{
	float4 pos	: SV_POSITION;
	float4 dif	: COLOR0;
	float2 uv	: TEXCOORD0;
};

// �o��
struct PsOutput
{
	float4 output	: SV_TARGET0;
};

cbuffer Constant : register(b3)
{
	float angle;
}

PsOutput main(PsInput psInput)
{
	PsOutput psOutput;

	float2 xy = psInput.uv - 0.5;
	float posAngle = atan2(xy.y, xy.x);

	float len = length(xy);

	float h = 0.5 + (clamp(sin(len - angle), 0.0, 0.5) * cos(posAngle + 3.14)) * abs(sin(angle / 5.0));

	// �F�̌v�Z
	psOutput.output.rgb = h;
	// ���l��1�̂܂܂�
	psOutput.output.a = 1;

	return psOutput;
}