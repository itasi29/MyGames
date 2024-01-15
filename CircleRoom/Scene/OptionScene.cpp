#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "Common/Input.h"

#include "OptionScene.h"

#include "GameManager.h"
#include "SceneManager.h"

#include "StageSelectScene.h"
#include "ConfigScene.h"
#include "SoundOptionScene.h"
#include "OtherOptionScene.h"
#include "TitleScene.h"

namespace
{
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
	m_isFadeOut(false)
{
	m_updateFunc = &OptionScene::AppearUpdate;

	m_optionScn = std::make_shared<SceneManager>();

	ChangeScene(input);
}

void OptionScene::Update(Input& input)
{
	m_isEdit[1] = m_isEdit[0];
	if (input.IsTriggered("pause"))
	{
		m_updateFunc = &OptionScene::DisappearUpdate;
		m_isFadeOut = true;

		return;
	}

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
	const auto& m_size = app.GetWindowSize();
	// ������ƈÂ���`��`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 126);
	DrawBox(kMenuMargin, kMenuMargin, m_size.w - kMenuMargin, m_size.h - kMenuMargin,
		0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// �g��
	DrawBox(kMenuMargin, kMenuMargin, m_size.w - kMenuMargin, m_size.h - kMenuMargin,
		0xffffff, false);

	if (m_isGame)
	{
		DrawContent(kGameMenu, kGameMargin);
	}
	else
	{
		DrawContent(kTitleMenu, kTitleMargin);
	}
}

void OptionScene::DrawContent(std::vector<std::wstring> strs, int width)
{
	// �I�����Ă���ꏊ��`��
	DrawBox(kMenuMargin * 2 + width * m_currentMenuLine, static_cast<int>(kMenuMargin),
		kMenuMargin * 2 + width * (m_currentMenuLine + 1), static_cast<int>(kMenuMargin + kMenuMarginHeight),
		0xff0000, true);
	// ���j���[�̕�����Q
	for (int i = 0; i < strs.size(); i++)
	{
		if (m_currentMenuLine == i)
		{
			DrawString(kMenuMargin * 2 + width * i, kMenuMargin, strs[i].c_str(), 0x000000);
		}
		else
		{
			DrawString(kMenuMargin * 2 + width * i, kMenuMargin, strs[i].c_str(), 0xffffff);
		}
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
		m_optionScn->ChangeScene(std::make_shared<ConfigScene>(m_mgr, m_optionScn));
		break;

		// ���ʐݒ�
	case kValume:
		m_optionScn->ChangeScene(std::make_shared<SoundOptionScene>(m_mgr));
		break;

		// ���̑��ݒ�
	default:
		assert(false);
	case kOther:
		m_optionScn->ChangeScene(std::make_shared<OtherOptionScene>(m_mgr));
		break;
	}
}
