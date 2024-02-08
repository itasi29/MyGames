#include <DxLib.h>
#include <array>
#include "Application.h"

#include "SceneManager.h"
#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "Scene.h"
#include "BackgroundScene.h"

namespace
{
	constexpr int kShakeSize = 40;

	// ��x�ɐi�܂����
	constexpr float kRad = DX_PI_F / 180 * 2.5;

	// ���E�����̑傫��
	constexpr int kWidth = 64;
	constexpr int kHeight = 320;
}

SceneManager::SceneManager(bool isDrawBg) :
	m_isShake(false),
	m_shakeFrame(0),
	m_shakeSize(kShakeSize),
	m_isDrawBg(isDrawBg),
	m_isMove(false),
	m_isBaseX(true),
	m_angle(0.0)
{
	m_updateFunc = &SceneManager::NormalUpdate;
	m_drawFunc = &SceneManager::NormalDraw;

	const size size = Application::GetInstance().GetWindowSize();
	m_shakeHandle = MakeScreen(size.w, size.h);
	m_drawScreen = MakeScreen(size.w, size.h);
	m_shaderScreen = MakeScreen(size.w, size.h);
	m_moveScreen = MakeScreen(size.w, size.h, true);
	m_gaussianBlurScreen = MakeScreen(size.w, size.h, true);

	m_wavePs = LoadPixelShader(L"Data/Shader/wavePs.pso");
	m_ps = LoadPixelShader(L"Data/Shader/ps.pso");

	m_cbuffer = CreateShaderConstantBuffer(sizeof(float) * 4);
	m_gAngle = static_cast<float*>(GetBufferShaderConstantBuffer(m_cbuffer));
}

SceneManager::~SceneManager()
{
	DeleteGraph(m_shakeHandle);
	DeleteGraph(m_drawScreen);
	DeleteGraph(m_shaderScreen);
	DeleteGraph(m_moveScreen);
	DeleteGraph(m_gaussianBlurScreen);

	DeleteShader(m_wavePs);
	DeleteShader(m_ps);

	DeleteShaderConstantBuffer(m_cbuffer);
}

void SceneManager::Init()
{
	if (m_bg) return;

	auto& file = GameManager::GetInstance().GetFile();
	m_bg = std::make_shared<BackgroundScene>(file, true, m_isDrawBg);
}

void SceneManager::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void SceneManager::Draw()
{
	(this->*m_drawFunc)();
}

void SceneManager::ChangeScene(std::shared_ptr<Scene>nextScene, float speed)
{
	m_bg->Init(speed);
	if (m_scenes.empty()) {	// ���X�g������ۂ����������ւ���̂ł͂Ȃ�
		m_scenes.push_back(nextScene);	// �����ɒǉ�����
	}
	else {
		m_scenes.back() = nextScene;	// ���łɈ�ȏ゠�����疖���̂��̂����ւ���
	}
}

void SceneManager::ChangeSceneWithClear(std::shared_ptr<Scene> nextScene, float speed)
{
	m_bg->Init(speed);
	// ��x���ׂď���
	m_scenes.clear();
	// �V�[����ǉ�
	m_scenes.push_back(nextScene);
}

void SceneManager::PushScene(std::shared_ptr<Scene> scene)
{
	m_scenes.push_back(scene);
}

void SceneManager::PopScene()
{
	m_scenes.pop_back();
}

void SceneManager::ShakeScreen(int frame)
{
	ShakeScreen(frame, kShakeSize);
}

void SceneManager::ShakeScreen(int frame, int size = kShakeSize)
{
	m_shakeFrame = frame;
	m_shakeSize = size;
	m_isShake = true;

	m_updateFunc = &SceneManager::ShakeUpdate;
	m_drawFunc = &SceneManager::ShakeDraw;
}

void SceneManager::MoveScreen(const Vec2& vec)
{
	m_isMove = true;

	m_vec = vec;
	// ���K��
	m_vec.Normalize();

	// �ǂ��炪�傫����
	m_isBaseX = (abs(m_vec.x) > abs(m_vec.y));

	m_angle = 0.0;

	// �����o�֐��|�C���^�̍X�V
	m_updateFunc = &SceneManager::MoveUpdate;
	m_drawFunc = &SceneManager::MoveDraw;
}

void SceneManager::OnShader()
{
	m_angle = 3.14f;

	m_updateFunc = &SceneManager::ShaderUpdate;
	m_drawFunc = &SceneManager::ShaderDraw;
}

void SceneManager::EndShader()
{
	m_updateFunc = &SceneManager::NormalUpdate;
	m_drawFunc = &SceneManager::NormalDraw;
}

void SceneManager::OnBgMove()
{
	m_updateFunc = &SceneManager::NormalUpdate;
	m_drawFunc = &SceneManager::NormalDraw;
}

