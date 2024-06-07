#pragma once
#include <memory>

class SceneBase;

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
	void Final();
	void Update();
	void Draw() const;

	// シーンの変更
	void Change(float fadeSpeed = -1.0f);
	// フェードを行っているか
	bool IsFade() const { return m_isFade; }

private:
	void FadeUpdate();

	void FadeDraw() const;

private:
	std::shared_ptr<SceneBase> m_scene;
	std::shared_ptr<SceneBase> m_preScene;

	// レンダーターゲット
	int m_rt;

	/* フェード関係 */
	bool m_isFade;			// フェードしているか
	float m_fadeRate;		// 現在のフェード割合
	float m_fadeSpeed;		// フェードを行うスピード
	int m_preSceneScreen;	// 前のシーンのスクリーン画像
	int m_psH;				// シェーダ保存用変数
};

