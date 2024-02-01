#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "Common/Input.h"

#include "OptionScene.h"

#include "GameManager.h"
#include "SceneManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/BottansFile.h"
#include "FileSystem/KeyFile.h"

#include "StageSelectScene.h"
#include "ConfigScene.h"
#include "SoundOptionScene.h"
#include "OtherOptionScene.h"
#include "TitleScene.h"

namespace
{
	// �t���[���̐F
	constexpr unsigned int kFrameColor = 0xd2001a;
	// �ʏ핶����̐F
	constexpr unsigned int kStrColor = 0xf0ece5;
	// �I����������̐F
	constexpr unsigned int kYellowColor = 0xffde00;

	// �t�F�[�h����
	constexpr int kAppeaInterval = 5;

	// �]��
	constexpr int kMenuMargin = 50;
	// �I��g�̏c��
	constexpr int kMenuMarginHeight = 32;

	// �Q�[�����I��
	constexpr int kGameMargin = 270;
	// �^�C�g�����I��
	constexpr int kTitleMargin = 360;

	// �^�C�g���ł̕�����
	const std::vector<std::wstring> kTitleMenu = { L"����ݒ�",
		L"���ʐݒ�",
		L"���̑�" };

	// �Q�[�����̕�����
	const std::vector<std::wstring> kGameMenu = { L"�X�e�[�W�I��",
		L"����ݒ�",
		L"���ʐݒ�",
		L"���̑�" };

	enum
	{
		kStageSelect,
		kOperat,
		kValume,
		kOther
	};
}

OptionScene::OptionScene(GameManager& mgr, Input& input, bool isGame) :
	Scene(mgr),
	m_isGame(isGame),
	m_isEdit{ false },
	m_currentMenuLine(0),
	m_isFadeOut(false),
	m_type(InputType::pad)
{
	m_updateFunc = &OptionScene::AppearUpdate;

	m_bt = std::make_shared<BottansFile>(m_mgr.GetFile());
	m_key = std::make_shared<KeyFile>(m_mgr.GetFile());

	auto& file = m_mgr.GetFile();
	m_cursorUpSe = file->LoadSound(L"Se/cursorUp.mp3", true);
	m_cursorDownSe = file->LoadSound(L"Se/cursorDown.mp3", true);

	m_optionScn = std::make_shared<SceneManager>(false);
	m_optionScn->Init();

	ChangeScene(input);
}

void OptionScene::Update(Input& input)
{
	m_isEdit[1] = m_isEdit[0];

	m_optionScn->Update(input);

	// �ҏW���͏����̕ύX�����Ȃ�
	if (m_isEdit[1]) return;
	(this->*m_updateFunc)(input);
}

void OptionScene::Draw()
{
	// �t�F�[�h�A�E�g���͕`�悵�Ȃ�
	if (m_isFadeOut) return;

	// MEMO:�����ŕύX������\��������̂Ō��̂Ɏ����`�ŏ����Ă���
	// MEMO:�ύX�Ȃ���Ί֐����ނ̂�߂�
	NormalDraw();
	m_optionScn->Draw();
}

void OptionScene::ChangeScene(std::shared_ptr<Scene> scene)
{
	m_optionScn->ChangeScene(scene);
}

void OptionScene::AppearUpdate(Input&)
{
	m_frame++;
	if (kAppeaInterval <= m_frame)
	{
		m_updateFunc = &OptionScene::NormalUpdate;
	}
}

void OptionScene::NormalUpdate(Input& input)
{
	m_type = input.GetType();

	if (input.IsTriggered("pause"))
	{
		m_updateFunc = &OptionScene::DisappearUpdate;
		m_isFadeOut = true;

		return;
	}

	if (input.IsTriggered("cancel"))
	{
		m_updateFunc = &OptionScene::DisappearUpdate;
		m_isFadeOut = true;

		return;
	}

	if (input.IsTriggered("optionLeft"))
	{
		if (m_isGame)
		{
			m_currentMenuLine = (m_currentMenuLine - 1 + static_cast<int>(kGameMenu.size())) % static_cast<int>(kGameMenu.size());
		}
		else
		{
			m_currentMenuLine = (m_currentMenuLine - 1 + static_cast<int>(kTitleMenu.size())) % static_cast<int>(kTitleMenu.size());
		}
		m_sound->PlaySe(m_cursorUpSe->GetHandle());
		ChangeScene(input);
	}
	if (input.IsTriggered("optionRight"))
	{
		if (m_isGame)
		{
			m_currentMenuLine = (m_currentMenuLine + 1) % static_cast<int>(kGameMenu.size());
		}
		else
		{
			m_currentMenuLine = (m_currentMenuLine + 1) % static_cast<int>(kTitleMenu.size());
		}
		m_sound->PlaySe(m_cursorDownSe->GetHandle());
		ChangeScene(input);
	}
}

