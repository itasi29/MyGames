#pragma once
#ifdef _DEBUG

// class GameManager;

#include "Scene.h"
class DebugScene : public Scene
{
public:
	DebugScene(GameManager& mgr);
	~DebugScene();

	virtual void Update(Input& input);
	virtual void Draw();

private:
	int m_line;
};
#endif

