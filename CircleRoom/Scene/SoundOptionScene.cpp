#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/FontSystem.h"
#include "Input.h"

#include "OptionScene.h"

#include "SoundOptionScene.h"


namespace
{
	constexpr int kAppeaInterval = 5;
	constexpr int kMenuMargin = 120;

	constexpr int kMenuLineInterval = 128;

	// ƒQ[ƒW‚Ì’·‚³
	constexpr int kGaugeLength = 100;

	enum
	{
		kBgm,
		kSe,
		kMax
	};
}

SoundOptionScene::SoundOptionScene(GameManager& mgr) :
	Scene(mgr),
	m_currentLineIndex(0),
	m_isEdit(false),
	m_frame(0)
{
	m_updateFunc = &SoundOptionScene::NormalUpdate;

	m_soundSys = mgr.GetSound();
	auto& file = m_mgr.GetFile();
	m_selectSe = file->LoadSound(L"Se/select.mp3", true);
	m_cursorUpSe = file->LoadSound(L"Se/cursorUp.mp3", true);
	m_cursorDownSe = file->LoadSound(L"Se/cursorDown.mp3", true);
}

SoundOptionScene::~SoundOptionScene()
{
}

void SoundOptionScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void SoundOptionScene::Draw()
{	
	int y = kMenuMargin + 38 + m_currentLineIndex * kMenuLineInterval;

	// ‘I‘ğ‚µ‚Ä‚¢‚éêŠ‚ğ•`‰æ
	if (!m_isEdit || static_cast<int>(m_frame * 0.05f) % 2)
	{
		DrawBox(128, y,
			kMenuMargin + 800, y + 40,
			0xff0000, true);
	}
	else
	{
		DrawBox(128, y,
			kMenuMargin + 800, y + 40,
			0xff8800, true);
	}

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	y = kMenuMargin + 42;
	auto rate = m_mgr.GetSound()->GetBgmVolRate();
	DrawName(y, kBgm, L"BGM");
	DrawFormatStringToHandle(200, y, 0xffffff, fontHandle, L"%3d“", static_cast<int>(rate * 100));
	DrawGauge(500, y, rate);

	y += kMenuLineInterval;
	rate = m_mgr.GetSound()->GetSeVolRate();
	DrawName(y, kSe, L"SE");
	DrawFormatStringToHandle(200, y, 0xffffff, fontHandle, L"%3d“", static_cast<int>(rate * 100));
	DrawGauge(500, y, rate);
}

void SoundOptionScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		m_soundSys->PlaySe(m_selectSe->GetHandle());
		m_isEdit = true;
		m_frame = 0;
		std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
		optionScene->InverseIsEdit();

		m_updateFunc = &SoundOptionScene::EditUpdate;
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

void SoundOptionScene::EditUpdate(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		m_isEdit = false;
		std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
		optionScene->InverseIsEdit();

		m_updateFunc = &SoundOptionScene::NormalUpdate;
	}

	if (input.IsTriggered("right"))
	{
		switch (m_currentLineIndex)
		{
		default:
			assert(false);
		case kBgm:
			m_mgr.GetSound()->ChangeBgmVol(10);
			break;

		case kSe:
			m_mgr.GetSound()->ChangeSeVol(10);
			break;
		}
	}
	if (input.IsTriggered("left"))
	{
		switch (m_currentLineIndex)
		{
		default:
			assert(false);
		case kBgm:
			m_mgr.GetSound()->ChangeBgmVol(-10);
			break;

		case kSe:
			m_mgr.GetSound()->ChangeSeVol(-10);
			break;
		}
	}

	m_frame++;
}

void SoundOptionScene::DrawName(int drawY, int index, std::wstring str)
{
	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	if (m_currentLineIndex == index)
	{
		if (!m_isEdit)
		{
			int frame = (m_frame % 80) - 40;
			float rate = fabsf(static_cast<float>(frame)) / 40.0f;
			int alpha = static_cast <int>(255 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		}
		DrawStringToHandle(132, drawY, str.c_str(), 0x000000, fontHandle);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		DrawStringToHandle(132, drawY, str.c_str(), 0xffffff, fontHandle);
	}
}

void SoundOptionScene::DrawGauge(int drawX, int drawY, float rate)
{
	// ‰º’n•`‰æ
	DrawBox(drawX, drawY, drawX + kGaugeLength, drawY + 32, 0xffffff, true);

	// ƒQ[ƒWŠ„‡•`‰æ
	DrawBox(drawX, drawY, drawX + static_cast<int>(kGaugeLength * rate), drawY + 32, 0xffff00, true);
}
