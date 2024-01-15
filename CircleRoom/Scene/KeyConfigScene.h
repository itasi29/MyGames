#pragma once
#include "Scene.h"
#include <unordered_map>
#include <memory>
#include "Input.h"

class BottansFile;
class KeyFile;
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
	void EditEndUpdate(Input&);		// �ҏW�I�����

	void DrawCommandList();	// �R�}���h���X�g�̕`��(�e�L�X�g�`��)

	void CommitCurrenKeySetting();

	std::wstring GetKeyName(int keycode);

private:
	UpdateFunc_t  m_updateFunc;

	// Option�̃V�[��
	std::shared_ptr<SceneManager> m_optionScn;

	// KEY�̉摜�p
	std::shared_ptr<KeyFile> m_keyImg;

	// ����̏���ɃR�}���h����ёւ��邽�߂̔z��
	std::vector<std::string> m_menuTable;

	// Input�N���X�̎Q�Ƃ������Ă���
	Input& m_input;
	// Input�N���X�̃e�[�u��
	InputTable_t m_commandTable;

	// ���ݍs�̃C���f�b�N�X
	int m_currentLineIndex;
	// �ҏW��
	bool m_isEdit;
	int m_frame;


	// MEMO:����͕ύX������\������
	std::unordered_map<int, std::wstring> m_keynameTable;
};

