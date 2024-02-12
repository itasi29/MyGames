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
	void Draw() const;

private:	
	// �X�V�֐�
	void NormalUpdate(Input&);		// �ʏ���
	void EditUpdate(Input&);		// �ҏW���
	void EditEndUpdate(Input&);		// �ҏW�I�����

	void DrawCommandList() const;	// �R�}���h���X�g�̕`��(�e�L�X�g�`��)

	void CommitCurrenKeySetting();

	/// <summary>
	/// �I�𓙂̕����E�摜�`��
	/// </summary>
	/// /// <param name="cmd">�R�}���h��</param>
	/// <param name="str">�E�F�[�u�����镶����</param>
	/// <param name="num">������</param>
	void DrawWave(int x, int y, const char* const cmd, const wchar_t* const str[], int num) const;

	std::wstring GetKeyName(int keycode) const;

private:
	// �X�V�����o�֐��|�C���^
	using UpdateFunc_t = void(KeyConfigScene::*)(Input& input);
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
	int m_fadeFrame;
	// �L�����Z���m�F�t���[��
	int m_cancleFrame;

	// MEMO:����͕ύX������\������
	std::unordered_map<int, std::wstring> m_keynameTable;

	// �摜
	std::shared_ptr<FileBase> m_frame;
	std::shared_ptr<FileBase> m_addFrame;
	std::shared_ptr<FileBase> m_startFrame;
	// Se
	std::shared_ptr<FileBase> m_selectSe;
	std::shared_ptr<FileBase> m_cancelSe;
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;

	// �����E�F�[�u�p�̊p�x
	float m_waveAngle;

	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;
};

