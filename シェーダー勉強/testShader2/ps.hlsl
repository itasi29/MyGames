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
	float4 color	: SV_TARGET0;
};

SamplerState sam;
Texture2D tex : register(t0);

// �����炭main.cpp�̕���
// float* gAngle = GetShader...
// �̂�œ����H����
// ���C�����[�v����gAngle[0]��angle�̒l������
// UpdateShader...
// �̂�ŏ����X�V���Ă�����ł��g����悤�ɂ��Ă���H
cbuffer Constant : register(b3)
{
	float angle;
}

PsOutput main(PsInput psInput)
{
	PsOutput psOutput;

	const int kPosX = 640;
	const int kPosY = 360;

	// �h�炬�v�Z
	float dist = sqrt(pow(psInput.pos.x - kPosX, 2) + pow(psInput.pos.y - kPosY, 2));
	float sinTheta = (psInput.pos.y - kPosY) / dist;
	float cosTheta = (psInput.pos.x - kPosX) / dist;

	// �䂪�ݍ쐬�p
	float angle2 = psInput.pos.y - kPosY + psInput.pos.y - kPosX + angle;
	angle2 = radians(angle2);

	// �䂪�ݔ��a�v�Z
	float waveRadius = 256 + 256 / 16 * (-1 + sin(angle2));

	// ���S���痣���قǉe���_�E��
	float powerRatio = (waveRadius - dist) / waveRadius;
	if (powerRatio < 0)
	{
		powerRatio = 0;
	}

	// �F�����擾����ʒu
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