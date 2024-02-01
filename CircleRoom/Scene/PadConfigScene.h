#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "Scene.h"
#include "Input.h"

class KeyFile;
class BottansFile;
class SceneManager;

/// <summary>
/// PAD�R���t�B�O�̃V�[��
/// </summary>
class PadConfigScene : public Scene
{
public:
	PadConfigScene(GameManager& mgr, Input& input, std::shared_ptr<SceneManager> scn);
	~PadConfigScene();

	void Update(Input& input);
	void Draw();

private:
	void NormalUpdate(Input& input);
	void EditUpdate(Input& input);
	void EditEndUpdate(Input& input);

	void DrawCommandList();

	/// <summary>
	/// �I�𓙂̕����E�摜�`��
	/// </summary>
	/// /// <param name="cmd">�R�}���h��</param>
	/// <param name="str">�E�F�[�u�����镶����</param>
	/// <param name="num">������</param>
	void DrawWave(const char* const cmd, const wchar_t* const str[], int num);

	std::wstring GetPadName(int padstate);

private:
	using updateFunc_t = void (PadConfigScene::*)(Input&);
	updateFunc_t m_updateFunc;

	// option�̃V�[��
	std::shared_ptr<SceneManager> m_optionScn;

	// PAD�̉摜�p
	std::shared_ptr<BottansFile> m_btImg;

	// ���j���[�̕��ԏ��p
	std::vector<std::string> m_menuTable;

	// Input�N���X�������玝���Ă���
	Input& m_input;
	// Input�N���X�̃e�[�u��
	InputTable_t m_commandTable;

	// �I���ʒu
	int m_currentLineIndex;
	// �ҏW��
	bool m_isEdit;
	int m_fadeFrame;
	// �L�����Z���m�F�t���[��
	int m_cancleFrame;

	// PAD�̔ԍ��Ɩ��O�̘A�g
	std::unordered_map<int, std::wstring> m_bottanTable;

	// �摜
	std::shared_ptr<FileBase> m_frame;
	std::shared_ptr<FileBase> m_addFrame;
	std::shared_ptr<FileBase> m_startFrame;
	// Se
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;

	// �����E�F�[�u�p�̊p�x
	float m_waveAngle;
	// �E�G�[�u�̂�`�悷�邩
	bool m_isWaveDraw;

	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;
};

