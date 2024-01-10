#include <DxLib.h>

#include "StageSelectScene.h"

namespace
{
	constexpr int kAppeaInterval = 5;
	constexpr int kMenuMargin = 120;
}

StageSelectScene::StageSelectScene(GameManager& mgr) :
	Scene(mgr)
{
}

StageSelectScene::~StageSelectScene()
{
}

void StageSelectScene::Update(Input& input)
{
}

void StageSelectScene::Draw()
{
	DrawString(100, kMenuMargin + 10, L"StagSelectScene", 0xffffff);
}
