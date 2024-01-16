#include <DxLib.h>
#include "Input.h"
#include "Application.h"

#include "GameManager.h"
#include "SceneManager.h"

#include "OneShotScene.h"

namespace
{
	// �摜���㉺�Ɉړ�����t���[��
	constexpr int kApeearFrame = 10;

	// �摜�T�C�Y�̌Œ艻
	constexpr float kSize = 0.8f;

	// �`��ʒu
	constexpr int kDrawX = 128;
	constexpr int kDrawY = 72;
}

OneShotScene::OneShotScene(GameManager& mgr, int handle) :
	Scene(mgr),
	m_handle(handle),
	m_frame(0)
{
	m_updateFunc = &OneShotScene::AppearUpdate;
	m_drawFunc = &OneShotScene::MoveDraw;

	/* �摜�T�C�Y�Œ艻�p���� */
	// �摜�̃T�C�Y���擾����
	int graphSizeW, graphSizeH;
	GetGraphSize(m_handle, &graphSizeW, &graphSizeH);

	const size& screenSize = Application::GetInstance().GetWindowSize();

	// �c���̔{�����v�Z����
	m_sizeW = static_cast<double>(screenSize.w * kSize / graphSizeW);
	m_sizeH = static_cast<double>(screenSize.h * kSize / graphSizeH);

}

OneShotScene::~OneShotScene()
{
}

void OneShotScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void OneShotScene::Draw()
{
	(this->*m_drawFunc)();
}

void OneShotScene::AppearUpdate(Input& input)
{
	m_frame++;
	if (m_frame > kApeearFrame)
	{
		m_updateFunc = &OneShotScene::NormalUpdate;
		m_drawFunc = &OneShotScene::NormalDraw;
	}
}

void OneShotScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		m_updateFunc = &OneShotScene::DisAppearUpdate;
		m_drawFunc = &OneShotScene::MoveDraw;

		m_frame = kApeearFrame;
	}
}

void OneShotScene::DisAppearUpdate(Input& input)
{
	m_frame--;
	if (m_frame < 0)
	{
		m_mgr.GetScene()->PopScene();
	}
}

void OneShotScene::MoveDraw()
{
	const size& size = Application::GetInstance().GetWindowSize();

	int y = kDrawY - static_cast<int>((1.0f - m_frame / static_cast<float>(kApeearFrame)) * size.h);

	DrawRotaGraph3(kDrawX, y,
		0, 0, m_sizeW, m_sizeH, 0.0, m_handle, false);
}

void OneShotScene::NormalDraw()
{
	DrawRotaGraph3(kDrawX, kDrawY,
		0, 0, m_sizeW, m_sizeH, 0.0, m_handle, false);
}
