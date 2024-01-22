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
	// OptionSceneが持っているScene
	std::shared_ptr<SceneManager> m_optionScn;
	int m_currentLineIndex;

	int m_frame;

	// 音関係
	std::shared_ptr<SoundSystem> m_soundSys;
	// 選択時Se
	std::shared_ptr<FileBase> m_selectSe;
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;
};

