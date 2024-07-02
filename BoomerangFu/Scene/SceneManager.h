#pragma once
#include <memory>

class SceneBase;
class Input;

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
	void Update();
	void Draw() const;
	void End();

	/// <summary>
	/// �V�[���̕ύX
	/// </summary>
	/// <param name="isFade">true: �t�F�[�h���s��(default) / false: �t�F�[�h���s�킢</param>
	void Change(bool isFade = true);

	/// <summary>
	/// ����Bgm��ύX����
	/// </summary>
	/// <param name="handle">����Bgm�̃n���h��</param>
	void ChangeBgmH(int handle);

private:
	using FadeFunc_t = void(SceneManager::*)();
	void FadeNone();
	void FadeIn();
	void FadeOut();

private:
	std::shared_ptr<SceneBase> m_scene;

	FadeFunc_t m_updateFunc;
	float m_fadeRate;
	bool m_isFade;
	int m_bgmH;
};

