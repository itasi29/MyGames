#pragma once
#include "Scene.h"
#include <array>

class SceneManager;
class FileBase;
class BottansFile;
class KeyFile;

class ExplanationScene : public Scene
{
public:
	ExplanationScene(GameManager& mgr, Input& input, std::shared_ptr<SceneManager> scn);
	~ExplanationScene();

	void Update(Input& input);
	void Draw();

private:
	void SelectUpdate(Input& input);
	void OperationUpdate(Input&);
	void ProceedUpdate(Input&);
	void DashUpdate(Input&);
	void BossUpdate(Input&);

	/// <summary>
	/// �I�𓙂̕����E�摜�`��
	/// </summary>
	/// /// <param name="cmd">�R�}���h��</param>
	/// <param name="str">�E�F�[�u�����镶����</param>
	/// <param name="num">������</param>
	void DrawWave(int x, int y, const char* const cmd, const wchar_t* const str[], int num);

private:
	using UpdateFunc_t = void(ExplanationScene::*)(Input&);

	UpdateFunc_t m_updateFunc;

	// �摜
	std::shared_ptr<FileBase> m_frame;
	std::shared_ptr<FileBase> m_startFrame;
	std::array<std::shared_ptr<FileBase>, 2> m_operator;
	std::array<std::shared_ptr<FileBase>, 3> m_proceed;
	std::shared_ptr<FileBase> m_dash;
	std::shared_ptr<FileBase> m_boss;
	// SE
	std::shared_ptr<FileBase> m_selectSe;

	// option�̃V�[��
	std::shared_ptr<SceneManager> m_optionScn;

	int m_currentLineIndex;

	int m_fadeFrame;

	int m_index;

	// �����E�F�[�u�p�̊p�x
	float m_waveAngle;
	// �E�G�[�u�̂�`�悷�邩
	bool m_isWaveDraw;

	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;

	const Input& m_input;
};

