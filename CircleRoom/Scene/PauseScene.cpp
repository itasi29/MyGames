#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "Common/Input.h"

#include "PauseScene.h"

#include "GameManager.h"
#include "KeyConfigScene.h"
#include "TitleScene.h"

namespace
{
	constexpr int kAppeaInterval = 60;
	constexpr int kMenuMargin = 50;

	// �����Ԋu
	constexpr int kMenuStringMargin = 32;

	const std::vector<std::wstring> kMenuString = { L"�L�[�ݒ�",
		L"���ʐݒ�",
		L"���C�����j���[�ɖ߂�" };

	enum
	{
		kKey,
		kValume,
		kMainMenu
	};
}

PauseScene::PauseScene(GameManager& mgr) :
	Scene(mgr),
	m_title(false),
	m_currentMenuLine(0)
{
	m_updateFunc = &PauseScene::AppearUpdate;
	m_drawFunc = &PauseScene::ExpandDraw;
}

void PauseScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void PauseScene::Draw()
{
	(this->*m_drawFunc)();
}

void PauseScene::AppearUpdate(Input&)
{
	m_frame++;
	if (kAppeaInterval <= m_frame)
	{
		m_updateFunc = &PauseScene::NormalUpdate;
		m_drawFunc = &PauseScene::NormalDraw;
	}
}

void PauseScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("pause"))
	{
		m_updateFunc = &PauseScene::DisappearUpdate;
		m_drawFunc = &PauseScene::ExpandDraw;

		return;
	}

	if (input.IsTriggered("OK"))
	{
		switch (m_currentMenuLine)
		{
			// �L�[�ݒ�
		case kKey:
			m_mgr.GetScene().PushScene(std::make_shared<KeyConfigScene>(m_mgr, input));
			break;

			// ���ʐݒ�
		case kValume:
			break;

			// ���j���[�ɖ߂�
		case kMainMenu:
			m_updateFunc = &PauseScene::DisappearUpdate;
			m_drawFunc = &PauseScene::FadeDraw;
			m_title = true;

			return;
			break;

		default:
			assert(false);
			break;
		}
	}

	if (input.IsTriggered("up"))
	{
		m_currentMenuLine = (m_currentMenuLine - 1 + static_cast<int>(kMenuString.size())) % static_cast<int>(kMenuString.size());
	}
	if (input.IsTriggered("down"))
	{
		m_currentMenuLine = (m_currentMenuLine + 1) % kMenuString.size();
	}
}

void PauseScene::DisappearUpdate(Input&)
{
	m_frame--;
	if (m_frame > 0) return;

	// �^�C�g���V�[��
	if (m_title)
	{
		m_mgr.GetScene().MoveScene(std::make_shared<TitleScene>(m_mgr));
		return;
	}
	// �v���C�V�[��
	else
	{
		m_mgr.GetScene().PopScene();
		return;	
	}

	
}

void PauseScene::ExpandDraw()
{
	Application& app = Application::GetInstance();
	const auto& m_size = app.GetWindowSize();

	int halfHeight = (m_size.h - 100) / 2;
	int centerY = m_size.h / 2;

	float rate = static_cast<float>(m_frame) / kAppeaInterval;	// ���݂̎��Ԃ̊���(0.0�`1.0)
	int currentHalfHeight = static_cast<int>(rate * halfHeight);

	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	// ������ƈÂ���`��`��
	DrawBox(kMenuMargin, centerY - currentHalfHeight, m_size.w - kMenuMargin, centerY + currentHalfHeight,
		0x888888, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(kMenuMargin, centerY - currentHalfHeight, m_size.w - kMenuMargin, centerY + currentHalfHeight,
		0xffffff, false);
}

void PauseScene::FadeDraw()
{
	NormalDraw();
	Application& app = Application::GetInstance();
	const auto& m_size = app.GetWindowSize();

	float rate = 1.0f - static_cast<float>(m_frame) / kAppeaInterval;	// ���݂̎��Ԃ̊���(0.0�`1.0)
	SetDrawBlendMode(DX_BLENDMODE_MULA, static_cast<int>(255 * rate));
	DrawBox(0, 0, m_size.w, m_size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void PauseScene::NormalDraw()
{
	Application& app = Application::GetInstance();
	const auto& m_size = app.GetWindowSize();
	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	// ������ƈÂ���`��`��
	DrawBox(kMenuMargin, kMenuMargin, m_size.w - kMenuMargin, m_size.h - kMenuMargin,
		0x888888, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawBox(kMenuMargin, kMenuMargin, m_size.w - kMenuMargin, m_size.h - kMenuMargin,
		0xffffff, false);

	// ���j���[���̐Ԑ�
	DrawBox(kMenuMargin * 2, static_cast<int>(kMenuMargin * 2 + kMenuStringMargin * m_currentMenuLine - kMenuStringMargin * 0.25f),
		m_size.w - kMenuMargin * 2, static_cast<int>(kMenuMargin * 2 + kMenuStringMargin * (m_currentMenuLine + 1) - kMenuStringMargin * 0.25f),
		0xff0000, true);
	// ���j���[�̕�����Q
	for (int i = 0; i < kMenuString.size(); i++)
	{
		if (m_currentMenuLine == i)
		{
			DrawString(kMenuMargin * 2, kMenuMargin * 2 + kMenuStringMargin * i, kMenuString[i].c_str(), 0x000000);
		}
		else
		{
			DrawString(kMenuMargin * 2, kMenuMargin * 2 + kMenuStringMargin * i, kMenuString[i].c_str(), 0xffffff);
		}
	}
}
