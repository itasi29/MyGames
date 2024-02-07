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
	
#if false
	float2 xy = psInput.uv - 0.5;
	float posAngle = atan2(xy.y, xy.x);

	// ���݂̏ꏊ���璆�S�܂ł̋���
	float len = clamp(length(xy), 0.0, 0.5);

	// �����ɂ��鏬���ȉ~�̕���
	float smallCircle = pow(2 * (0.5 - len) * (0.75 + sin(posAngle + 1.57)), 3.0f);



	float h = smallCircle * (cos(abs(len - angle)) / 2.0);

	h = clamp(h + 0.5, 0.0, 0.5);
#else
	float2 xy = psInput.uv - 0.5;
	float posAngle = atan2(xy.y, xy.x);

	float len = length(xy);

	float h = 0.5 + (clamp(sin(len - angle), 0.0, 0.5) * cos((posAngle + 3.14 * 1.1))) * abs(sin(angle / 5.0));

#endif


	// 0.5���^��
	// 0�ɋ߂Â��قǉE����
	// 1�ɋ߂Â��قǍ����
	// �Y����

	// �F�̌v�Z
	psOutput.output.rgb = h;
	// ���l��1�̂܂܂�
	psOutput.output.a = 1;

	return psOutput;
}