/*�g���`�����V�F�[�_�[�̉�ʉ摜�Ɖ摜�����킹��V�F�[�_�[(�̔F���ł����Ă���͂�)*/

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

// �T���v���Ƃ����炵���������͕������
// ��
// �V�F�[�_�[���f���S�̕ϐ��ɒǉ����ĕ����̊֐����g�p�\�ɂ��Ă���H
SamplerState sam;
// �e�N�X�`��
Texture2D tex : register(t0);
Texture2D nor : register(t1);

cbuffer ConstantBuffer : register(b0)
{
	float threthold;
	float angle;
}

PsOutput main(PsInput input)
{
	PsOutput output;

	// �T���v�����O
	float2 n = nor.Sample(sam, input.uv).xy;
	// -1~1�͈̔͂�
	n = n * 2 - 1;

	float2 ygm = n.xy * 0.05;

	output.color = tex.Sample(sam, input.uv + ygm);

	return output;
}