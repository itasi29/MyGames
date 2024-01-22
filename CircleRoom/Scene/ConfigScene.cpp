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

	constexpr int kMenuLineInterval = 128;

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

	auto& file = m_mgr.GetFile();
	m_selectSe = file->LoadSound(L"Se/select.mp3", true);
	m_cursorUpSe = file->LoadSound(L"Se/cursorUp.mp3", true);
	m_cursorDownSe = file->LoadSound(L"Se/cursorDown.mp3", true);
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
		m_sound->PlaySe(m_cursorUpSe->GetHandle());
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % kMax;
		m_frame = 0;
		m_sound->PlaySe(m_cursorDownSe->GetHandle());
	}
}

void ConfigScene::Draw()
{
	int y = kMenuMargin + 38 + m_currentLineIndex * kMenuLineInterval;

	DrawBox(128, y,
		kMenuMargin + 800, y + 40,
		0xff0000, true);

	y = kMenuMargin + 42;

	DrawName(y, kKey, L"ƒL[Ý’è");

	y += kMenuLineInterval;

	DrawName(y, kPad, L"PADÝ’è");
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
