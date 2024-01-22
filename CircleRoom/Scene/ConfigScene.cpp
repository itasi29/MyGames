#include <DxLib.h>
#include <cassert>
#include "Input.h"

#include "GameManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"

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
	m_currentLineIndex(0),
	m_frame(0)
{
	m_soundSys = mgr.GetSound();
	m_selectSe = mgr.GetFile()->LoadSound(L"Se/select.mp3");
}

ConfigScene::~ConfigScene()
{
}

void ConfigScene::Update(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		m_soundSys->PlaySe(m_selectSe->GetHandle());
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

	m_frame++;

	if (input.IsTriggered("up"))
	{
		m_currentLineIndex = (m_currentLineIndex - 1 + kMax) % kMax;
		m_frame = 0;
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % kMax;
		m_frame = 0;
	}
}

void ConfigScene::Draw()
{
	DrawBox(128, static_cast<int>(kMenuMargin + 42 + m_currentLineIndex * 64),
		kMenuMargin + 800, static_cast<int>(kMenuMargin + 74 + m_currentLineIndex * 64),
		0xff0000, true);

	DrawName(kMenuMargin + 42, kKey, L"ƒL[Ý’è");

	DrawName(kMenuMargin + 106, kPad, L"PADÝ’è");
}

void ConfigScene::DrawName(int drawY, int index, std::wstring str)
{
	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	if (m_currentLineIndex == index)
	{
		float frame = (m_frame % 80) - 40.0f;
		float rate = fabsf(frame) / 40.0f;
		int alpha = static_cast <int>(255 * rate);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawStringToHandle(132, drawY, str.c_str(), 0x000000, fontHandle);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		DrawStringToHandle(132, drawY, str.c_str(), 0xffffff, fontHandle);
	}
}
