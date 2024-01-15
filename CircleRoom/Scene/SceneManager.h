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
	SceneManager();
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
	/// 現在あるすべてのシーンを削除しシーンを切り替える
	/// </summary>
	/// <param name="nextScene">次のシーン</param>
	void ChangeSceneWithClear(std::shared_ptr<Scene> nextScene);

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
	/// 画面全体を揺らす
	/// </summary>
	/// <param name="frame">揺らすフレーム</param>
	void ShakeScreen(int frame);
	/// <summary>
	/// 画面全体を揺らす
	/// </summary>
	/// <param name="frame">揺らすフレーム</param>
	/// <param name="size">揺らす際の幅</param>
	void ShakeScreen(int frame, int size);

	/// <summary>
	/// 末尾のSceneを取得
	/// </summary>
	std::shared_ptr<Scene> GetTopScene();

private:
	// シーンを入れる
	std::list<std::shared_ptr<Scene>> m_scenes;

	// 画面を揺らすときに使う画面
	int m_shakeHandle;
	// 画面を揺らすフレーム
	int m_shakeFrame;
	// 画面を揺らす際のサイズ
	int m_shakeSize;
	// 画面を揺らすか
	bool m_isShake;
};

