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

	// �䂪�܂����ʂ������p�̃X�N���[���n���h��
	int waveScreen = MakeScreen(1280, 720, false);
	// �s�N�Z���V�F�[�_�[�̃v���O�������������ɓǂݍ��ނ悤
	// �R���p�C�����Ȃ��Ɠǂݍ��߂��A�R���p�C������ɂ�DxLib���ɂ���Tool\ShaderCompiler���g��
	// ��
	// �R�}���h�v�����v�g������
	int wavePs = LoadPixelShader(L"wavePs.pso");

	int graphHandle = LoadGraph(L"test.png");

	// �H�F�V�F�[�_�[�p�萔�o�b�t�@�n���h���̏�����
	int cbuffer = CreateShaderConstantBuffer(sizeof(float) * 4);
	// �����炭��L�̂��̂̃A�h���X���擾
	float* gAngle = static_cast<float*>(GetBufferShaderConstantBuffer(cbuffer));
	// �����炭��ʂ��䂪�܂���Ƃ��̒l�ύX�p�ϐ�
	float angle = 3.14f;
	// �H�F�V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@���w��̃V�F�[�_�[�̎w��̃X���b�g�ɃZ�b�g����
	SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 0);


	int ps = LoadPixelShader(L"ps.pso");

	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() != -1)
	{

		angle += 0.03125f;

		*gAngle = angle;

		// �����炭�V�F�[�_�[���̕ύX�̓K�p
		UpdateShaderConstantBuffer(cbuffer);
		// �H�F�w��̃X���b�g�ɃZ�b�g����͕̂ς��Ȃ��Ǝv�����ǂȂ�ŃX���b�g�����Ⴄ�̂��͕s��
		SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 3);

		// ��x��ʂɑS�������グ��
		SetDrawScreen(waveScreen);
		ClearDrawScreen();
		SetUsePixelShader(wavePs);
		MyGraph(0, 0, 1280, 720);

		// �{��ʂɕ`�悳����
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