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
	void ChangeScene(std::shared_ptr<Scene> nextScene, float speed = -1);

	/// <summary>
	/// ���݂��邷�ׂẴV�[�����폜���V�[����؂�ւ���
	/// </summary>
	/// <param name="nextScene">���̃V�[��</param>
	/// <param name="isMoveBg">�w�i�𓮂�����</param>
	void ChangeSceneWithClear(std::shared_ptr<Scene> nextScene, float speed = -1);

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
	/// �V�F�[�_�[�J�n
	/// </summary>
	void OnShader();

	/// <summary>
	/// �K�E�X�ڂ����J�n
	/// </summary>
	void OnGaussianBlur(bool isAll = false);

	/// <summary>
	/// �ʏ�`���
	/// </summary>
	void OnNormal();

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
	void ShaderUpdate(Input& input);
	void GaussianBlurUpdate(Input& input);

	void NormalDraw() const;
	void ShakeDraw() const;
	void ShaderDraw() const;
	void GaussianBlurDraw() const;

	/// <summary>
	/// �V�F�[�_�[�`���悤
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <param name="width">��</param>
	/// <param name="height">����</param>
	void MyDraw(int x, int y, int width, int height) const;

private:
	using UpdateFunc_t = void(SceneManager::*)(Input&);
	using DrawFunc_t = void(SceneManager::*)() const;

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	// �V�[��������
	std::list<std::shared_ptr<Scene>> m_scenes;
	// �w�i�p�V�[��
	std::shared_ptr<BackgroundScene> m_bg;

	// ��ʂ�h�炷�Ƃ��Ɏg�����
	int m_shakeHandle;
	// ��ʂ�h�炷�t���[��
	int m_shakeFrame;
	// ��ʂ�h�炷�ۂ̃T�C�Y
	int m_shakeSize;
	// ��ʂ�h�炷��
	bool m_isShake;

	// �w�i��`�悷�邩
	bool m_isDrawBg;

	// �V�F�[�_�[�Ɏg���p
	int m_cbuffer;
	float m_angle;
	float* m_gAngle;
	int m_wavePs;
	int m_ps;
	int m_shaderScreen;
	int m_drawScreen;

	// �K�E�X�ڂ����Ɏg���悤
	int m_gaussianBlurScreen;
	bool m_isAllGaussianBlur;
};

