#include "PauseScene.h"
#include <DxLib.h>
#include "../Input.h"
#include "../Application.h"
#include "SceneManager.h"
#include "KeyConfigScene.h"

namespace
{
	constexpr int kAppeaInterval = 60;
	constexpr int kMenuMargin = 50;
}

PauseScene::PauseScene(SceneManager& mgr) : Scene(mgr)
{
	m_updateFunc = &PauseScene::AppearUpdate;
	m_drawFunc = &PauseScene::ExpandDraw;
}

void PauseScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void PauseScene::Draw()
{
	(this->*m_drawFunc)();
}

void PauseScene::AppearUpdate(Input&)
{
	m_frame++;
	if (kAppeaInterval <= m_frame)
	{
		m_updateFunc = &PauseScene::NormalUpdate;
		m_drawFunc = &PauseScene::NormalDraw;
	}
}

void PauseScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("pause"))
	{
		m_updateFunc = &PauseScene::DisappearUpdate;
		m_drawFunc = &PauseScene::ExpandDraw;
	}
	else if (input.IsTriggered("keyconf"))
	{
		m_manager.PushScene(std::make_shared<KeyConfigScene>(m_manager, input));
	}
}

void PauseScene::DisappearUpdate(Input&)
{
	m_frame--;
	if (m_frame == 0)
	{
		m_manager.PopScene();
	}
}

void PauseScene::ExpandDraw()
{
	Application& app = Application::GetInstance();
	const auto& size = app.GetWindowSize();

	int halfHeight = (size.h - 100) / 2;
	int centerY = size.h / 2;

	float rate = static_cast<float>(m_frame) / kAppeaInterval;	// åªç›ÇÃéûä‘ÇÃäÑçá(0.0Å`1.0)
	int currentHalfHeight = static_cast<int>(rate * halfHeight);

	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	// ÇøÇÂÇ¡Ç∆à√Ç¢ãÈå`Çï`âÊ
	DrawBox(kMenuMargin, centerY - currentHalfHeight, size.w - kMenuMargin, centerY + currentHalfHeight,
		0x888888, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(kMenuMargin, centerY - currentHalfHeight, size.w - kMenuMargin, centerY + currentHalfHeight,
		0xffffff, false);
}

void PauseScene::NormalDraw()
{
	Application& app = Application::GetInstance();
	const auto& size = app.GetWindowSize();
	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	// ÇøÇÂÇ¡Ç∆à√Ç¢ãÈå`Çï`âÊ
	DrawBox(kMenuMargin, kMenuMargin, size.w - kMenuMargin, size.h - kMenuMargin,
		0x888888, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(100, 100, L"Pause Scene", 0xffffff);

	DrawBox(kMenuMargin, kMenuMargin, size.w - kMenuMargin, size.h - kMenuMargin,
		0xffffff, false);
}
