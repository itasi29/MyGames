#include <DxLib.h>
#include <cassert>
#include <vector>
#include <string>
#include <memory>

#include "Application.h"
#include "Input.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/SoundSystem.h"

#include "TitleScene.h"
#include "OneShotScene.h"

#include "OtherOptionScene.h"

namespace
{
	// フレームの色
	constexpr unsigned int kFrameColor = 0xd2001a;
	// 通常文字列の色
	constexpr unsigned int kStrColor = 0xf0ece5;
	// 選択時文字列の色
	constexpr unsigned int kSelectStrColor = 0x161a30;
	// 点滅間隔
	constexpr int kFlashInterval = 40;

	// フェードフレーム
	constexpr int kFadeFrame = 60;

	constexpr int kAppeaInterval = 5;
	constexpr int kMenuMargin = 120;

	constexpr int kMenuLineInterval = 128;

	enum
	{
		kTitle,
		kRightsNotation,
		kEnd
	};

	const std::vector<std::wstring> kGameMenu = { L"タイトルへ",
		L"権利表記",
		L"終了"
	};

}

OtherOptionScene::OtherOptionScene(GameManager& mgr) :
	Scene(mgr),
	m_currentLineIndex(0),
	m_fadeFrame(0)
{
	// FIXME:仮画像

	auto& file = m_mgr.GetFile();
	m_rightNotationImg = file->LoadGraphic(L"UI/16-9.png");
	m_frame = file->LoadGraphic(L"UI/normalFrame.png", true);

	m_cursorUpSe = file->LoadSound(L"Se/cursorUp.mp3", true);
	m_cursorDownSe = file->LoadSound(L"Se/cursorDown.mp3", true);

	m_updateFunc = &OtherOptionScene::NormalUpdate;
	m_drawFunc = &OtherOptionScene::NormalDraw;
}

OtherOptionScene::~OtherOptionScene()
{
}

void OtherOptionScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void OtherOptionScene::Draw()
{
	(this->*m_drawFunc)();
}

void OtherOptionScene::FadeUpdate(Input& input)
{
	m_fadeFrame++;
	float rate = (1.0f - m_fadeFrame / static_cast<float>(kFadeFrame));
	m_sound->PlayFadeBgm(-1, rate);

	if (m_fadeFrame > kFadeFrame)
	{
		m_mgr.GetScene()->ChangeSceneWithClear(std::make_shared<TitleScene>(m_mgr));
		return;
	}
}

void OtherOptionScene::NormalUpdate(Input& input)
{
	m_fadeFrame++;

	if (input.IsTriggered("up"))
	{
		m_currentLineIndex = (m_currentLineIndex - 1 + static_cast<int>(kGameMenu.size())) % static_cast<int>(kGameMenu.size());
		m_fadeFrame = 0;
		m_sound->PlaySe(m_cursorUpSe->GetHandle());
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % kGameMenu.size();
		m_fadeFrame = 0;
		m_sound->PlaySe(m_cursorDownSe->GetHandle());
	}

	if (input.IsTriggered("OK"))
	{
		switch (m_currentLineIndex)
		{
		default:
			assert(false);
		case kTitle:
			m_fadeFrame = 0;
			m_updateFunc = &OtherOptionScene::FadeUpdate;
			m_drawFunc = &OtherOptionScene::FadeDraw;
			break;

		case kRightsNotation:
			m_mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(m_mgr, m_rightNotationImg->GetHandle()));
			break;

		case kEnd:
			Application& app = Application::GetInstance();
			app.End();
			break;
		}
	}
}

void OtherOptionScene::FadeDraw()
{
	NormalDraw();

	const auto& size = Application::GetInstance().GetWindowSize();

	float rate = m_fadeFrame / static_cast<float>(kFadeFrame);
	int alpha = 255 * rate;
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, size.w, size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void OtherOptionScene::NormalDraw()
{
	// 選択している場所を描画
	int y = kMenuMargin + 60 + m_currentLineIndex * kMenuLineInterval;

	DrawGraph(kMenuMargin + 800, y, m_frame->GetHandle(), true);
	DrawBox(kMenuMargin + 200, y,
		kMenuMargin + 800, y + 40,
		kFrameColor, true);

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	y = kMenuMargin + 64;

	// メニューの文字列群
	for (int i = 0; i < kGameMenu.size(); i++)
	{
		if (m_currentLineIndex == i)
		{
			int frame = (m_fadeFrame % (kFlashInterval * 2)) - kFlashInterval;
			float rate = fabsf(static_cast<float>(frame)) / kFlashInterval;
			int alpha = static_cast <int>(255 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawStringToHandle(kMenuMargin + 200, y, kGameMenu[i].c_str(), kSelectStrColor, fontHandle);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			DrawStringToHandle(kMenuMargin + 200, y, kGameMenu[i].c_str(), kStrColor, fontHandle);
		}

		y += kMenuLineInterval;
	}
}
