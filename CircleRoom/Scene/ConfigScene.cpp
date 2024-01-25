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
	// フレームの色
	constexpr unsigned int kFrameColor = 0xd2001a;
	// 通常文字列の色
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// 選択時文字列の色
	constexpr unsigned int kSelectStrColor = 0x161a30;
	// 点滅間隔
	constexpr int kFlashInterval = 40;

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
	m_fadeFrame(0)
{
	m_soundSys = mgr.GetSound();

	auto& file = m_mgr.GetFile();
	m_frame = file->LoadGraphic(L"UI/normalFrame.png", true);

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

		return;
	}

	m_fadeFrame++;

	if (input.IsTriggered("up"))
	{
		m_currentLineIndex = (m_currentLineIndex - 1 + kMax) % kMax;
		m_fadeFrame = 0;
		m_sound->PlaySe(m_cursorUpSe->GetHandle());
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % kMax;
		m_fadeFrame = 0;
		m_sound->PlaySe(m_cursorDownSe->GetHandle());
	}
}

void ConfigScene::Draw()
{
	int y = kMenuMargin + 38 + m_currentLineIndex * kMenuLineInterval;

	DrawGraph(kMenuMargin + 800, y, m_frame->GetHandle(), true);
	DrawBox(128, y,
		kMenuMargin + 800, y + 40,
		kFrameColor, true);

	y = kMenuMargin + 42;

	DrawName(y, kKey, L"キー設定");

	y += kMenuLineInterval;

	DrawName(y, kPad, L"PAD設定");
}

void ConfigScene::DrawName(int drawY, int index, std::wstring str)
{
	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	if (m_currentLineIndex == index)
	{
		int frame = (m_fadeFrame % (kFlashInterval * 2)) - kFlashInterval;
		float rate = fabsf(static_cast<float>(frame)) / kFlashInterval;
		int alpha = static_cast <int>(255 * rate);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawStringToHandle(132, drawY, str.c_str(), kSelectStrColor, fontHandle);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		DrawStringToHandle(132, drawY, str.c_str(), kWhiteColor, fontHandle);
	}
}
