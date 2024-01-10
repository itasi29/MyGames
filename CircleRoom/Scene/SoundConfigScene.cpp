#include <DxLib.h>
#include "Application.h"
#include "GameManager.h"

#include "SoundConfigScene.h"


namespace
{
	constexpr int kAppeaInterval = 60;
	constexpr int kMenuMargin = 60;
}

SoundConfigScene::SoundConfigScene(GameManager& mgr) :
	Scene(mgr)
{
	m_updateFunc = &SoundConfigScene::AppearUpdate;
	m_drawFunc = &SoundConfigScene::ExpandDraw;
}

SoundConfigScene::~SoundConfigScene()
{
}

void SoundConfigScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void SoundConfigScene::Draw()
{
	(this->*m_drawFunc)();
}

void SoundConfigScene::AppearUpdate(Input&)
{
	m_frame++;
	if (kAppeaInterval <= m_frame)
	{
		m_updateFunc = &SoundConfigScene::NormalUpdate;
		m_drawFunc = &SoundConfigScene::NormalDraw;
	}
}

void SoundConfigScene::NormalUpdate(Input&)
{
}

void SoundConfigScene::EditUpdate(Input&)
{
}

void SoundConfigScene::DisappearUpdate(Input&)
{
	m_frame--;
	if (m_frame == 0)
	{
		m_mgr.GetScene().PopScene();
	}
}

void SoundConfigScene::ExpandDraw()
{
	Application& app = Application::GetInstance();
	const auto& m_size = app.GetWindowSize();

	int halfHeight = (m_size.h - 100) / 2;
	int centerY = m_size.h / 2;

	float rate = static_cast<float>(m_frame) / kAppeaInterval;	// Œ»İ‚ÌŠÔ‚ÌŠ„‡(0.0`1.0)
	int currentHalfHeight = static_cast<int>(rate * halfHeight);

	// ‚¿‚å‚Á‚ÆˆÃ‚¢‹éŒ`‚ğ•`‰æ
	DrawBox(kMenuMargin, centerY - currentHalfHeight, m_size.w - kMenuMargin, centerY + currentHalfHeight,
		0x444444, true);
	DrawBox(kMenuMargin, centerY - currentHalfHeight, m_size.w - kMenuMargin, centerY + currentHalfHeight,
		0xffffff, false);
}

void SoundConfigScene::NormalDraw()
{
	Application& app = Application::GetInstance();
	const auto& m_size = app.GetWindowSize();
	// ‚¿‚å‚Á‚ÆˆÃ‚¢‹éŒ`‚ğ•`‰æ
	DrawBox(kMenuMargin, kMenuMargin, m_size.w - kMenuMargin, m_size.h - kMenuMargin,
		0x444444, true);

	DrawString(100, kMenuMargin + 10, L"KeyConfig Scene", 0xffffff);

	DrawBox(kMenuMargin, kMenuMargin, m_size.w - kMenuMargin, m_size.h - kMenuMargin,
		0xffffff, false);
}

void SoundConfigScene::DrawGauge(int drawX, int drawY, float rate)
{
}
