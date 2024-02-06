#pragma once
#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include "Scene.h"

class FileBase;
class StageBase;
class SoundSystem;

/// <summary>
/// ステージ選択シーン
/// </summary>
class StageSelectScene : public Scene
{
public:
	StageSelectScene(GameManager& mgr, Input& input);
	~StageSelectScene();

	void Update(Input& input);
	void Draw();

private:
	void DrawInf(const std::string& st);

	void CurrosrPos();

private:
	int m_indexRow;
	int m_indexLine;

	int m_fadeFrame;

	std::unordered_map<std::string, std::shared_ptr<StageBase>> m_stageData;

	// 画像関係
	std::shared_ptr<FileBase> m_frame;
	std::shared_ptr<FileBase> m_nowPos;
	std::shared_ptr<FileBase> m_lock;
	std::unordered_map<std::string, std::array<std::shared_ptr<FileBase>, 2>> m_stage;
	// 音関係
	std::shared_ptr<SoundSystem> m_soundSys;
	// 選択時Se
	std::shared_ptr<FileBase> m_selectSe;
};

