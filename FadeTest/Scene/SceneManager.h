#pragma once
#include <memory>

class SceneBase;

/// <summary>
/// �V�[���Ǘ��N���X
/// </summary>
class SceneManager
{
private:
	SceneManager();

	SceneManager(const SceneManager& mgr) = delete;
	void operator= (const SceneManager& mgr) = delete;

public:
	virtual ~SceneManager();

	static SceneManager& GetInstance();

	void Init();
	void Final();
	void Update();
	void Draw() const;

	// �V�[���̕ύX
	void Change(float fadeSpeed = -1.0f);
	// �t�F�[�h���s���Ă��邩
	bool IsFade() const { return m_isFade; }

private:
	void FadeUpdate();

	void FadeDraw() const;

private:
	std::shared_ptr<SceneBase> m_scene;
	std::shared_ptr<SceneBase> m_preScene;

	// �����_�[�^�[�Q�b�g
	int m_rt;

	/* �t�F�[�h�֌W */
	bool m_isFade;			// �t�F�[�h���Ă��邩
	float m_fadeRate;		// ���݂̃t�F�[�h����
	float m_fadeSpeed;		// �t�F�[�h���s���X�s�[�h
	int m_preSceneScreen;	// �O�̃V�[���̃X�N���[���摜
	int m_psH;				// �V�F�[�_�ۑ��p�ϐ�
};

