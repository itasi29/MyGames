#include <DxLib.h>
#include <array>

namespace
{
	constexpr int kWidth = 720;
	constexpr int kHeight = 720;

	constexpr float kRad = DX_PI_F / 180 * 0.1;
}

void MyGraph(int x, int y, int width, int height);

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// 一部の関数はDxLib_Init()の前に実行する必要がある
	ChangeWindowMode(true);
	SetGraphMode(1280, 720, 32);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// ダブルバッファリング
	SetDrawScreen(DX_SCREEN_BACK);
	
	// シェーダー画面
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



	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		// ゲームの処理
		angle -= kRad;
		gData[0] = angle;

		// おそらくシェーダー情報の変更の適用
		UpdateShaderConstantBuffer(cbuffer);
		// ？：指定のスロットにセットするのは変わらないと思うけどなんでスロット数が違うのかは不明
		SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 3);

		// 一度画面に全部書き上げる
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

	DeleteGraph(psScreen);
	DeleteGraph(handle);
	DeleteShader(ps);
	DeleteShader(psTexture);

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了
}

void MyGraph(int x, int y, int width, int height)
{
	std::array<VERTEX2DSHADER, 4> verts;
	for (auto& v : verts) {
		// これだけよくわからなかったが基本1でいいらしい
		v.rhw = 1.0f;
		// 拡散反射光
		v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff);
		// テクスチャ座標０
		v.u = 0.0f;
		v.v = 0.0f;
		v.su = 0.0f;
		// テクスチャ座標１
		v.sv = 0.0f;
		v.pos.z = 0.0f;
		// 鏡面反射光
		v.spc = GetColorU8(0, 0, 0, 0);
	}
	// 左上
	verts[0].pos.x = x;
	verts[0].pos.y = y;
	// 右上
	verts[1].pos.x = x + width;
	verts[1].pos.y = y;
	verts[1].u = 1.0f;
	// 左下
	verts[2].pos.x = x;
	verts[2].pos.y = y + height;
	verts[2].v = 1.0f;
	// 右下
	verts[3].pos.x = x + width;
	verts[3].pos.y = y + height;
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;
	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
}