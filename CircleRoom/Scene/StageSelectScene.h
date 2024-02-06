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
/// �X�e�[�W�I���V�[��
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

	// �摜�֌W
	std::shared_ptr<FileBase> m_frame;
	std::shared_ptr<FileBase> m_nowPos;
	std::shared_ptr<FileBase> m_lock;
	std::unordered_map<std::string, std::array<std::shared_ptr<FileBase>, 2>> m_stage;
	// ���֌W
	std::shared_ptr<SoundSystem> m_soundSys;
	// �I����Se
	std::shared_ptr<FileBase> m_selectSe;
};

