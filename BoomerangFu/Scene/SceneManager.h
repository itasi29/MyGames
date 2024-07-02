#pragma once
#include <memory>

class SceneBase;
class Input;

/// <summary>
/// シーン管理クラス
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
	/// シーンの変更
	/// </summary>
	/// <param name="isFade">true: フェードを行う(default) / false: フェードを行わい</param>
	void Change(bool isFade = true);

	/// <summary>
	/// 流すBgmを変更する
	/// </summary>
	/// <param name="handle">流すBgmのハンドル</param>
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

