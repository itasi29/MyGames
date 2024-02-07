#include <DxLib.h>
#include <array>

void MyGraph(int x, int y, int width, int height);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(true);
//	SetGraphMode(640, 360 * 2 + 100, 32);
	SetGraphMode(1280, 720, 32);
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// ゆがませる画面を書く用のスクリーンハンドル
	int waveScreen = MakeScreen(1280, 720, false);
	// ピクセルシェーダーのプログラムをメモリに読み込むよう
	// コンパイルしないと読み込めず、コンパイルするにはDxLib内にあるTool\ShaderCompilerを使う
	// ↓
	// コマンドプロンプトからやる
	int wavePs = LoadPixelShader(L"wavePs.pso");

	int graphHandle = LoadGraph(L"test.png");

	// ？：シェーダー用定数バッファハンドルの初期化
	int cbuffer = CreateShaderConstantBuffer(sizeof(float) * 4);
	// おそらく上記のもののアドレスを取得
	float* gAngle = static_cast<float*>(GetBufferShaderConstantBuffer(cbuffer));
	// おそらく画面をゆがませるときの値変更用変数
	float angle = 3.14f;
	// ？：シェーダー用定数バッファハンドルの定数バッファを指定のシェーダーの指定のスロットにセットする
	SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 0);


	int ps = LoadPixelShader(L"ps.pso");

	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() != -1)
	{

		angle += 0.03125f;

		*gAngle = angle;

		// おそらくシェーダー情報の変更の適用
		UpdateShaderConstantBuffer(cbuffer);
		// ？：指定のスロットにセットするのは変わらないと思うけどなんでスロット数が違うのかは不明
		SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 3);

		// 一度画面に全部書き上げる
		SetDrawScreen(waveScreen);
		ClearDrawScreen();
		SetUsePixelShader(wavePs);
		MyGraph(0, 0, 1280, 720);

		// 本画面に描画させる
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
#if true
		SetUsePixelShader(ps);
		SetUseTextureToShader(0, graphHandle);
		SetUseTextureToShader(1, waveScreen);
		MyGraph(0, 0, 1280, 720);
#else
		DrawGraph(0, 0, waveScreen, true);
#endif

		ScreenFlip();
	}

	DeleteGraph(waveScreen);
	DeleteGraph(graphHandle);

	DeleteShader(wavePs);
	DeleteShader(ps);

	DeleteShaderConstantBuffer(cbuffer);

	DxLib_End();

	return 0;
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