#include "KeyConfigScene.h"
#include <DxLib.h>
#include "../Input.h"
#include "../Application.h"
#include "SceneManager.h"
#include "../StringUtility.h"

namespace
{
	constexpr int kAppeaInterval = 60;
	constexpr int kMenuMargin = 60;
}

KeyConfigScene::KeyConfigScene(SceneManager& mgr, Input& input) : 
	Scene(mgr),
	m_input(input)
{
	m_keyCommandTable = input.GetCommandTable();
	m_updateFunc = &KeyConfigScene::AppearUpdate;
	m_drawFunc = &KeyConfigScene::ExpandDraw;
}

KeyConfigScene::~KeyConfigScene()
{
}

void KeyConfigScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void KeyConfigScene::Draw()
{
	(this->*m_drawFunc)();
}

void KeyConfigScene::AppearUpdate(Input&)
{
	m_frame++;
	if (kAppeaInterval <= m_frame)
	{
		m_updateFunc = &KeyConfigScene::NormalUpdate;
		m_drawFunc = &KeyConfigScene::NormalDraw;
	}
}

void KeyConfigScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("keyconf"))
	{
		m_updateFunc = &KeyConfigScene::DisappearUpdate;
		m_drawFunc = &KeyConfigScene::ExpandDraw;
		m_frame = kAppeaInterval;
	}
	else if (input.IsTriggered("keyconf"))
	{

	}
}

void KeyConfigScene::DisappearUpdate(Input&)
{
	m_frame--;
	if (m_frame == 0)
	{
		m_manager.PopScene();
	}
}

void KeyConfigScene::ExpandDraw()
{
	Application& app = Application::GetInstance();
	const auto& size = app.GetWindowSize();

	int halfHeight = (size.h - 100) / 2;
	int centerY = size.h / 2;

	float rate = static_cast<float>(m_frame) / kAppeaInterval;	// Œ»İ‚ÌŠÔ‚ÌŠ„‡(0.0`1.0)
	int currentHalfHeight = rate * halfHeight;

	// ‚¿‚å‚Á‚ÆˆÃ‚¢‹éŒ`‚ğ•`‰æ
	DrawBox(kMenuMargin, centerY - currentHalfHeight, size.w - kMenuMargin, centerY + currentHalfHeight,
		0x444444, true);
	DrawBox(kMenuMargin, centerY - currentHalfHeight, size.w - kMenuMargin, centerY + currentHalfHeight,
		0xffffff, false);
}

void KeyConfigScene::NormalDraw()
{
	Application& app = Application::GetInstance();
	const auto& size = app.GetWindowSize();
	// ‚¿‚å‚Á‚ÆˆÃ‚¢‹éŒ`‚ğ•`‰æ
	DrawBox(kMenuMargin, kMenuMargin, size.w - kMenuMargin, size.h - kMenuMargin,
		0x444444, true);

	DrawString(100, kMenuMargin + 10, L"KeyConfig Scene", 0xffffff);

	DrawBox(kMenuMargin, kMenuMargin, size.w - kMenuMargin, size.h - kMenuMargin,
		0xffffff, false);

	DrawCommandList();
}

void KeyConfigScene::DrawCommandList()
{
	int x = kMenuMargin + 50;
	int y = kMenuMargin + 50 + 10;

	for (const auto& cmd : m_keyCommandTable)
	{
		std::wstring cmdName = StringUtility::StringToWString(cmd.first);
		DrawFormatString(x, y, 0xffffff, L"%s", cmdName.c_str());
		y += 20;
	}
}

