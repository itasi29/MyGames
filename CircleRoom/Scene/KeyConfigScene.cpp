#include "KeyConfigScene.h"
#include <DxLib.h>
#include "Common/Input.h"
#include "Application.h"
#include "GameManager.h"
#include "StringUtility.h"
#include "FileSystem/BottansFile.h"

namespace
{
	constexpr int kAppeaInterval = 60;
	constexpr int kMenuMargin = 60;
}

KeyConfigScene::KeyConfigScene(GameManager& mgr, Input& input) :
	Scene(mgr),
	m_input(input)
{
	m_keyCommandTable = input.GetCommandTable();
	m_updateFunc = &KeyConfigScene::AppearUpdate;
	m_drawFunc = &KeyConfigScene::ExpandDraw;

	// メニューに並ぶ順を作る
	m_menuItems = {
		"dash",		// ダッシュ
		"OK",		// 選択or確定
		"cancel",	// キャンセル
		"pause",	// ポーズボタン
		"keyconf"	// キーコンフィグボタン
	};

	m_keynameTable[KEY_INPUT_A] = L"Ａキー";
	m_keynameTable[KEY_INPUT_B] = L"Ｂキー";
	m_keynameTable[KEY_INPUT_C] = L"Ｃキー";
	m_keynameTable[KEY_INPUT_D] = L"Ｄキー";
	m_keynameTable[KEY_INPUT_E] = L"Ｅキー";
	m_keynameTable[KEY_INPUT_F] = L"Ｆキー";
	m_keynameTable[KEY_INPUT_G] = L"Ｇキー";
	m_keynameTable[KEY_INPUT_H] = L"Ｈキー";
	m_keynameTable[KEY_INPUT_I] = L"Ｉキー";
	m_keynameTable[KEY_INPUT_J] = L"Ｊキー";
	m_keynameTable[KEY_INPUT_K] = L"Ｋキー";
	m_keynameTable[KEY_INPUT_L] = L"Ｌキー";
	m_keynameTable[KEY_INPUT_M] = L"Ｍキー";
	m_keynameTable[KEY_INPUT_N] = L"Ｎキー";
	m_keynameTable[KEY_INPUT_O] = L"Ｏキー";
	m_keynameTable[KEY_INPUT_P] = L"Ｐキー";
	m_keynameTable[KEY_INPUT_Q] = L"Ｑキー";
	m_keynameTable[KEY_INPUT_R] = L"Ｒキー";
	m_keynameTable[KEY_INPUT_S] = L"Ｓキー";
	m_keynameTable[KEY_INPUT_T] = L"Ｔキー";
	m_keynameTable[KEY_INPUT_U] = L"Ｕキー";
	m_keynameTable[KEY_INPUT_V] = L"Ｖキー";
	m_keynameTable[KEY_INPUT_W] = L"Ｗキー";
	m_keynameTable[KEY_INPUT_X] = L"Ｘキー";
	m_keynameTable[KEY_INPUT_Y] = L"Ｙキー";
	m_keynameTable[KEY_INPUT_Z] = L"Ｚキー";
	m_keynameTable[KEY_INPUT_BACK] = L"BSキー";
	m_keynameTable[KEY_INPUT_TAB] = L"Tabキー";
	m_keynameTable[KEY_INPUT_RETURN] = L"Enterキー";
	m_keynameTable[KEY_INPUT_LSHIFT] = L"左Shiftキー";
	m_keynameTable[KEY_INPUT_RSHIFT] = L"右Shiftキー";
	m_keynameTable[KEY_INPUT_LCONTROL] = L"左Ctrlキー";
	m_keynameTable[KEY_INPUT_RCONTROL] = L"右Ctrlキー";
	m_keynameTable[KEY_INPUT_ESCAPE] = L"Escキー";
	m_keynameTable[KEY_INPUT_SPACE] = L"スペースキー";

	m_bottans[PAD_INPUT_A] = L"ＡBottan";
	m_bottans[PAD_INPUT_B] = L"ＢBottan";
	m_bottans[PAD_INPUT_C] = L"ＣBottan";
	m_bottans[PAD_INPUT_X] = L"ＸBottan";
	m_bottans[PAD_INPUT_Y] = L"ＹBottan";
	m_bottans[PAD_INPUT_Z] = L"ＺBottan";
	m_bottans[PAD_INPUT_L] = L"ＬBottan";
	m_bottans[PAD_INPUT_R] = L"ＲBottan";
	m_bottans[PAD_INPUT_START] = L"ＳＴＡＲＴBottan";
	m_bottans[PAD_INPUT_M] = L"ＭBottan";

	m_btImg = std::make_shared<BottansFile>(m_mgr.GetFile());
}

