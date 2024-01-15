#include <DxLib.h>
#include <cassert>
#include "Input.h"

#include "SceneManager.h"
#include "KeyConfigScene.h"
#include "PadConfigScene.h"

#include "ConfigScene.h"

namespace
{
	constexpr int kMenuMargin = 120;

	enum
	{
		kKey,
		kPad,
		kMax
	};
}

ConfigScene::ConfigScene(GameManager& mgr, std::shared_ptr<SceneManager> scene) :
	Scene(mgr),
	m_optionScn(scene),
	m_currentLineIndex(0)
{
}

ConfigScene::~ConfigScene()
{
}

void ConfigScene::Update(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		switch (m_currentLineIndex)
		{
		default:
			assert(false);
		case kKey:
			m_optionScn->ChangeScene(std::make_shared<KeyConfigScene>(m_mgr, input, m_optionScn));
			break;

		case kPad:
			m_optionScn->ChangeScene(std::make_shared<PadConfigScene>(m_mgr, input, m_optionScn));
			break;
		}
	}

	if (input.IsTriggered("up"))
	{
		m_currentLineIndex = (m_currentLineIndex - 1 + kMax) % kMax;
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % kMax;
	}
}

void ConfigScene::Draw()
{
	DrawBox(128, static_cast<int>(kMenuMargin + 42 + m_currentLineIndex * 64),
		kMenuMargin + 800, static_cast<int>(kMenuMargin + 64 + m_currentLineIndex * 64),
		0xff0000, true);

	DrawName(kMenuMargin + 42, kKey, L"ÉLÅ[ê›íË");

	DrawName(kMenuMargin + 106, kPad, L"PADê›íË");
}

void ConfigScene::DrawName(int drawY, int index, std::wstring str)
{
	if (m_currentLineIndex == index)
	{
		DrawString(132, drawY, str.c_str(), 0x000000);
	}
	else
	{
		DrawString(132, drawY, str.c_str(), 0xffffff);
	}
}
