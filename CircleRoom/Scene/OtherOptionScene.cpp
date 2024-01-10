#include <DxLib.h>

#include "OtherOptionScene.h"


namespace
{
	constexpr int kAppeaInterval = 5;
	constexpr int kMenuMargin = 120;
}

OtherOptionScene::OtherOptionScene(GameManager& mgr) :
	Scene(mgr)
{
}

OtherOptionScene::~OtherOptionScene()
{
}

void OtherOptionScene::Update(Input& input)
{
}

void OtherOptionScene::Draw()
{
	DrawString(100, kMenuMargin + 10, L"OtherOptionScene", 0xffffff);
}
