#include <DxLib.h>
#include <array>

#if true
void MyGraph(int x, int y, int width, int height);

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// �ꕔ�̊֐���DxLib_Init()�̑O�Ɏ��s����K�v������
	ChangeWindowMode(true);
	SetGraphMode(1280, 720, 32);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// �_�u���o�b�t�@�����O
	SetDrawScreen(DX_SCREEN_BACK);

	int handle = LoadGraph("bg.png");
	int rtScreen = MakeScreen(1280, 720, true);

	auto ps = LoadPixelShader("GaussianBlur.pso");
	auto ps2 = LoadPixelShader("ColorGradhing.pso");

	auto cbuffer = CreateShaderConstantBuffer(sizeof(float*) * 4);
	auto gFrame = static_cast<float*>(GetBufferShaderConstantBuffer(cbuffer));
	SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 0);
	float frame = 0;

	bool isHit = false;

	// �Q�[�����[�v
	while (ProcessMessage() != -1)
	{
		// ���̃t���[���̊J�n�������o���Ă���
		LONGLONG start = GetNowHiPerformanceCount();



		if (CheckHitKey(KEY_INPUT_UP))
		{
			if (!isHit)
			{
				isHit = true;

				frame = static_cast<float>((3 + static_cast<int>(frame) - 1) % 3);
			}
		}
		else if (CheckHitKey(KEY_INPUT_DOWN))
		{
			if (!isHit)
			{
				isHit = true;

				frame = static_cast<float>((static_cast<int>(frame) + 1) % 3);
			}
		}
		else
		{
			isHit = false;
		}

		gFrame[0] = frame;

		UpdateShaderConstantBuffer(cbuffer);
		SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 3);



		// �`����s���O�ɉ�ʂ��N���A����
		SetDrawScreen(rtScreen);
		ClearDrawScreen();
		SetUsePixelShader(ps2);
		SetUseTextureToShader(0, handle);
		MyGraph(0, 0, 1280, 720);

		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		DrawGraph(0, 0, rtScreen, true);

		DrawFormatString(100, 100, 0x00ff00, "%f", frame);

		// ��ʂ��؂�ւ��̂�҂�
		ScreenFlip();

		// esc�L�[�ŃQ�[���I��
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// FPS60�ɌŒ肷��
		// 16.66ms(16667�}�C�N���b)�o�߂���܂ő҂�
		while (GetNowHiPerformanceCount() - start < 16667) {}
	}

	DeleteGraph(handle);
	DeleteShader(ps);

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I��
}

void MyGraph(int x, int y, int width, int height)
{
	std::array<VERTEX2DSHADER, 4> verts;
	for (auto& v : verts) {
		// ���ꂾ���悭�킩��Ȃ���������{1�ł����炵��
		v.rhw = 1.0f;
		// �g�U���ˌ�
		v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff);
		// �e�N�X�`�����W�O
		v.u = 0.0f;
		v.v = 0.0f;
		v.su = 0.0f;
		// �e�N�X�`�����W�P
		v.sv = 0.0f;
		v.pos.z = 0.0f;
		// ���ʔ��ˌ�
		v.spc = GetColorU8(0, 0, 0, 0);
	}
	// ����
	verts[0].pos.x = x;
	verts[0].pos.y = y;
	// �E��
	verts[1].pos.x = x + width;
	verts[1].pos.y = y;
	verts[1].u = 1.0f;
	// ����
	verts[2].pos.x = x;
	verts[2].pos.y = y + height;
	verts[2].v = 1.0f;
	// �E��
	verts[3].pos.x = x + width;
	verts[3].pos.y = y + height;
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;
	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
}

#else

// �K�E�X�ڂ������s���s�N�Z���V�F�[�_�[�̃R�[�h
const char* pixelShaderCode = R"(
Texture2D<float4> TextureInput : register(t0);
SamplerState SamplerLinear : register(s0);

float4 PS_GaussianBlur(float2 texCoord : TEXCOORD0) : SV_TARGET
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float weights[9] = {0.05, 0.09, 0.12, 0.15, 0.18, 0.15, 0.12, 0.09, 0.05}; // �K�E�X�ڂ����̏d��

    // �K�E�X�ڂ�����K�p
    for (int i = -4; i <= 4; ++i)
    {
        color += TextureInput.Sample(SamplerLinear, texCoord + float2(i * 0.001f, 0)) * weights[i + 4];
    }

    return color;
}
)";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// DxLib�̏�����
	if (DxLib_Init() == -1) return -1;

	// ��ʃ��[�h�ݒ�
	SetGraphMode(800, 600, 32);

	// �s�N�Z���V�F�[�_�[�̍쐬
	int pixelShaderHandle = LoadPixelShaderMem(pixelShaderCode, strlen(pixelShaderCode), "PS_GaussianBlur", "ps_5_0");

	// �摜��ǂݍ��ށi��F"image.png"��ǂݍ��ޏꍇ�j
	int textureHandle = LoadGraph("image.png");

	// ���C�����[�v
	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 && ProcessMessage() == 0)
	{
		// ��ʂ��o�b�N�o�b�t�@�ɃR�s�[����
		SetDrawScreen(DX_SCREEN_BACK);

		// �e�N�X�`���̃Z�b�g�A�b�v
		SetTextureToShader(textureHandle, "TextureInput");

		// �K�E�X�ڂ����̓K�p
		SetUsePixelShader(pixelShaderHandle);
		DrawRectGraph(0, 0, 0, 0, 800, 600, textureHandle, TRUE);

		// �t�����g�o�b�t�@�֕`��𔽉f������
		ScreenFlip();
	}

	// DxLib�̏I������
	DxLib_End();

	return 0;
}

#endif