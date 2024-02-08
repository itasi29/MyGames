#include <DxLib.h>
#include <array>

namespace
{
	constexpr int kWidth = 720;
	constexpr int kHeight = 720;

	constexpr float kRad = DX_PI_F / 180 * 0.1;
}

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
	
	// �V�F�[�_�[���
	auto psScreen = MakeScreen(kWidth, kHeight, true);

	float angle = 0.0f;

	auto cbuffer = CreateShaderConstantBuffer(sizeof(float) * 4);
	auto gData = static_cast<float*>(GetBufferShaderConstantBuffer(cbuffer));
	SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 0);

	auto ps = LoadPixelShader("ps.pso");
	auto psTexture = LoadPixelShader("waveTexture.pso");


	auto handle = LoadGraph("test.png");

	bool isClick = false;
	int x = 640;
	int y = 360;



	// �Q�[�����[�v
	while (ProcessMessage() != -1)
	{
		// ���̃t���[���̊J�n�������o���Ă���
		LONGLONG start = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A����
		ClearDrawScreen();

		// �Q�[���̏���
		angle -= kRad;
		gData[0] = angle;

		// �����炭�V�F�[�_�[���̕ύX�̓K�p
		UpdateShaderConstantBuffer(cbuffer);
		// �H�F�w��̃X���b�g�ɃZ�b�g����͕̂ς��Ȃ��Ǝv�����ǂȂ�ŃX���b�g�����Ⴄ�̂��͕s��
		SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 3);

		// ��x��ʂɑS�������グ��
		SetDrawScreen(psScreen);
		ClearDrawScreen();
		SetUsePixelShader(ps);
		SetUseTextureToShader(0, handle);
		MyGraph(0, 0, kWidth, kHeight);

		SetDrawScreen(DX_SCREEN_BACK);
#if false
		SetUsePixelShader(psTexture);
		SetUseTextureToShader(0, handle);
		SetUseTextureToShader(1, psScreen);
		MyGraph(0, 0, 1280, 720);
#else
		DrawRotaGraph(x, y, 1.0, 0.0, psScreen, true);
#endif
		


		clsDx();
		printfDx("%.3f", angle);


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

	DeleteGraph(psScreen);
	DeleteGraph(handle);
	DeleteShader(ps);
	DeleteShader(psTexture);

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