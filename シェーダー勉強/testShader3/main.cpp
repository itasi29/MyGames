#include <DxLib.h>
#include <array>

#if true
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

	int handle = LoadGraph("bg.png");
	int rtScreen = MakeScreen(1280, 720, true);

	auto ps = LoadPixelShader("GaussianBlur.pso");
	auto ps2 = LoadPixelShader("ColorGradhing.pso");

	auto cbuffer = CreateShaderConstantBuffer(sizeof(float*) * 4);
	auto gFrame = static_cast<float*>(GetBufferShaderConstantBuffer(cbuffer));
	SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 0);
	float frame = 0;

	bool isHit = false;

	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// このフレームの開始時刻を覚えておく
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



		// 描画を行う前に画面をクリアする
		SetDrawScreen(rtScreen);
		ClearDrawScreen();
		SetUsePixelShader(ps2);
		SetUseTextureToShader(0, handle);
		MyGraph(0, 0, 1280, 720);

		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		DrawGraph(0, 0, rtScreen, true);

		DrawFormatString(100, 100, 0x00ff00, "%f", frame);

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
	DeleteShader(ps);

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

#else

// ガウスぼかしを行うピクセルシェーダーのコード
const char* pixelShaderCode = R"(
Texture2D<float4> TextureInput : register(t0);
SamplerState SamplerLinear : register(s0);

float4 PS_GaussianBlur(float2 texCoord : TEXCOORD0) : SV_TARGET
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float weights[9] = {0.05, 0.09, 0.12, 0.15, 0.18, 0.15, 0.12, 0.09, 0.05}; // ガウスぼかしの重み

    // ガウスぼかしを適用
    for (int i = -4; i <= 4; ++i)
    {
        color += TextureInput.Sample(SamplerLinear, texCoord + float2(i * 0.001f, 0)) * weights[i + 4];
    }

    return color;
}
)";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// DxLibの初期化
	if (DxLib_Init() == -1) return -1;

	// 画面モード設定
	SetGraphMode(800, 600, 32);

	// ピクセルシェーダーの作成
	int pixelShaderHandle = LoadPixelShaderMem(pixelShaderCode, strlen(pixelShaderCode), "PS_GaussianBlur", "ps_5_0");

	// 画像を読み込む（例："image.png"を読み込む場合）
	int textureHandle = LoadGraph("image.png");

	// メインループ
	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 && ProcessMessage() == 0)
	{
		// 画面をバックバッファにコピーする
		SetDrawScreen(DX_SCREEN_BACK);

		// テクスチャのセットアップ
		SetTextureToShader(textureHandle, "TextureInput");

		// ガウスぼかしの適用
		SetUsePixelShader(pixelShaderHandle);
		DrawRectGraph(0, 0, 0, 0, 800, 600, textureHandle, TRUE);

		// フロントバッファへ描画を反映させる
		ScreenFlip();
	}

	// DxLibの終了処理
	DxLib_End();

	return 0;
}

#endif