#include "PauseScene.h"
#include <DxLib.h>
#include "Common/Input.h"
#include "Application.h"
#include "SceneManager.h"
#include "KeyConfigScene.h"

namespace
{
	constexpr int kAppeaInterval = 60;
	constexpr int kMenuMargin = 50;

	// 文字間隔
	constexpr int kMenuStringMargin = 32;

	const std::vector<std::wstring> kMenuString = { L"キー設定",
		L"音量設定",
		L"メインメニューに戻る" };
}

PauseScene::PauseScene(SceneManager& scnMgr, StageManager& stgMgr) :
	Scene(scnMgr, stgMgr),
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
	if (m_frame == 0)
	{
		m_scnMgr.PopScene();
	}
}

void PauseScene::ExpandDraw()
{
	Application& app = Application::GetInstance();
	const auto& size = app.GetWindowSize();

	int halfHeight = (size.h - 100) / 2;
	int centerY = size.h / 2;

	float rate = static_cast<float>(m_frame) / kAppeaInterval;	// 現在の時間の割合(0.0～1.0)
	int currentHalfHeight = static_cast<int>(rate * halfHeight);

	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	// ちょっと暗い矩形を描画
	DrawBox(kMenuMargin, centerY - currentHalfHeight, size.w - kMenuMargin, centerY + currentHalfHeight,
		0x888888, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(kMenuMargin, centerY - currentHalfHeight, size.w - kMenuMargin, centerY + currentHalfHeight,
		0xffffff, false);
}

void PauseScene::NormalDraw()
{
	Application& app = Application::GetInstance();
	const auto& size = app.GetWindowSize();
	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	// ちょっと暗い矩形を描画
	DrawBox(kMenuMargin, kMenuMargin, size.w - kMenuMargin, size.h - kMenuMargin,
		0x888888, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawBox(kMenuMargin, kMenuMargin, size.w - kMenuMargin, size.h - kMenuMargin,
		0xffffff, false);

	// メニュー後ろの赤線
	DrawBox(kMenuMargin * 2, static_cast<int>(kMenuMargin * 2 + kMenuStringMargin * m_currentMenuLine - kMenuStringMargin * 0.25f),
		size.w - kMenuMargin * 2, static_cast<int>(kMenuMargin * 2 + kMenuStringMargin * (m_currentMenuLine + 1) - kMenuStringMargin * 0.25f),
		0xff0000, true);
	// メニューの文字列群
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
