#pragma once
#include <memory>
#include <list>

class Application;
class Input;
class Scene;

/// <summary>
/// �e�V�[���̑J�ڂ��R���g���[������N���X
/// </summary>
class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	/// <summary>
	/// �����Ă���V�[����Update�֐����Ăяo��
	/// </summary>
	/// <param name="input">���̓N���X</param>
	void Update(Input& input);
	/// <summary>
	/// �����Ă���V�[���̕`��Draw�֐����Ăяo��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���ݖ����Ŏ��s���̃V�[���������Ŏw�肳�ꂽ�V�[���ɐ؂�ւ���
	/// </summary>
	/// <param name="nextScene">���̃V�[��</param>
	void ChangeScene(std::shared_ptr<Scene> nextScene);

	/// <summary>
	/// ���݂��邷�ׂẴV�[�����폜���V�[����؂�ւ���
	/// </summary>
	/// <param name="nextScene">���̃V�[��</param>
	void ChangeSceneWithClear(std::shared_ptr<Scene> nextScene);

	/// <summary>
	/// ���݂̃V�[���̏�ɃV�[����������܂�
	/// </summary>
	/// <param name="scene">��ɏ���������V�[��</param>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// ���݂̖����V�[�����|�b�v���܂��B
	/// </summary>
	void PopScene();

	/// <summary>
	/// ��ʑS�̂�h�炷
	/// </summary>
	/// <param name="frame">�h�炷�t���[��</param>
	void ShakeScreen(int frame);
	/// <summary>
	/// ��ʑS�̂�h�炷
	/// </summary>
	/// <param name="frame">�h�炷�t���[��</param>
	/// <param name="size">�h�炷�ۂ̕�</param>
	void ShakeScreen(int frame, int size);

	/// <summary>
	/// ������Scene���擾
	/// </summary>
	std::shared_ptr<Scene> GetTopScene();

private:
	// �V�[��������
	std::list<std::shared_ptr<Scene>> m_scenes;

	// ��ʂ�h�炷�Ƃ��Ɏg�����
	int m_shakeHandle;
	// ��ʂ�h�炷�t���[��
	int m_shakeFrame;
	// ��ʂ�h�炷�ۂ̃T�C�Y
	int m_shakeSize;
	// ��ʂ�h�炷��
	bool m_isShake;
};

