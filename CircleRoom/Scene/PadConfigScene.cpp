#include <DxLib.h>
#include <cassert>
#include "Input.h"
#include "StringUtility.h"

#include "GameManager.h"
#include "SceneManager.h"
#include "FileSystem/KeyFile.h"
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

PadConfigScene::PadConfigScene(GameManager& mgr, Input& input, std::shared_ptr<SceneManager> scn) :
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
	m_updateFunc = &PadConfigScene::EditEndUpdate;

	m_commandTable = input.GetCommandTable();

	m_menuTable = {
		"dash",		// �_�b�V��
		"OK",		// �I��or�m��
		"cancel",	// �L�����Z��
		"pause"		// �|�[�Y�{�^��
	};

	m_bottanTable[PAD_INPUT_A] = L"�`Bottan";
	m_bottanTable[PAD_INPUT_B] = L"�aBottan";
	m_bottanTable[PAD_INPUT_C] = L"�wBottan";
	m_bottanTable[PAD_INPUT_X] = L"�xBottan";
	m_bottanTable[PAD_INPUT_5] = L"�kBottan";
	m_bottanTable[PAD_INPUT_6] = L"�qBottan";
	m_bottanTable[PAD_INPUT_R] = L"�r�s�`�q�sBottan";
	m_bottanTable[PAD_INPUT_L] = L"�lBottan";

	m_btImg = std::make_shared<BottansFile>(m_mgr.GetFile());

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

	m_frameScreen = MakeScreen(1280, 720, true);
}

PadConfigScene::~PadConfigScene()
{
	// ���̏�������(�R�ꂪ�Ȃ��悤��)
	for (const auto& cmd : m_commandTable)
	{
		m_input.m_commandTable[cmd.first] = cmd.second;
	}
	m_input.Save("Data/key.cnf");

	std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
	optionScene->InverseIsEdit();

	DeleteGraph(m_frameScreen);
}

void PadConfigScene::Update(Input& input)
{
	m_isWaveDraw = true;
	m_waveAngle -= kWaveSpeed;
	(this->*m_updateFunc)(input);
}

void PadConfigScene::Draw()
{
	DrawStringToHandle(100, kMenuMargin + 10, L"�p�b�h�ύX", 0xffffff, m_mgr.GetFont()->GetHandle(32));

	// �I�����Ă���ꏊ��`��
	int y = kMenuMargin + 62 + m_currentLineIndex * kMenuLineInterval;

	SetDrawScreen(m_frameScreen);
	ClearDrawScreen();
	// �I�����Ă���ꏊ��`��
	DrawGraph(kMenuMargin + 800, y, m_frame->GetHandle(), true);
	DrawBox(128 - kFrameMargin, y,
		kMenuMargin + 800, y + 44,
		kFrameColor, true);

	int nowScreen = m_mgr.GetScene()->GetScreenHandle();
	SetDrawScreen(nowScreen);
	GraphFilter(m_frameScreen, DX_GRAPH_FILTER_HSB, 0, -15 * m_currentLineIndex, 0, 0);
	DrawGraph(0, 0, m_frameScreen, true);

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

void PadConfigScene::NormalUpdate(Input& input)
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
		m_updateFunc = &PadConfigScene::EditUpdate;
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
		m_currentLineIndex = (m_currentLineIndex + 1) % m_menuTable.size();
		m_fadeFrame = 0;
		m_sound->PlaySe(m_cursorDownSe->GetHandle());
	}
}

void PadConfigScene::EditUpdate(Input& input)
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
			m_updateFunc = &PadConfigScene::NormalUpdate;
			m_isEdit = false;
			m_fadeFrame = 0;
		}
		return;
	}
	// ��莞�ԓ���cancel����������cancel�̒l������
	if (m_cancleFrame > 0)
	{
		if (m_input.GetType() == InputType::keybd)
		{
			m_sound->PlaySe(m_cancelSe->GetHandle());
			m_updateFunc = &PadConfigScene::NormalUpdate;
			m_isEdit = false;
			m_fadeFrame = 0;
			return;
		}

		auto state = m_commandTable["cancel"][InputType::pad];

		cmd[InputType::pad] = state;

		// �{�̂̕�����������
		m_input.m_commandTable[strItem][InputType::pad] = state;

		m_isEdit = false;
		m_updateFunc = &PadConfigScene::EditEndUpdate;
		return;
	}

	// PAD�����͂��ꂽ��ύX
	int padstate = GetJoypadInputState(DX_INPUT_PAD1);
	if (padstate)
	{
		// MEMO:�B���Ă���R�}���h�Ɠ����ꍇ�͕ύX�ł��Ȃ��悤�ɂ���
		for (const auto& str : m_input.GetExclusiveCommandTable())
		{
			// �R�}���h�ƕύX����Key�������Ȃ�ς����ɏ����I��
			auto ret = input.m_commandTable[str][InputType::pad][0];
			if (ret == padstate)
			{
				return;
			}
		}

		cmd[InputType::pad][0] = padstate;

		// �{�̂̕�����������
		m_input.m_commandTable[strItem][InputType::pad][0] = padstate;

		m_isEdit = false;
		m_updateFunc = &PadConfigScene::EditEndUpdate;
		return;
	}
}

void PadConfigScene::EditEndUpdate(Input& input)
{
	// OK�{�^����������Ă��Ȃ����A�����[�X��Ԃł��Ȃ��Ȃ�ʏ��Ԃɖ߂�
	if (input.IsNotPress("OK") && !input.IsReleased("OK"))
	{
		m_updateFunc = &PadConfigScene::NormalUpdate;
		m_fadeFrame = 0;
	}
}

void PadConfigScene::DrawCommandList()
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

		m_btImg->DrawBottan(GetPadName(cmd.at(InputType::pad)[0]), kMenuMargin + 50 + 376, y, kExtendRate);

		y += kMenuLineInterval;
	}
}

void PadConfigScene::DrawWave(int x, int y, const char* const cmd, const wchar_t* const str[], int num)
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