void SceneManager::OnBgGaussianBlur()
{
	m_updateFunc = &SceneManager::GaussianBlurUpdate;
	m_drawFunc = &SceneManager::GaussianBlurDraw;
}

std::shared_ptr<Scene> SceneManager::GetTopScene()
{
	return m_scenes.back();
}

int SceneManager::GetScreenHandle() const
{
	if (m_isShake)
	{
		return m_shakeHandle;
	}
	else if (m_isMove)
	{ 
		return m_moveScreen;
	}
	else
	{
		return DX_SCREEN_BACK;
	}
}

void SceneManager::NormalUpdate(Input& input)
{
	// �w�i�̍X�V
	m_bg->Update();

	// �����̂ݎ��s
	m_scenes.back()->Update(input);
}

void SceneManager::ShakeUpdate(Input& input)
{
	NormalUpdate(input);

	m_shakeFrame--;

	if (m_shakeFrame < 0)
	{
		m_isShake = false;

		m_updateFunc = &SceneManager::NormalUpdate;
		m_drawFunc = &SceneManager::NormalDraw;
	}
}

void SceneManager::MoveUpdate(Input& input)
{
	NormalUpdate(input);

	m_angle += kRad;

	if (m_angle >= DX_PI)
	{
		m_isMove = false;

		m_updateFunc = &SceneManager::NormalUpdate;
		m_drawFunc = &SceneManager::NormalDraw;
	}
}

void SceneManager::ShaderUpdate(Input& input)
{
	NormalUpdate(input);

	m_angle += kRad;

	*m_gAngle = m_angle;

	UpdateShaderConstantBuffer(m_cbuffer);
	SetShaderConstantBuffer(m_cbuffer, DX_SHADERTYPE_PIXEL, 3);
}

void SceneManager::GaussianBlurUpdate(Input& input)
{
	NormalUpdate(input);
}

void SceneManager::NormalDraw() const
{
	m_bg->Draw();
	// �擪���珇�ɕ`��(�Ō�ɐς񂾂��̂��Ō�ɕ`�悳���)
	for (auto& scene : m_scenes)
	{
		scene->Draw();
	}
}

void SceneManager::ShakeDraw() const
{
	SetDrawScreen(m_shakeHandle);
	ClearDrawScreen();

	NormalDraw();

	SetDrawScreen(DX_SCREEN_BACK);

	int x = GetRand(m_shakeSize) - static_cast<int>(m_shakeSize * 0.5f);
	int y = GetRand(m_shakeSize) - static_cast<int>(m_shakeSize * 0.5f);
	DrawGraph(x, y, m_shakeHandle, true);
}

void SceneManager::MoveDraw() const
{
	m_bg->Draw();
	SetDrawScreen(m_moveScreen);
	ClearDrawScreen();
	for (auto& scene : m_scenes)
	{
		scene->Draw();
	}
	SetDrawScreen(DX_SCREEN_BACK);

	int x, y;

	float rate = -sinf(m_angle);
	float dRate = sinf(m_angle * 2) * 0.5f;
	if (m_isBaseX)
	{
		x = static_cast<int>(m_vec.x * rate * kHeight + m_vec.y * dRate * kWidth);
		y = static_cast<int>(m_vec.x * dRate * kWidth + m_vec.y * rate * kHeight);
	}
	else
	{
		x = static_cast<int>(m_vec.y * dRate * kWidth + m_vec.x * rate * kHeight);
		y = static_cast<int>(m_vec.y * rate * kHeight + m_vec.x * dRate * kWidth);
	}

	DrawGraph(x, y, m_moveScreen, true);
}

void SceneManager::ShaderDraw() const
{
	const size& size = Application::GetInstance().GetWindowSize();

	SetDrawScreen(m_drawScreen);
	ClearDrawScreen();
	NormalDraw();

	SetDrawScreen(m_shaderScreen);
	ClearDrawScreen();
	SetUsePixelShader(m_wavePs);
	MyDraw(0, 0, size.w, size.h);

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	SetUsePixelShader(m_ps);
	SetUseTextureToShader(0, m_drawScreen);
	SetUseTextureToShader(1, m_shaderScreen);
	MyDraw(0, 0, size.w, size.h);

}

void SceneManager::GaussianBlurDraw() const
{
	SetDrawScreen(m_gaussianBlurScreen);
	ClearDrawScreen();

	m_bg->Draw();

	auto itr = m_scenes.begin();
	// �Ō�̃V�[���ȊO��`�悳����������T�C�Y-1
	int index = m_scenes.size() - 1;
	for (int i = 0; i < index; i++)
	{
		itr->get()->Draw();
		itr++;
	}

	// �ڂ���
	GraphFilter(m_gaussianBlurScreen, DX_GRAPH_FILTER_GAUSS, 32, 1400);

	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, m_gaussianBlurScreen, false);
	itr->get()->Draw();

}

void SceneManager::MyDraw(int x, int y, int width, int height) const
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
	// �`��
	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
}
