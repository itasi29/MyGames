#include <DxLib.h>
#include "Application.h"
#include "Common/Input.h"
#include "GameManager.h"
#include "FileSystem/KeyFile.h"
#include "StringUtility.h"

#include "SceneManager.h"
#include "ConfigScene.h"
#include "OptionScene.h"

#include "KeyConfigScene.h"

namespace
{
	constexpr unsigned int kDefColor = 0xffffff;
	constexpr int kMenuMargin = 120;

	constexpr double kExtendRate = 1.5;
}

KeyConfigScene::KeyConfigScene(GameManager& mgr, Input& input, std::shared_ptr<SceneManager> scn) :
	Scene(mgr),
	m_optionScn(scn),
	m_input(input),
	m_currentLineIndex(0),
	m_isEdit(false),
	m_frame(0)
{
	m_commandTable = input.GetCommandTable();
	m_updateFunc = &KeyConfigScene::EditEndUpdate;

	// ���j���[�ɕ��ԏ������
	m_menuTable = {
		"dash",		// �_�b�V��
		"OK",		// �I��or�m��
		"cancel",	// �L�����Z��
		"pause",	// �|�[�Y�{�^��
		"keyconf"	// �L�[�R���t�B�O�{�^��
	};

	m_keynameTable[KEY_INPUT_A] = L"�`�L�[";
	m_keynameTable[KEY_INPUT_B] = L"�a�L�[";
	m_keynameTable[KEY_INPUT_C] = L"�b�L�[";
	m_keynameTable[KEY_INPUT_D] = L"�c�L�[";
	m_keynameTable[KEY_INPUT_E] = L"�d�L�[";
	m_keynameTable[KEY_INPUT_F] = L"�e�L�[";
	m_keynameTable[KEY_INPUT_G] = L"�f�L�[";
	m_keynameTable[KEY_INPUT_H] = L"�g�L�[";
	m_keynameTable[KEY_INPUT_I] = L"�h�L�[";
	m_keynameTable[KEY_INPUT_J] = L"�i�L�[";
	m_keynameTable[KEY_INPUT_K] = L"�j�L�[";
	m_keynameTable[KEY_INPUT_L] = L"�k�L�[";
	m_keynameTable[KEY_INPUT_M] = L"�l�L�[";
	m_keynameTable[KEY_INPUT_N] = L"�m�L�[";
	m_keynameTable[KEY_INPUT_O] = L"�n�L�[";
	m_keynameTable[KEY_INPUT_P] = L"�o�L�[";
	m_keynameTable[KEY_INPUT_Q] = L"�p�L�[";
	m_keynameTable[KEY_INPUT_R] = L"�q�L�[";
	m_keynameTable[KEY_INPUT_S] = L"�r�L�[";
	m_keynameTable[KEY_INPUT_T] = L"�s�L�[";
	m_keynameTable[KEY_INPUT_U] = L"�t�L�[";
	m_keynameTable[KEY_INPUT_V] = L"�u�L�[";
	m_keynameTable[KEY_INPUT_W] = L"�v�L�[";
	m_keynameTable[KEY_INPUT_X] = L"�w�L�[";
	m_keynameTable[KEY_INPUT_Y] = L"�x�L�[";
	m_keynameTable[KEY_INPUT_Z] = L"�y�L�[";
	m_keynameTable[KEY_INPUT_BACK] = L"BS�L�[";
	m_keynameTable[KEY_INPUT_TAB] = L"Tab�L�[";
	m_keynameTable[KEY_INPUT_RETURN] = L"Enter�L�[";
	m_keynameTable[KEY_INPUT_LSHIFT] = L"��Shift�L�[";
	m_keynameTable[KEY_INPUT_RSHIFT] = L"�EShift�L�[";
	m_keynameTable[KEY_INPUT_LCONTROL] = L"��Ctrl�L�[";
	m_keynameTable[KEY_INPUT_RCONTROL] = L"�ECtrl�L�[";
	m_keynameTable[KEY_INPUT_ESCAPE] = L"Esc�L�[";
	m_keynameTable[KEY_INPUT_SPACE] = L"�X�y�[�X�L�[";

	m_keyImg = std::make_shared<KeyFile>(m_mgr.GetFile());

	std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
	optionScene->InverseIsEdit();
}

