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

	// メニューに並ぶ順を作る
	m_menuItems = {
		"OK",		// 選択or確定
		"cancel",	// キャンセル
		"pause",	// ポーズボタン
		"keyconf"	// キーコンフィグボタン
	};
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
	// トグル処理
	if (input.IsTriggered("OK"))
	{
		if (m_currentLineIndex < m_keyCommandTable.size())
		{
			m_isEditingNow = !m_isEditingNow;
			m_updateFunc = &KeyConfigScene::EditUpdate;
		}
		else // 確定
		{
			CommitCurrenKeySetting();

			m_updateFunc = &KeyConfigScene::DisappearUpdate;
			m_drawFunc = &KeyConfigScene::ExpandDraw;
			m_frame = kAppeaInterval;
		}

		return;
	}

	if (input.IsTriggered("keyconf"))
	{
		m_updateFunc = &KeyConfigScene::DisappearUpdate;
		m_drawFunc = &KeyConfigScene::ExpandDraw;
		m_frame = kAppeaInterval;
	}

	auto size = m_keyCommandTable.size() + 1;
	if (input.IsTriggered("up"))
	{
		m_currentLineIndex = (m_currentLineIndex + size - 1) % size;
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % size;
	}
}

void KeyConfigScene::EditUpdate(Input& input)
{
	// トグル処理
	if (input.IsTriggered("OK"))
	{
		m_isEditingNow = !m_isEditingNow;
		m_updateFunc = &KeyConfigScene::NormalUpdate;

		return;
	}

	char keystate[256];
	GetHitKeyStateAll(keystate);
	int padstate = GetJoypadInputState(DX_INPUT_PAD1);

	auto strItem = m_menuItems[m_currentLineIndex];
	auto& cmd = m_keyCommandTable[strItem];
	for (int i = 0; i < 256; i++)
	{
		if (keystate[i])
		{
			cmd[InputType::keybd] = i;
			break;
		}
	}
	if (padstate)
	{
		cmd[InputType::pad] = padstate;
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

	float rate = static_cast<float>(m_frame) / kAppeaInterval;	// 現在の時間の割合(0.0～1.0)
	int currentHalfHeight = static_cast<int>(rate * halfHeight);

	// ちょっと暗い矩形を描画
	DrawBox(kMenuMargin, centerY - currentHalfHeight, size.w - kMenuMargin, centerY + currentHalfHeight,
		0x444444, true);
	DrawBox(kMenuMargin, centerY - currentHalfHeight, size.w - kMenuMargin, centerY + currentHalfHeight,
		0xffffff, false);
}

void KeyConfigScene::NormalDraw()
{
	Application& app = Application::GetInstance();
	const auto& size = app.GetWindowSize();
	// ちょっと暗い矩形を描画
	DrawBox(kMenuMargin, kMenuMargin, size.w - kMenuMargin, size.h - kMenuMargin,
		0x444444, true);

	DrawString(100, kMenuMargin + 10, L"KeyConfig Scene", 0xffffff);

	DrawBox(kMenuMargin, kMenuMargin, size.w - kMenuMargin, size.h - kMenuMargin,
		0xffffff, false);

	DrawCommandList();
}

void KeyConfigScene::DrawCommandList()
{
	constexpr int kLineHeight = 30;
	int y = kMenuMargin + 50 + 10;
	int idx = 0;
	constexpr unsigned int kDefaultColor = 0xffffff;
	for (const auto& item : m_menuItems)
	{
		auto& cmd = m_keyCommandTable[item];
		unsigned int lineColor = kDefaultColor;
		int x = kMenuMargin + 50;
		std::wstring cmdName = StringUtility::StringToWString(item);
		if (idx == m_currentLineIndex)
		{
			DrawString(x - 20, y, L"⇒", 0xff0000);
			x += 10;
			if (m_isEditingNow)
			{
				lineColor = 0xffaa00;
				x += 5;
			}
		}
		DrawFormatString(x, y, lineColor, L"%s : keybd=%02x , pad=%03x",
										cmdName.c_str(), // コマンド名
										cmd.at(InputType::keybd),	// キーボードの値
										cmd.at(InputType::pad));		// パッドの値
		y += 20;
		idx++;
	}
	y += kLineHeight;
	int x = kMenuMargin + 200;
	unsigned int lineColor = kDefaultColor;
	if (m_currentLineIndex == m_keyCommandTable.size())
	{
		x += 10;
		DrawString(x - 20, y, L"⇒", 0xff0000);
	}
	DrawString(x, y, L"確定", lineColor);
}

void KeyConfigScene::CommitCurrenKeySetting()
{
	// input本体のキー情報を書き換えています。
	for (const auto& cmd : m_keyCommandTable)
	{
		m_input.m_commandTable[cmd.first] = cmd.second;
	}
}

