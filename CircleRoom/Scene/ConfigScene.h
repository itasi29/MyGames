#pragma once
#include <memory>
#include "Scene.h"

class FileBase;
class BottansFile;
class KeyFile;
class SoundSystem;

class ConfigScene : public Scene
{
public:
	ConfigScene(GameManager& mgr, Input& input, std::shared_ptr<SceneManager> scene);
	~ConfigScene();

	void Update(Input& input);
	void Draw() const;

private:
	void DrawName(int drawY, int index, std::wstring str) const;

	/// <summary>
	/// �I�𓙂̕����E�摜�`��
	/// </summary>
	/// /// <param name="cmd">�R�}���h��</param>
	/// <param name="str">�E�F�[�u�����镶����</param>
	/// <param name="num">������</param>
	void DrawWave(int x, int y, const char* const cmd, const wchar_t* const str[], int num) const;

private:
	// OptionScene�������Ă���Scene
	std::shared_ptr<SceneManager> m_optionScn;

	int m_currentLineIndex;

	int m_fadeFrame;

	// �摜�֌W
	std::shared_ptr<FileBase> m_frame;
	// ���֌W
	std::shared_ptr<SoundSystem> m_soundSys;
	std::shared_ptr<FileBase> m_startFrame;
	// �I����Se
	std::shared_ptr<FileBase> m_selectSe;
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;

	// �����E�F�[�u�p�̊p�x
	float m_waveAngle;
	// �E�G�[�u�̂�`�悷�邩
	bool m_isWaveDraw;

	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;

	const Input& m_input;
};

