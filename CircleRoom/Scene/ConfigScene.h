#pragma once
#include <memory>

#include "Scene.h"
class ConfigScene : public Scene
{
public:
	ConfigScene(GameManager& mgr, std::shared_ptr<SceneManager> scene);
	~ConfigScene();

	void Update(Input& input);
	void Draw();

private:
	void DrawName(int drawY, int index, std::wstring str);

private:
	// OptionScene‚ª‚Á‚Ä‚¢‚éScene
	std::shared_ptr<SceneManager> m_optionScn;
	int m_currentLineIndex;
};

