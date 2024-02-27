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
	void Draw() const;

private:
	void DrawInf(const std::string& st) const;

	/// <summary>
	/// �I�𓙂̕����E�摜�`��
	/// </summary>
	/// /// <param name="cmd">�R�}���h��</param>
	/// <param name="str">�E�F�[�u�����镶����</param>
	/// <param name="num">������</param>
	void DrawWave(int x, int y, const char* const cmd, const wchar_t* const str[], int num) const;

	void CurrosrPos();

private:
	int m_indexRow;
	int m_indexLine;

	int m_fadeFrame;

	std::unordered_map<std::string, std::shared_ptr<StageBase>> m_stageData;
	// ���}�l�[�W���[
	std::shared_ptr<SoundSystem> m_soundSys;

	// �摜�֌W
	std::shared_ptr<FileBase> m_frame;
	std::shared_ptr<FileBase> m_nowPos;
	std::shared_ptr<FileBase> m_lock;
	std::shared_ptr<FileBase> m_startFrame;
	std::unordered_map<std::string, std::array<std::shared_ptr<FileBase>, 2>> m_stage;
	// �I����Se
	std::shared_ptr<FileBase> m_selectSe;

	// �����E�F�[�u�p�̊p�x
	float m_waveAngle;

	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;

	const Input& m_input;
};

