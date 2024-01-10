#pragma once
#include "Scene.h"

/// <summary>
/// ステージ選択シーン
/// </summary>
class StageSelectScene : public Scene
{
public:
	StageSelectScene(GameManager& mgr);
	~StageSelectScene();

	void Update(Input& input);
	void Draw();

private:
	int m_frame = 0;
};

