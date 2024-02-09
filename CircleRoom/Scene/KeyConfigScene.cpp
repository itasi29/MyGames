#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "Common/Input.h"
#include "GameManager.h"
#include "FileSystem/KeyFile.h"
#include "FileSystem/BottansFile.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/SoundSystem.h"
#include "StringUtility.h"

#include "SceneManager.h"
#include "ConfigScene.h"
#include "OptionScene.h"

#include "KeyConfigScene.h"

namespace
{
	// �t���[���̐F
	constexpr unsigned int kFrameColor = 0xd2001a;
	// �t���[���_�Ŏ��̐F�̍�(RGB��)
	constexpr unsigned int kFrameColorDeff = 0x225036;

	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// �I����������̐F
	constexpr unsigned int kYellowColor = 0xffde00;
	// �_�ŊԊu
	constexpr int kFlashInterval = 40;

	// �t���[���̍��]��
	constexpr int kFrameMargin = 16;

	constexpr unsigned int kDefColor = 0xffffff;
	constexpr int kMenuMargin = 120;

	constexpr int kMenuLineInterval = 48;

	constexpr double kExtendRate = 1.5;

	// �X�^�[�g�����̃E�F�[�u�X�s�[�h
	constexpr float kWaveSpeed = DX_PI_F / 180 * 5;
	// �E�F�[�u�̊Ԋu
	constexpr float kWaveInterval = DX_PI_F / 15.0f;

	// �E�F�[�u������
	int kSelectWaveNum = 4;
	int kSelectWavePosX = 1064;
	int kSelectWavePosY = 592;
	const wchar_t* const kSelectWave[] = { L"��", L"��", L"��", L"��" };

	int kBackWaveNum = 3;
	int kBackWavePosX = 1128;
	int kBackWavePosY = 544;
	const wchar_t* const kBackWave[] = { L"��", L"��", L"��" };
}

KeyConfigScene::KeyConfigScene(GameManager& mgr, Input& input, std::shared_ptr<SceneManager> scn) :
	Scene(mgr),
	m_optionScn(scn),
	m_input(input),
	m_currentLineIndex(0),
	m_isEdit(false),
	m_fadeFrame(0),
	m_cancleFrame(0),
	m_waveAngle(DX_PI_F),
	m_isWaveDraw(true)
{
	m_commandTable = input.GetCommandTable();
	m_updateFunc = &KeyConfigScene::EditEndUpdate;

	// ���j���[�ɕ��ԏ������
	m_menuTable = {
		"dash",		// �_�b�V��
		"OK",		// �I��or�m��
		"cancel",	// �L�����Z��
		"pause"		// �|�[�Y�{�^��
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

	auto& file = m_mgr.GetFile();
	m_frame = file->LoadGraphic(L"UI/normalFrame.png", true);
	m_addFrame = file->LoadGraphic(L"UI/addFrame.png");
	m_startFrame = file->LoadGraphic(L"UI/startFrame.png");

	m_selectSe = file->LoadSound(L"Se/select.mp3", true);
	m_cancelSe = file->LoadSound(L"Se/cancel.mp3", true);
	m_cursorUpSe = file->LoadSound(L"Se/cursorUp.mp3", true);
	m_cursorDownSe = file->LoadSound(L"Se/cursorDown.mp3", true);

	m_bt = std::make_shared<BottansFile>(file);
	m_key = std::make_shared<KeyFile>(file);

	const auto& size = Application::GetInstance().GetWindowSize();
}

KeyConfigScene::~KeyConfigScene()
{
	// ���̏�������(�R�ꂪ�Ȃ��悤��)
	for (const auto& cmd : m_commandTable)
	{
		m_input.m_commandTable[cmd.first] = cmd.second;
	}
	m_input.Save("Data/key.cnf");

	std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
	optionScene->InverseIsEdit();
}

void KeyConfigScene::Update(Input & input)
{
	m_isWaveDraw = true;
	m_waveAngle -= kWaveSpeed;
	(this->*m_updateFunc)(input);
}

void KeyConfigScene::Draw()
{
	DrawStringToHandle(100, kMenuMargin + 10, L"�L�[�ύX", 0xffffff, m_mgr.GetFont()->GetHandle(32));

	// �I�����Ă���ꏊ��`��
	int y = kMenuMargin + 62 + m_currentLineIndex * kMenuLineInterval;

	// �I�����Ă���ꏊ��`��
	DrawGraph(kMenuMargin + 800, y, m_frame->GetHandle(), true);
	DrawBox(128 - kFrameMargin, y,
		kMenuMargin + 800, y + 44,
		kFrameColor, true);

	// �I�𒆂̏ꍇ�͐F��ǉ����ē_�ł�����
	if (m_isEdit)
	{
		int frame = (m_fadeFrame % (kFlashInterval * 2)) - kFlashInterval;
		float rate = fabsf(static_cast<float>(frame)) / kFlashInterval;
		int add = static_cast<int>(255 * rate);
		SetDrawBlendMode(DX_BLENDMODE_ADD, add);
		DrawGraph(kMenuMargin + 800, y, m_addFrame->GetHandle(), true);
		DrawBox(128 - kFrameMargin, y,
			kMenuMargin + 800, y + 44,
			kFrameColorDeff, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		DrawWave(kSelectWavePosX, kSelectWavePosY, "OK", kSelectWave, kSelectWaveNum);
		m_isWaveDraw = true;
	}
	DrawWave(kBackWavePosX, kBackWavePosY, "cancel", kBackWave, kBackWaveNum);

	DrawCommandList();
}

void KeyConfigScene::NormalUpdate(Input & input)
{
	if (input.IsTriggered("cancel"))
	{
		m_sound->PlaySe(m_cancelSe->GetHandle());
		m_optionScn->ChangeScene(std::make_shared<ConfigScene>(m_mgr, input, m_optionScn));
	}

	if (input.IsReleased("OK"))
	{
		m_sound->PlaySe(m_selectSe->GetHandle());
		m_isEdit = true;
		m_updateFunc = &KeyConfigScene::EditUpdate;
		m_fadeFrame = kFlashInterval;
		m_cancleFrame = 0;
	}

	m_fadeFrame++;

	if (input.IsTriggered("up"))
	{
		m_currentLineIndex = (m_currentLineIndex - 1 + static_cast<int>(m_menuTable.size())) % static_cast<int>(m_menuTable.size());
		m_fadeFrame = 0;
		m_sound->PlaySe(m_cursorUpSe->GetHandle());
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % static_cast<int>(m_menuTable.size());
		m_fadeFrame = 0;
		m_sound->PlaySe(m_cursorDownSe->GetHandle());
	}
}

void KeyConfigScene::EditUpdate(Input & input)
{
	m_fadeFrame++;

	// ���ݑI�����Ă���R�}���h�̃f�[�^���Q��
	const auto& strItem = m_menuTable[m_currentLineIndex];
	auto& cmd = m_commandTable[strItem];

	if (input.IsPress("cancel"))
	{
		m_cancleFrame++;
		if (m_cancleFrame > 10)
		{
			m_sound->PlaySe(m_cancelSe->GetHandle());
			m_updateFunc = &KeyConfigScene::NormalUpdate;
			m_isEdit = false;
			m_fadeFrame = 0;
		}
		return;
	}
	if (m_cancleFrame > 0)
	{
		if (m_input.GetType() == InputType::pad)
		{
			m_sound->PlaySe(m_cancelSe->GetHandle());
			m_updateFunc = &KeyConfigScene::NormalUpdate;
			m_isEdit = false;
			m_fadeFrame = 0;
			return;
		}

		auto state = m_commandTable["cancel"][InputType::keybd];

		cmd[InputType::keybd] = state;

		// �{�̂̕�����������
		m_input.m_commandTable["cancel"][InputType::keybd] = state;

		m_isEdit = false;
		m_updateFunc = &KeyConfigScene::EditEndUpdate;
		return;
	}

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
				auto ret = input.m_commandTable[str][InputType::keybd][0];
				if (ret == i)
				{
					return;
				}
			}

			cmd[InputType::keybd][0] = i;

			// �{�̂̕�����������
			m_input.m_commandTable[strItem][InputType::keybd][0] = i;

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
		m_fadeFrame = 0;
	}
}

