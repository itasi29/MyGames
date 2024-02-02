#pragma once
#include "Scene.h"

class SceneManager;
class FileBase;
class BottansFile;
class KeyFile;

class OtherOptionScene : public Scene
{
public:
	OtherOptionScene(GameManager& mgr, Input& input, std::shared_ptr<SceneManager> scn);
	~OtherOptionScene();

	void Update(Input& input);
	void Draw();

private:
	void FadeUpdate(Input& input);
	void NormalUpdate(Input& input);

	void FadeDraw();
	void NormalDraw();

	void DrawWindowMode(int index, int handle, int y, unsigned int color);

	/// <summary>
	/// �I�𓙂̕����E�摜�`��
	/// </summary>
	/// /// <param name="cmd">�R�}���h��</param>
	/// <param name="str">�E�F�[�u�����镶����</param>
	/// <param name="num">������</param>
	void DrawWave(int x, int y, const char* const cmd, const wchar_t* const str[], int num);

private:
	using UpdateFunc_t = void(OtherOptionScene::*)(Input&);
	using DrawFunc_t = void(OtherOptionScene::*)();

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	// �摜
	std::shared_ptr<FileBase> m_frame;
	std::shared_ptr<FileBase> m_rightNotationImg;
	std::shared_ptr<FileBase> m_startFrame;
	// Se
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;
	std::shared_ptr<FileBase> m_selectSe;

	// option�̃V�[��
	std::shared_ptr<SceneManager> m_optionScn;

	int m_currentLineIndex;

	int m_fadeFrame;

	// �����E�F�[�u�p�̊p�x
	float m_waveAngle;
	// �E�G�[�u�̂�`�悷�邩
	bool m_isWaveDraw;

	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;

	const Input& m_input;
};

