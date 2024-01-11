#include <DxLib.h>
#include <cassert>
#include <vector>
#include <string>
#include <memory>

#include "Application.h"
#include "Input.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"

#include "TitleScene.h"

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

	const std::vector<std::wstring> kMenuString = { L"タイトルへ",
		L"権利表記",
		L"終了"
	};

}

OtherOptionScene::OtherOptionScene(GameManager& mgr) :
	Scene(mgr),
	m_currentLineIndex(0)
{
}

OtherOptionScene::~OtherOptionScene()
{
}

void OtherOptionScene::Update(Input& input)
{
	if (input.IsTriggered("up"))
	{
		m_currentLineIndex = (m_currentLineIndex - 1 + static_cast<int>(kMenuString.size())) % static_cast<int>(kMenuString.size());
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % kMenuString.size();
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
	DrawString(100, kMenuMargin + 10, L"OtherOptionScene", 0xffffff);

	// 選択している場所を描画
	DrawBox(kMenuMargin + 200, static_cast<int>(kMenuMargin + 64 + m_currentLineIndex * 32),
		kMenuMargin + 800, static_cast<int>(kMenuMargin + 64 + (m_currentLineIndex + 1) * 32),
		0xff0000, true);

	// メニューの文字列群
	for (int i = 0; i < kMenuString.size(); i++)
	{
		if (m_currentLineIndex == i)
		{
			DrawString(kMenuMargin + 200, kMenuMargin + 64 + i * 32, kMenuString[i].c_str(), 0x000000);
		}
		else
		{
			DrawString(kMenuMargin + 200, kMenuMargin + 64 + i * 32, kMenuString[i].c_str(), 0xffffff);
		}
	}
}
