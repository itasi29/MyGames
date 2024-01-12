#pragma once
#include "Scene.h"
#include <memory>

class Input;
class SceneManager;

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
	void InverseIsEdit() { m_isEdit = !m_isEdit; }

	bool IsEdit() const { return m_isEdit; }

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

	void DrawContent(std::vector<std::wstring> strs, int width);

	/// <summary>
	/// ���j���[��ύX�����Ƃ��ɉ���`�悷�邩��ύX����
	/// </summary>
	void ChangeScene(Input& input);

private:
	// �Q�[���V�[���ƃ^�C�g���V�[���ύX������
	bool m_isGame;

	// �V�[���̈�����������Ȃ̂�SceneManager�ʘg�Ƃ��Ď���
	// ���������Ȃ�\�������邽�ߗv�m�F
	std::shared_ptr<SceneManager> m_optionScn;

	int m_frame = 0;
	bool m_isEdit;

	int m_currentMenuLine;

	bool m_isFadeOut;
};

