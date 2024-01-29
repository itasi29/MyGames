#pragma once
#include <memory>
#include <list>
#include "Vec2.h"

class Application;
class Input;
class Scene;
class FileBase;
class BackgroundScene;

/// <summary>
/// �e�V�[���̑J�ڂ��R���g���[������N���X
/// </summary>
class SceneManager
{
public:
	SceneManager(bool isDrawBg = true);
	~SceneManager();

	void Init();

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
	/// <param name="isMoveBg">�w�i�𓮂�����</param>
	void ChangeScene(std::shared_ptr<Scene> nextScene, bool isMoveBg = true);

	/// <summary>
	/// ���݂��邷�ׂẴV�[�����폜���V�[����؂�ւ���
	/// </summary>
	/// <param name="nextScene">���̃V�[��</param>
	/// <param name="isMoveBg">�w�i�𓮂�����</param>
	void ChangeSceneWithClear(std::shared_ptr<Scene> nextScene, bool isMoveBg = true);

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
	/// ��ʂ��w��x�N�g���ɓ�����
	/// </summary>
	/// <param name="vec">�x�N�g��</param>
	void MoveScreen(const Vec2& vec);

	/// <summary>
	/// ������Scene���擾
	/// </summary>
	std::shared_ptr<Scene> GetTopScene();

	/// <summary>
	/// ���ݕ`�悵�Ă��������
	/// </summary>
	/// <returns>�`���n���h��</returns>
	int GetScreenHandle() const;

private:
	void NormalUpdate(Input& input);
	void ShakeUpdate(Input& input);
	void MoveUpdate(Input& input);

	void NormalDraw() const;
	void ShakeDraw() const;
	void MoveDraw() const;

private:
	using UpdateFunc_t = void(SceneManager::*)(Input&);
	using DrawFunc_t = void(SceneManager::*)() const;

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

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

	int m_moveScreen;

	// ��ʂ𓮂����x�N�g��
	Vec2 m_vec;
	// ���������X����
	bool m_isBaseX;
	// sinf���g���悤
	double m_angle;

	// �w�i�p
	std::shared_ptr<BackgroundScene> m_bg;

	bool m_isDrawBg;
};

