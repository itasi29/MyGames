#pragma once
#include "Scene.h"
#include <memory>
#include <array>

struct size;
class Input;
class FileBase;
class BottansFile;
class KeyFile;
class SceneManager;
enum class InputType;

/// <summary>
/// �|�[�Y���̃V�[���N���X
/// </summary>
class OptionScene : public Scene
{
public:
	OptionScene(GameManager& mgr, Input& input, bool isGame = true);

	void Update(Input& input);
	void Draw();

	void ChangeScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// m_isEdit�̃t���O�𔽓]������
	/// </summary>
	void InverseIsEdit() { m_isEdit[0] = !m_isEdit[0]; }
private:
	// �X�V�����o�֐��|�C���^
	using UpdateFunc_t = void(OptionScene::*)(Input& input);
	UpdateFunc_t  m_updateFunc;

	// �X�V�֐�
	void AppearUpdate(Input&);	// �o����
	void NormalUpdate(Input&);		// �ʏ���
	void DisappearUpdate(Input&);	// �ޏ���

	// �`��֐�
	void NormalDraw();	// ��t�F�[�h�`��
	
	/// <summary>
	/// ���E�ړ�������摜�̕`��
	/// </summary>
	void DrawWave(const size& size);
	/// <summary>
	/// �g���̕`��
	/// </summary>
	void DrawFrame(int divisionNum, int width, const size& size);
	/// <summary>
	/// ���e�̕�����`��
	/// </summary>
	void DrawContent(std::vector<std::wstring> strs, int width);

	/// <summary>
	/// ���j���[��ύX�����Ƃ��ɉ���`�悷�邩��ύX����
	/// </summary>
	void ChangeScene(Input& input);

private:
	// �Q�[���V�[���ƃ^�C�g���V�[���ύX������
	bool m_isGame;

	// 
	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;

	// �V�[���̈�����������Ȃ̂�SceneManager�ʘg�Ƃ��Ď���
	// ���������Ȃ�\�������邽�ߗv�m�F
	std::shared_ptr<SceneManager> m_optionScn;

	int m_frame = 0;
	std::array<bool, 2> m_isEdit;

	int m_currentMenuLine;

	bool m_isFadeOut;

	InputType m_type;

	// Se
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;
};

