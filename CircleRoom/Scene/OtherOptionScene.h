#pragma once
#include "Scene.h"
class OtherOptionScene : public Scene
{
public:
	OtherOptionScene(GameManager& mgr);
	~OtherOptionScene();

	void Update(Input& input);
	void Draw();

private:
	int m_frame = 0;
};

