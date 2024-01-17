#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/FontSystem.h"
#include "Input.h"

#include "OptionScene.h"

#include "SoundOptionScene.h"


namespace
{
	constexpr int kAppeaInterval = 5;
	constexpr int kMenuMargin = 120;

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
#ifdef _DEBUG
	DrawString(100, kMenuMargin + 10, L"SoundOptionScene", 0xffffff);
#endif
	
	// ‘I‘ğ‚µ‚Ä‚¢‚éêŠ‚ğ•`‰æ
	if (!m_isEdit || static_cast<int>(m_frame * 0.05f) % 2)
	{
		DrawBox(128, static_cast<int>(kMenuMargin + 42 + m_currentLineIndex * 64),
			kMenuMargin + 800, static_cast<int>(kMenuMargin + 64 + m_currentLineIndex * 64),
			0xff0000, true);
	}
	else
	{
		DrawBox(128, static_cast<int>(kMenuMargin + 42 + m_currentLineIndex * 64),
			kMenuMargin + 800, static_cast<int>(kMenuMargin + 64 + m_currentLineIndex * 64),
			0xff8800, true);
	}

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	auto rate = m_mgr.GetSound()->GetBgmVolRate();
	DrawName(kMenuMargin + 42, kBgm, L"BGM");
	DrawFormatStringToHandle(200, kMenuMargin + 42, 0xffffff, fontHandle, L"%3d“", static_cast<int>(rate * 100));
	DrawGauge(500, kMenuMargin + 42, rate);

	rate = m_mgr.GetSound()->GetSeVolRate();
	DrawName(kMenuMargin + 106, kSe, L"SE");
	DrawFormatStringToHandle(200, kMenuMargin + 106, 0xffffff, fontHandle, L"%3d“", static_cast<int>(rate * 100));
	DrawGauge(500, kMenuMargin + 106, rate);
}

void SoundOptionScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		m_isEdit = true;
		m_frame = 0;
		std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
		optionScene->InverseIsEdit();

		m_updateFunc = &SoundOptionScene::EditUpdate;
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
		DrawStringToHandle(132, drawY, str.c_str(), 0x000000, fontHandle);
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
