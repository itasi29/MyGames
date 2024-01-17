#pragma once
#include <memory>

class FileManager;
class StageManager;
class SceneManager;
class SoundSystem;
class FontSystem;

/// <summary>
/// ゲーム全体のマネージャーを一括管理する
/// </summary>
class GameManager
{
private:
	GameManager();
	// コピー代入の禁止
	GameManager(const GameManager& game) = delete;
	void operator=(const GameManager& game) = delete;

public:
	~GameManager();

	/// <summary>
	/// ゲームマネージャークラスのインスタンスを返す
	/// </summary>
	/// <returns>ゲームマネージャークラスのインスタンス</returns>
	static GameManager& GetInstance();

	std::shared_ptr<FileManager>& GetFile();
	std::shared_ptr<StageManager>& GetStage();
	std::shared_ptr<SceneManager>& GetScene();
	std::shared_ptr<SoundSystem>& GetSound();
	std::shared_ptr<FontSystem>& GetFont();

private:
	std::shared_ptr<FileManager> m_file;
	std::shared_ptr<StageManager> m_stage;
	std::shared_ptr<SceneManager> m_scene;
	std::shared_ptr<SoundSystem> m_sound;
	std::shared_ptr<FontSystem> m_font;
};