void OptionScene::DisappearUpdate(Input&)
{
	m_frame--;
	if (m_frame > 0) return;

	m_mgr.GetScene()->PopScene();
}

void OptionScene::NormalDraw()
{
	Application& app = Application::GetInstance();
	const auto& size = app.GetWindowSize();
	// ������ƈÂ���`��`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 189);
	DrawBox(kMenuMargin, kMenuMargin, size.w - kMenuMargin, size.h - kMenuMargin,
		0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// �g��
	DrawBox(kMenuMargin, kMenuMargin, size.w - kMenuMargin, size.h - kMenuMargin,
		0xffffff, false);

	DrawWave(size);
	if (m_isGame)
	{
		DrawFrame(static_cast<int>(kGameMenu.size()), kGameMargin, size);
		DrawContent(kGameMenu, kGameMargin);
	}
	else
	{
		DrawFrame(static_cast<int>(kTitleMenu.size()), kTitleMargin, size);
		DrawContent(kTitleMenu, kTitleMargin);
	}
}

void OptionScene::DrawWave(const size& size)
{
	switch (m_type)
	{
	case InputType::keybd:
		m_key->DrawKey(L"�p�L�[", kMenuMargin + 7, kMenuMargin, 2.5);
		m_key->DrawKey(L"�d�L�[", size.w - kMenuMargin - 43, kMenuMargin, 2.5);
		break;
	default:
		assert(false);
	case InputType::pad:
		// L�AR�{�^���̕`��
		m_bt->DrawBottan(L"�kBottan", kMenuMargin + 7, kMenuMargin, 2.5);
		m_bt->DrawBottan(L"�qBottan", size.w - kMenuMargin - 48, kMenuMargin, 2.5);
		break;
	}
}

void OptionScene::DrawFrame(int divisionNum, int width, const size& size)
{
	int y = kMenuMargin + kMenuMarginHeight;
	// ���̐��̕`��
	DrawLine(kMenuMargin, y, size.w - kMenuMargin, y, 0xffffff);

	int x = kMenuMargin * 2;

	for (int i = 0; i < divisionNum + 1; i++)
	{
		DrawLine(x, kMenuMargin, x, y, 0xffffff);

		x += width;
	}
}

void OptionScene::DrawContent(std::vector<std::wstring> strs, int width)
{
	// �I�����Ă���ꏊ��`��
	int x = kMenuMargin * 2;
	DrawBox(x + width * m_currentMenuLine + 1, static_cast<int>(kMenuMargin + 1),
		x + width * (m_currentMenuLine + 1), static_cast<int>(kMenuMargin + kMenuMarginHeight),
		kFrameColor, true);

	int fontHandle = m_mgr.GetFont()->GetHandle(32);
	unsigned int color;
	// ���j���[�̕�����Q
	for (int i = 0; i < strs.size(); i++)
	{
		color = kStrColor;
		if (m_currentMenuLine == i)
		{
			color = kYellowColor;
		}
		DrawStringToHandle(x + width * i, kMenuMargin, strs[i].c_str(), color, fontHandle);
	}
}

void OptionScene::ChangeScene(Input& input)
{
	int current = m_currentMenuLine;

	if (!m_isGame)
	{
		// �^�C�g�����ƃX�e�[�W�I�����Ȃ����߂͂��߂��΂�
		current++;
	}

	switch (current)
	{
		// �X�e�[�W�I��
	case kStageSelect:
		m_optionScn->ChangeScene(std::make_shared<StageSelectScene>(m_mgr));
		break;

		// �L�[�ݒ�
	case kOperat:
		m_optionScn->ChangeScene(std::make_shared<ConfigScene>(m_mgr, input, m_optionScn));
		break;

		// ���ʐݒ�
	case kValume:
		m_optionScn->ChangeScene(std::make_shared<SoundOptionScene>(m_mgr, input));
		break;

		// ���̑��ݒ�
	default:
		assert(false);
	case kOther:
		m_optionScn->ChangeScene(std::make_shared<OtherOptionScene>(m_mgr, input, m_optionScn));
		break;
	}
}
