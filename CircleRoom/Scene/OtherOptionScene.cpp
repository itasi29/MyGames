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
	m_frame(0)
{
	// FIXME:仮画像
	m_rightNotationImg = mgr.GetFile()->LoadGraphic(L"UI/16-9.png");

	auto& file = m_mgr.GetFile();
	m_cursorUpSe = file->LoadSound(L"Se/cursorUp.mp3", true);
	m_cursorDownSe = file->LoadSound(L"Se/cursorDown.mp3", true);
}

OtherOptionScene::~OtherOptionScene()
{
}

void OtherOptionScene::Update(Input& input)
{
	m_frame++;

	if (input.IsTriggered("up"))
	{
		m_currentLineIndex = (m_currentLineIndex - 1 + static_cast<int>(kGameMenu.size())) % static_cast<int>(kGameMenu.size());
		m_frame = 0;
		m_sound->PlaySe(m_cursorUpSe->GetHandle());
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % kGameMenu.size();
		m_frame = 0;
		m_sound->PlaySe(m_cursorDownSe->GetHandle());
	}

	if (input.IsTriggered("OK"))
	{
		switch (m_currentLineIndex)
		{
		default:
			assert(false);
		case kTitle:
			m_sound->Stop();
			m_mgr.GetScene()->ChangeSceneWithClear(std::make_shared<TitleScene>(m_mgr));
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

void OtherOptionScene::Draw()
{
	// 選択している場所を描画
	int y = kMenuMargin + 60 + m_currentLineIndex * kMenuLineInterval;

	DrawBox(kMenuMargin + 200, y,
		kMenuMargin + 800, y + 40,
		0xff0000, true);

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	y = kMenuMargin + 64;

	// メニューの文字列群
	for (int i = 0; i < kGameMenu.size(); i++)
	{
		if (m_currentLineIndex == i)
		{
			int frame = (m_frame % 80) - 40;
			float rate = fabsf(static_cast<float>(frame)) / 40.0f;
			int alpha = static_cast <int>(255 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawStringToHandle(kMenuMargin + 200, y, kGameMenu[i].c_str(), 0x000000, fontHandle);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			DrawStringToHandle(kMenuMargin + 200, y, kGameMenu[i].c_str(), 0xffffff, fontHandle);
		}

		y += kMenuLineInterval;
	}
}
