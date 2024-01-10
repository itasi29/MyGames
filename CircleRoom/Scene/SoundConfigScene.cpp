#include <DxLib.h>
#include "Application.h"
#include "GameManager.h"

#include "SoundConfigScene.h"


namespace
{
	constexpr int kAppeaInterval = 60;
	constexpr int kMenuMargin = 60 * 2;

	// ÉQÅ[ÉWÇÃí∑Ç≥
	constexpr int kGaugeLength = 100;
}

SoundConfigScene::SoundConfigScene(GameManager& mgr) :
	Scene(mgr)
{
}

SoundConfigScene::~SoundConfigScene()
{
}

void SoundConfigScene::Update(Input& input)
{
	
}

void SoundConfigScene::Draw()
{
	DrawString(100, kMenuMargin + 10, L"SoundConfig Scene", 0xffffff);

	DrawString(132, kMenuMargin + 42, L"BGM", 0xffffff);
	DrawGauge(500, kMenuMargin + 42, 1.0f);

	DrawString(132, kMenuMargin + 106, L"BGM", 0xffffff);
	DrawGauge(500, kMenuMargin + 106, 0.5f);
}

void SoundConfigScene::DrawGauge(int drawX, int drawY, float rate)
{
	// â∫ínï`âÊ
	DrawBox(drawX, drawY, drawX + kGaugeLength, drawY + 32, 0xffffff, true);

	// ÉQÅ[ÉWäÑçáï`âÊ
	DrawBox(drawX, drawY, drawX + kGaugeLength * rate, drawY + 32, 0xffff00, true);
}
