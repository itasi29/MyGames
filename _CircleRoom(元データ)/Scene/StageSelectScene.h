#pragma once
#include <vector>
#include "Scene.h"

class FileBase;
class SoundSystem;

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
	int m_indexRow;
	int m_indexLine;

	int m_fadeFrame;

	// 画像関係
	std::shared_ptr<FileBase> m_frame;
	std::shared_ptr<FileBase> m_nowPos;
	std::shared_ptr<FileBase> m_lock;
	// 音関係
	std::shared_ptr<SoundSystem> m_soundSys;
	// 選択時Se
	std::shared_ptr<FileBase> m_selectSe;
};