KeyConfigScene::~KeyConfigScene()
{
	// ���̏�������(�R�ꂪ�Ȃ��悤��)
	for (const auto& cmd : m_commandTable)
	{
		m_input.m_commandTable[cmd.first] = cmd.second;
	}
	m_input.Save("key.cnf");

	std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
	optionScene->InverseIsEdit();
}

void KeyConfigScene::Update(Input & input)
{
	(this->*m_updateFunc)(input);
}

void KeyConfigScene::Draw()
{
	DrawString(100, kMenuMargin + 10, L"KeyConfig Scene", 0xffffff);

	DrawCommandList();
}

void KeyConfigScene::NormalUpdate(Input & input)
{
	if (input.IsTriggered("cancel"))
	{
		m_optionScn->ChangeScene(std::make_shared<ConfigScene>(m_mgr, m_optionScn));
	}

	if (input.IsReleased("OK"))
	{
		m_isEdit = true;
		m_updateFunc = &KeyConfigScene::EditUpdate;
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

void KeyConfigScene::EditUpdate(Input & input)
{
	m_frame++;

	// ���ݑI�����Ă���R�}���h�̃f�[�^���Q��
	auto strItem = m_menuTable[m_currentLineIndex];
	auto& cmd = m_commandTable[strItem];

	// KEY�����͂��ꂽ��ύX
	char keystate[256];
	GetHitKeyStateAll(keystate);
	for (int i = 0; i < 256; i++)
	{
		if (keystate[i])
		{
			// MEMO:�B���Ă���R�}���h�Ɠ����ꍇ�͕ύX�ł��Ȃ��悤�ɂ���
			for (const auto& str : m_input.GetExclusiveCommandTable())
			{
				// �R�}���h�ƕύX����Key�������Ȃ�ς����ɏ����I��
				auto ret = input.m_commandTable[str][InputType::keybd];
				if (ret == i)
				{
					return;
				}
			}

			cmd[InputType::keybd] = i;

			// �{�̂̕�����������
			m_input.m_commandTable[strItem][InputType::keybd] = i;

			m_updateFunc = &KeyConfigScene::EditEndUpdate;
			m_isEdit = false;
			break;
		}
	}
}

void KeyConfigScene::EditEndUpdate(Input& input)
{
	// OK�{�^����������Ă��Ȃ����A�����[�X��Ԃł��Ȃ��Ȃ�ʏ��Ԃɖ߂�
	if (input.IsNotPress("OK") && !input.IsReleased("OK"))
	{
		m_updateFunc = &KeyConfigScene::NormalUpdate;
	}
}

void KeyConfigScene::DrawCommandList()
{
	// �I�����Ă���ꏊ��`��
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
		// �\������R�}���h�̏����擾
		auto& cmd = m_commandTable[m_menuTable[i]];

		unsigned int lineColor = kDefColor;
		if (i == m_currentLineIndex)
		{
			lineColor = 0x000000;
		}

		std::wstring cmdName = StringUtility::StringToWString(m_menuTable[i]);

		// FIXME:�t�H���g�T�C�Y�ł̕��@�ɕύX����\����
		//DrawFormatString(kMenuMargin + 50, y, lineColor, L"%s",	cmdName.c_str());
		DrawRotaFormatString(kMenuMargin + 50, y, kExtendRate, kExtendRate, 0.0, 0.0,
			0.0, lineColor, lineColor, 0, L"%s", cmdName.c_str());

		m_keyImg->DrawKey(GetKeyName(cmd.at(InputType::keybd)), kMenuMargin + 50 + 376, y, kExtendRate);

		y += 48;
	}
}

void KeyConfigScene::CommitCurrenKeySetting()
{
	// input�{�̂̃L�[�������������Ă��܂��B
	for (const auto& cmd : m_commandTable)
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