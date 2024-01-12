#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "Scene.h"
#include "Input.h"

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
	void DrawCommandList();

private:
	// option�̃V�[��
	std::shared_ptr<SceneManager> m_optionScn;

	// ���j���[�̕��ԏ��p
	std::vector<std::string> m_menuTable;

	// Input�N���X�������玝���Ă���
	Input& m_input;
	// Input�N���X�̃e�[�u��
	InputTable_t m_commandTable;

	// �I���ʒu
	int m_currentLineIndex;

	// PAD�̔ԍ��Ɩ��O�̘A�g
	std::unordered_map<int, std::wstring> m_bottanTable;
};

