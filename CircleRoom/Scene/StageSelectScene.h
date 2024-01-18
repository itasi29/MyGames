#pragma once
#include <vector>
#include "Scene.h"

class FileBase;
class SoundSystem;

/// <summary>
/// �X�e�[�W�I���V�[��
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

	int m_frame = 0;

	// ���֌W
	std::shared_ptr<SoundSystem> m_soundSys;
	// �I����Se
	std::shared_ptr<FileBase> m_selectSe;
};

