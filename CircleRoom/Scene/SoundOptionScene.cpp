#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "GameManager.h"
#include "Input.h"

#include "OptionScene.h"

#include "SoundOptionScene.h"


namespace
{
	constexpr int kAppeaInterval = 5;
	constexpr int kMenuMargin = 120;

	// ÉQÅ[ÉWÇÃí∑Ç≥
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
}

SoundOptionScene::~SoundOptionScene()
{
}

void SoundOptionScene::Update(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		m_isEdit = !m_isEdit;
		std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene().GetTopScene());
		optionScene->InverseIsEdit();

		m_frame = 0;
	}

	if (m_isEdit)
	{
		if (input.IsTriggered("right"))
		{
			switch (m_currentLineIndex)
			{
			default:
				assert(false);
			case kBgm:
				m_mgr.GetSound().ChangeBgmVol(10);
				break;

			case kSe:
				m_mgr.GetSound().ChangeSeVol(10);
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
				m_mgr.GetSound().ChangeBgmVol(-10);
				break;

			case kSe:
				m_mgr.GetSound().ChangeSeVol(-10);
				break;
			}
		}

		m_frame++;
	}
	else
	{
		if (input.IsTriggered("up"))
		{
			m_currentLineIndex = (m_currentLineIndex - 1 + kMax) % kMax;
		}
		if (input.IsTriggered("down"))
		{
			m_currentLineIndex = (m_currentLineIndex + 1) % kMax;
		}
	}
}

void SoundOptionScene::Draw()
{
	DrawString(100, kMenuMargin + 10, L"SoundOptionScene", 0xffffff);
	
	// ëIëÇµÇƒÇ¢ÇÈèÍèäÇï`âÊ
	if (m_isEdit)
	{
		if (static_cast<int>(m_frame * 0.05f) % 2)
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
	}
	else
	{
		DrawBox(128, static_cast<int>(kMenuMargin + 42 + m_currentLineIndex * 64),
			kMenuMargin + 800, static_cast<int>(kMenuMargin + 64 + m_currentLineIndex * 64),
			0xff0000, true);
	}

	auto rate = m_mgr.GetSound().GetBgmVolRate();
	DrawName(kMenuMargin + 42, kBgm, L"BGM");
	DrawFormatString(200, kMenuMargin + 42, 0xffffff, L"%3dÅì", static_cast<int>(rate * 100));
	DrawGauge(500, kMenuMargin + 42, rate);

	rate = m_mgr.GetSound().GetSeVolRate();
	DrawName(kMenuMargin + 106, kSe, L"SE");
	DrawFormatString(200, kMenuMargin + 106, 0xffffff, L"%3dÅì", static_cast<int>(rate * 100));
	DrawGauge(500, kMenuMargin + 106, rate);
}

void SoundOptionScene::DrawName(int drawY, int index, std::wstring str)
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

void SoundOptionScene::DrawGauge(int drawX, int drawY, float rate)
{
	// â∫ínï`âÊ
	DrawBox(drawX, drawY, drawX + kGaugeLength, drawY + 32, 0xffffff, true);

	// ÉQÅ[ÉWäÑçáï`âÊ
	DrawBox(drawX, drawY, drawX + kGaugeLength * rate, drawY + 32, 0xffff00, true);
}
