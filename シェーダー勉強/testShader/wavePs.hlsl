/*�������͔̂g��̃V�F�[�_�[��`������*/



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
	
	// ���݂̏ꏊ���璆�S�܂ł̋���
	float len = length(abs(psInput.uv - 0.5));

	float h;

	h = sin(abs(len) - angle);

	// �F��0x000000 = ��, 0xffffff = ���@�Ɠ���

	// �F�̌v�Z
	psOutput.output.rgb = h;
	// ���l��1�̂܂܂�
	psOutput.output.a = 1;

	return psOutput;
}