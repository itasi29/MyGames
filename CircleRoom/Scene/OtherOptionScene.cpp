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

#include "TitleScene.h"
#include "OneShotScene.h"

#include "OtherOptionScene.h"

namespace
{
	constexpr int kAppeaInterval = 5;
	constexpr int kMenuMargin = 120;

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
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % kGameMenu.size();
		m_frame = 0;
	}

	if (input.IsTriggered("OK"))
	{
		switch (m_currentLineIndex)
		{
		default:
			assert(false);
		case kTitle:
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
#ifdef _DEBUG
	DrawString(100, kMenuMargin + 10, L"OtherOptionScene", 0xffffff);
#endif

	// 選択している場所を描画
	DrawBox(kMenuMargin + 200, static_cast<int>(kMenuMargin + 64 + m_currentLineIndex * 32),
		kMenuMargin + 800, static_cast<int>(kMenuMargin + 64 + (m_currentLineIndex + 1) * 32),
		0xff0000, true);

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	// メニューの文字列群
	for (int i = 0; i < kGameMenu.size(); i++)
	{
		if (m_currentLineIndex == i)
		{
			int frame = (m_frame % 80) - 40;
			float rate = fabs(frame) / 40.0f;
			int alpha = 255 * rate;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawStringToHandle(kMenuMargin + 200, kMenuMargin + 64 + i * 32, kGameMenu[i].c_str(), 0x000000, fontHandle);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			DrawStringToHandle(kMenuMargin + 200, kMenuMargin + 64 + i * 32, kGameMenu[i].c_str(), 0xffffff, fontHandle);
		}
	}
}
