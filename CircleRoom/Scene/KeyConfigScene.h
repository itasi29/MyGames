#pragma once
#include "Scene.h"
#include <unordered_map>
#include <memory>
#include "Input.h"

class BottansFile;
class SceneManager;

/// <summary>
/// �L�[�R���t�B�O�̃V�[��
/// </summary>
class KeyConfigScene : public Scene
{
public:
	KeyConfigScene(GameManager& mgr, Input& input, std::shared_ptr<SceneManager> scn);
	~KeyConfigScene();

	void Update(Input& input);
	void Draw();

private:
	// �X�V�����o�֐��|�C���^
	using UpdateFunc_t = void(KeyConfigScene::*)(Input& input);
	
	// �X�V�֐�
	void NormalUpdate(Input&);		// �ʏ���
	void EditUpdate(Input&);		// �ҏW���

	// �`��֐�
	void ExpandDraw();	// �g���k���`��
	void NormalDraw();	// ��t�F�[�h�`��

	void DrawCommandList();	// �R�}���h���X�g�̕`��(�e�L�X�g�`��)

	void CommitCurrenKeySetting();

	std::wstring GetKeyName(int keycode);
	std::wstring GetPadName(int padstate);

private:
	UpdateFunc_t  m_updateFunc;

	std::shared_ptr<SceneManager> m_optionScn;

	int m_frame = 0;
	std::vector<std::string> m_menuItems;	// ����̏���ɃR�}���h����ёւ��邽�߂̔z��

	int m_currentLineIndex = 0;		// ���ݍs�̃C���f�b�N�X
	bool m_isEditingNow = false;	// ���ҏW���Ȃ̂�(�g�O��)

	Input& m_input;	// Input�N���X�̎Q�Ƃ������Ă���
	InputTable_t m_keyCommandTable;

	bool m_isEditRequestButton;

	std::shared_ptr<BottansFile> m_btImg;

	// MEMO:����͕ύX������\������
	std::unordered_map<int, std::wstring> m_keynameTable;
	std::unordered_map<int, std::wstring> m_bottans;
};

