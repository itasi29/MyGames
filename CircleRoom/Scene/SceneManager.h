#pragma once
#include <memory>
#include <list>

class Application;
class Input;
class Scene;

/// <summary>
/// 各シーンの遷移をコントロールするクラス
/// </summary>
class SceneManager
{
public:
	SceneManager(Application& app);
	~SceneManager();
	/// <summary>
	/// 持っているシーンのUpdate関数を呼び出す
	/// </summary>
	/// <param name="input">入力クラス</param>
	void Update(Input& input);
	/// <summary>
	/// 持っているシーンの描画Draw関数を呼び出す
	/// </summary>
	void Draw();

	/// <summary>
	/// 現在末尾で実行中のシーンを引数で指定されたシーンに切り替える
	/// </summary>
	/// <param name="nextScene">次のシーン</param>
	void ChangeScene(std::shared_ptr<Scene> nextScene);

	/// <summary>
	/// 現在のシーンの上にシーンを乗っけます
	/// </summary>
	/// <param name="scene">上に乗っけたいシーン</param>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// 現在の末尾シーンをポップします。
	/// </summary>
	void PopScene();

	/// <summary>
	/// Applicationクラスを参照
	/// </summary>
	/// <returns>Application</returns>
	Application& GetApp() const { return m_app; }

private:
	// Application
	Application& m_app;
	// シーンを入れる
	std::list<std::shared_ptr<Scene>> m_scenes;
};