KeyConfigScene::~KeyConfigScene()
{
}

void KeyConfigScene::Update(Input & input)
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

void KeyConfigScene::NormalUpdate(Input & input)
{
	// トグル処理
	if (input.IsTriggered("OK"))
	{
		if (m_currentLineIndex < m_keyCommandTable.size())
		{
			m_isEditRequestButton = true;
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

	if (m_isEditRequestButton)
	{
		if (input.IsReleased("OK"))
		{
			m_isEditingNow = !m_isEditingNow;
			m_updateFunc = &KeyConfigScene::EditUpdate;
			m_isEditRequestButton = false;
			return;
		}
	}

	if (input.IsTriggered("pause"))
	{
		m_updateFunc = &KeyConfigScene::DisappearUpdate;
		m_drawFunc = &KeyConfigScene::ExpandDraw;
		m_frame = kAppeaInterval;
	}

	int size = static_cast<int>(m_keyCommandTable.size() + 1);
	if (input.IsTriggered("up"))
	{
		m_currentLineIndex = (m_currentLineIndex + size - 1) % size;
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % size;
	}
}

void KeyConfigScene::EditUpdate(Input & input)
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
		m_mgr.GetScene().PopScene();
	}
}

void KeyConfigScene::ExpandDraw()
{
	Application& app = Application::GetInstance();
	const auto& m_size = app.GetWindowSize();

	int halfHeight = (m_size.h - 100) / 2;
	int centerY = m_size.h / 2;

	float rate = static_cast<float>(m_frame) / kAppeaInterval;	// 現在の時間の割合(0.0～1.0)
	int currentHalfHeight = static_cast<int>(rate * halfHeight);

	// ちょっと暗い矩形を描画
	DrawBox(kMenuMargin, centerY - currentHalfHeight, m_size.w - kMenuMargin, centerY + currentHalfHeight,
		0x444444, true);
	DrawBox(kMenuMargin, centerY - currentHalfHeight, m_size.w - kMenuMargin, centerY + currentHalfHeight,
		0xffffff, false);
}

void KeyConfigScene::NormalDraw()
{
	Application& app = Application::GetInstance();
	const auto& m_size = app.GetWindowSize();
	// ちょっと暗い矩形を描画
	DrawBox(kMenuMargin, kMenuMargin, m_size.w - kMenuMargin, m_size.h - kMenuMargin,
		0x444444, true);

	DrawString(100, kMenuMargin + 10, L"KeyConfig Scene", 0xffffff);

	DrawBox(kMenuMargin, kMenuMargin, m_size.w - kMenuMargin, m_size.h - kMenuMargin,
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
		auto keyname = GetKeyName(cmd.at(InputType::keybd));
		auto padname = GetPadName(cmd.at(InputType::pad));
		DrawFormatString(x, y, lineColor, L"%s : keybd=%s , pad=%s",
			cmdName.c_str(), // コマンド名
			keyname.c_str(),	// キーボードの値
			padname.c_str());		// パッドの値
		m_btImg->DrawBottan(padname, x + 376, y);

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
	m_input.Save("key.conf");
}

std::wstring KeyConfigScene::GetKeyName(int keycode)
{
	wchar_t name[16];
	auto it = m_keynameTable.find(keycode);
	if (it == m_keynameTable.end())
	{
		wsprintf(name, L"%02x", keycode);
		return name;
	}
	return (it->second);
}

std::wstring KeyConfigScene::GetPadName(int padstate)
{
	wchar_t name[16];
	auto it = m_bottans.find(padstate);
	if (it == m_bottans.end())
	{
		wsprintf(name, L"%04x", padstate);
		return name;
	}
	return (it->second);
}