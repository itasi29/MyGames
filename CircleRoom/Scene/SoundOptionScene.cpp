#include <DxLib.h>
#include "Application.h"
#include "GameManager.h"
#include "Input.h"

#include "SoundOptionScene.h"


namespace
{
	constexpr int kAppeaInterval = 5;
	constexpr int kMenuMargin = 120;

	// ÉQÅ[ÉWÇÃí∑Ç≥
	constexpr int kGaugeLength = 100;
}

SoundOptionScene::SoundOptionScene(GameManager& mgr) :
	Scene(mgr)
{
}

SoundOptionScene::~SoundOptionScene()
{
}

void SoundOptionScene::Update(Input& input)
{
	if (input.IsTriggered("up"))
	{
		m_mgr.GetSound().ChangeBgmVol(10);
	}
	if (input.IsTriggered("down"))
	{
		m_mgr.GetSound().ChangeBgmVol(-10);
	}
}

void SoundOptionScene::Draw()
{
	DrawString(100, kMenuMargin + 10, L"SoundOptionScene", 0xffffff);

	DrawString(132, kMenuMargin + 42, L"BGM", 0xffffff);
	DrawGauge(500, kMenuMargin + 42, 1.0f);

	DrawString(132, kMenuMargin + 106, L"BGM", 0xffffff);
	DrawGauge(500, kMenuMargin + 106, 0.5f);
}

void SoundOptionScene::DrawGauge(int drawX, int drawY, float rate)
{
	// â∫ínï`âÊ
	DrawBox(drawX, drawY, drawX + kGaugeLength, drawY + 32, 0xffffff, true);

	// ÉQÅ[ÉWäÑçáï`âÊ
	DrawBox(drawX, drawY, drawX + kGaugeLength * rate, drawY + 32, 0xffff00, true);
}
