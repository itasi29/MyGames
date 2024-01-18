#pragma once
#include <memory>

class FileBase;
class SoundSystem;

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

	int m_frame;

	// ‰¹ŠÖŒW
	std::shared_ptr<SoundSystem> m_soundSys;
	// ‘I‘ğSe
	std::shared_ptr<FileBase> m_selectSe;
};

