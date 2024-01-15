#include <DxLib.h>
#include "Input.h"
#include "StringUtility.h"

#include "GameManager.h"
#include "SceneManager.h"
#include "FileSystem/BottansFile.h"

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
	m_frame(0)
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
}

PadConfigScene::~PadConfigScene()
{
	// 情報の書き換え(漏れがないように)
	for (const auto& cmd : m_commandTable)
	{
		m_input.m_commandTable[cmd.first] = cmd.second;
	}
	m_input.Save("key.cnf");

	std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
	optionScene->InverseIsEdit();
}

void PadConfigScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void PadConfigScene::Draw()
{
	DrawString(100, kMenuMargin + 10, L"PadConfigScene", 0xffffff);

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
	}

	if (input.IsTriggered("up"))
	{
		m_currentLineIndex = (m_currentLineIndex - 1 + m_menuTable.size()) % m_menuTable.size();
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % m_menuTable.size();
	}
}

void PadConfigScene::EditUpdate(Input& input)
{
	m_frame++;

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

		unsigned int lineColor = kDefColor;
		if (i == m_currentLineIndex)
		{
			lineColor = 0x000000;
		}

		std::wstring cmdName = StringUtility::StringToWString(m_menuTable[i]);

		// FIXME:フォントサイズでの方法に変更する可能性大
		//DrawFormatString(kMenuMargin + 50, y, lineColor, L"%s",	cmdName.c_str());
		DrawRotaFormatString(kMenuMargin + 50, y, kExtendRate, kExtendRate, 0.0, 0.0,
			0.0, lineColor, lineColor, 0, L"%s", cmdName.c_str());

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