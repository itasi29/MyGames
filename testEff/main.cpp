#include <DxLib.h>
#include "Player.h"
#include "input.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// 一部の関数はDxLib_Init()の前に実行する必要がある
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// ダブルバッファリング
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

	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		DrawBox(0, 0, 640, 480, 0x757679, true);

		// ゲームの処理
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

		// 画面が切り替わるのを待つ
		ScreenFlip();

		// escキーでゲーム終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// FPS60に固定する
		// 16.66ms(16667マイクロ秒)経過するまで待つ
		while (GetNowHiPerformanceCount() - start < 16667) {}
	}

	DeleteGraph(handle);
	DeleteGraph(enemy);

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了
}