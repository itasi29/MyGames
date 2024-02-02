#include <DxLib.h>
#include "Player.h"
#include "input.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// �ꕔ�̊֐���DxLib_Init()�̑O�Ɏ��s����K�v������
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// �_�u���o�b�t�@�����O
	SetDrawScreen(DX_SCREEN_BACK);
	Player player;
	Input input;

	int handle = MakeScreen(640, 480);
	int enemy = LoadGraph("Normal.png");
	int shadow = LoadGraph("ShadowNormal.png");

	int radius1 = 64;
	int radius2 = 32;
	int radius3 = 16;
	int kLineThickness = 4;

	// �Q�[�����[�v
	while (ProcessMessage() != -1)
	{
		// ���̃t���[���̊J�n�������o���Ă���
		LONGLONG start = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A����
		ClearDrawScreen();

		DrawBox(0, 0, 640, 480, 0x757679, true);

		// �Q�[���̏���
#if false
		radius1 += kLineThickness;
		radius2 += kLineThickness;
		radius3 += kLineThickness;
		if (CheckHitKey(KEY_INPUT_RETURN))
		{
			radius1 = 64;
			radius2 = 32;
			radius3 = 16;
		}

		DrawBox(0, 0, 640, 480, 0x00ff00, true);

		SetDrawScreen(handle);
		SetDrawBlendMode(DX_BLENDMODE_MULA, 16);
		DrawBox(0, 0, 640, 480, 0x000000, true);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawCircle(320, 240, radius3, 0x789461, false, kLineThickness);
		DrawCircle(320, 240, radius2, 0x50623a, false, kLineThickness);
		DrawCircle(320, 240, radius1, 0x294b29, false, kLineThickness);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawScreen(DX_SCREEN_BACK);
		DrawGraph(0, 0, handle, true);
#else
		input.Update();
		player.Update(input);
		DrawRotaGraph(320 + 10, 240 + 10, 1.0, 0.0, shadow, true);
		DrawRotaGraph(320, 240, 1.0, 0.0, enemy, true);
		player.Draw();
#endif

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
	DeleteGraph(enemy);

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I��
}