#include <DxLib.h>
#include "Input.h"
#include "StringUtility.h"

#include "GameManager.h"
#include "SceneManager.h"
#include "FileSystem/BottansFile.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/SoundSystem.h"

#include "ConfigScene.h"
#include "OptionScene.h"
#include "PadConfigScene.h"

namespace
{
	constexpr unsigned int kDefColor = 0xffffff;
	constexpr int kMenuMargin = 120;

	constexpr double kExtendRate = 1.5;
}

PadConfigScene::PadConfigScene(GameManager& mgr, Input& input, std::shared_ptr<SceneManager> scn) :
	Scene(mgr),
	m_optionScn(scn),
	m_input(input),
	m_currentLineIndex(0),
	m_isEdit(false),
	m_frame(0),
	m_cancleFrame(0)
{
	m_updateFunc = &PadConfigScene::EditEndUpdate;

	m_commandTable = input.GetCommandTable();

	m_menuTable = {
		"dash",		// ダッシュ
		"OK",		// 選択or確定
		"cancel",	// キャンセル
		"pause",	// ポーズボタン
		"keyconf"	// キーコンフィグボタン
	};

	m_bottanTable[PAD_INPUT_A] = L"ＡBottan";
	m_bottanTable[PAD_INPUT_B] = L"ＢBottan";
	m_bottanTable[PAD_INPUT_C] = L"ＣBottan";
	m_bottanTable[PAD_INPUT_X] = L"ＸBottan";
	m_bottanTable[PAD_INPUT_Y] = L"ＹBottan";
	m_bottanTable[PAD_INPUT_Z] = L"ＺBottan";
	m_bottanTable[PAD_INPUT_L] = L"ＬBottan";
	m_bottanTable[PAD_INPUT_R] = L"ＲBottan";
	m_bottanTable[PAD_INPUT_START] = L"ＳＴＡＲＴBottan";
	m_bottanTable[PAD_INPUT_M] = L"ＭBottan";

	m_btImg = std::make_shared<BottansFile>(m_mgr.GetFile());

	std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
	optionScene->InverseIsEdit();

	auto& file = m_mgr.GetFile();
	m_cursorUpSe = file->LoadSound(L"Se/cursorUp.mp3", true);
	m_cursorDownSe = file->LoadSound(L"Se/cursorDown.mp3", true);
}

PadConfigScene::~PadConfigScene()
{
	// 情報の書き換え(漏れがないように)
	for (const auto& cmd : m_commandTable)
	{
		m_input.m_commandTable[cmd.first] = cmd.second;
	}
	m_input.Save("Data/Bin/key.cnf");

	std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
	optionScene->InverseIsEdit();
}

void PadConfigScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void PadConfigScene::Draw()
{
	DrawStringToHandle(100, kMenuMargin + 10, L"PadConfigScene", 0xffffff, m_mgr.GetFont()->GetHandle(32));

	DrawCommandList();
}

void PadConfigScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("cancel"))
	{
		m_optionScn->ChangeScene(std::make_shared<ConfigScene>(m_mgr, m_optionScn));
	}

	if (input.IsReleased("OK"))
	{
		m_isEdit = true;
		m_updateFunc = &PadConfigScene::EditUpdate;
		m_frame = 0;
		m_cancleFrame = 0;
	}

	m_frame++;

	if (input.IsTriggered("up"))
	{
		m_currentLineIndex = (m_currentLineIndex - 1 + static_cast<int>(m_menuTable.size())) % static_cast<int>(m_menuTable.size());
		m_frame = 0;
		m_sound->PlaySe(m_cursorUpSe->GetHandle());
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % m_menuTable.size();
		m_frame = 0;
		m_sound->PlaySe(m_cursorDownSe->GetHandle());
	}
}

void PadConfigScene::EditUpdate(Input& input)
{
	m_frame++;

	if (input.IsPress("cancel"))
	{
		m_cancleFrame++;
		if (m_cancleFrame > 10)
		{
			m_updateFunc = &PadConfigScene::NormalUpdate;
			m_isEdit = false;
			m_frame = 0;
		}
		return;
	}
	m_cancleFrame = 0;

	// 現在選択しているコマンドのデータを参照
	auto strItem = m_menuTable[m_currentLineIndex];
	auto& cmd = m_commandTable[strItem];

	// PADが入力されたら変更
	int padstate = GetJoypadInputState(DX_INPUT_PAD1);
	if (padstate)
	{
		// MEMO:隠しているコマンドと同じ場合は変更できないようにする
		for (const auto& str : m_input.GetExclusiveCommandTable())
		{
			// コマンドと変更するKeyが同じなら変えずに処理終了
			auto ret = input.m_commandTable[str][InputType::pad];
			if (ret == padstate)
			{
				return;
			}
		}

		cmd[InputType::pad] = padstate;

		// 本体の方も書き換え
		m_input.m_commandTable[strItem][InputType::pad] = padstate;

		m_isEdit = false;
		m_updateFunc = &PadConfigScene::EditEndUpdate;
		return;
	}
}

void PadConfigScene::EditEndUpdate(Input& input)
{
	// OKボタンが押されていないかつ、リリース状態でもないなら通常状態に戻る
	if (input.IsNotPress("OK") && !input.IsReleased("OK"))
	{
		m_updateFunc = &PadConfigScene::NormalUpdate;
		m_frame = 0;
	}
}

void PadConfigScene::DrawCommandList()
{
	// 選択している場所を描画
	if (!m_isEdit || static_cast<int>(m_frame * 0.05f) % 2)
	{
		DrawBox(128, static_cast<int>(kMenuMargin + 64 + m_currentLineIndex * 48),
			kMenuMargin + 800, static_cast<int>(kMenuMargin + 64 + 48 + m_currentLineIndex * 48),
			0xff0000, true);
	}
	else
	{
		DrawBox(128, static_cast<int>(kMenuMargin + 64 + m_currentLineIndex * 48),
			kMenuMargin + 800, static_cast<int>(kMenuMargin + 64 + 48 + m_currentLineIndex * 48),
			0xff8800, true);
	}

	int y = kMenuMargin + 64;

	for (int i = 0; i < m_menuTable.size(); i++)
	{
		// 表示するコマンドの情報を取得
		auto& cmd = m_commandTable[m_menuTable[i]];

		std::wstring cmdName = StringUtility::StringToWString(m_menuTable[i]);
		int fontHandle = m_mgr.GetFont()->GetHandle(32);

		if (i == m_currentLineIndex)
		{
			if (!m_isEdit)
			{
				int frame = (m_frame % 80) - 40;
				float rate = fabsf(static_cast<float>(frame)) / 40.0f;
				int alpha = static_cast <int>(255 * rate);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			}
			DrawFormatStringToHandle(kMenuMargin + 50, y, 0x000000, fontHandle, L"%s", cmdName.c_str());
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			DrawFormatStringToHandle(kMenuMargin + 50, y, 0xffffff, fontHandle, L"%s", cmdName.c_str());
		}

		m_btImg->DrawBottan(GetPadName(cmd.at(InputType::pad)), kMenuMargin + 50 + 376, y, kExtendRate);

		y += 48;
	}
}

std::wstring PadConfigScene::GetPadName(int padstate)
{
	wchar_t name[16];
	auto it = m_bottanTable.find(padstate);
	if (it == m_bottanTable.end())
	{
		wsprintf(name, L"%04x", padstate);
		return name;
	}
	return (it->second);
}