void KeyConfigScene::DrawCommandList()
{
	int y = kMenuMargin + 64;

	for (int i = 0; i < m_menuTable.size(); i++)
	{
		// �\������R�}���h�̏����擾
		auto& cmd = m_commandTable[m_menuTable[i]];

		std::wstring cmdName = StringUtility::StringToWString(m_menuTable[i]);
		int fontHandle = m_mgr.GetFont()->GetHandle(32);

		if (i == m_currentLineIndex)
		{
			if (!m_isEdit)
			{
				int frame = (m_fadeFrame % (kFlashInterval * 2)) - kFlashInterval;
				float rate = fabsf(static_cast<float>(frame)) / kFlashInterval;
				int alpha = static_cast <int>(255 * rate);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			}
			DrawFormatStringToHandle(kMenuMargin + 50, y, kYellowColor, fontHandle, L"%s", cmdName.c_str());
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			DrawFormatStringToHandle(kMenuMargin + 50, y, kWhiteColor, fontHandle, L"%s", cmdName.c_str());
		}

		m_keyImg->DrawKey(GetKeyName(cmd.at(InputType::keybd)[0]), kMenuMargin + 50 + 376, y, kExtendRate);

		y += kMenuLineInterval;
	}
}

void KeyConfigScene::CommitCurrenKeySetting()
{
	// input�{�̂̃L�[�������������Ă��܂��B
	for (const auto& cmd : m_commandTable)
	{
		m_input.m_commandTable[cmd.first] = cmd.second;
	}
	m_input.Save("Data/key.conf");
}

void KeyConfigScene::DrawWave(int x, int y, const char* const cmd, const wchar_t* const str[], int num)
{
	if (!m_isWaveDraw) return;
	m_isWaveDraw = false;

	DrawGraph(x - 84, y - 5, m_startFrame->GetHandle(), true);

	switch (m_input.GetType())
	{
	case InputType::keybd:
		m_key->DrawKey(m_input.GetHardDataName(cmd, InputType::keybd), x - 48, y, 2.0);
		break;
	default:
		assert(false);
	case InputType::pad:
		m_bt->DrawBottan(m_input.GetHardDataName(cmd, InputType::pad), x - 48, y, 2.0);
		break;
	}

	int handle = m_mgr.GetFont()->GetHandle(32);

	int strX = x;

	for (int i = 0; i < num; i++)
	{
		int add = static_cast<int>(sinf(m_waveAngle + kWaveInterval * i) * -10);

		if (add > 0)
		{
			add = 0;
		}

		int strY = y + add;


		DrawStringToHandle(strX, strY, str[i], kWhiteColor, handle);
		strX += 24;
	}
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