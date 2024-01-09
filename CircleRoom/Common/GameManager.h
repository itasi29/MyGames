#pragma once

#include "Scene/SceneManager.h"
#include "Stage/StageManager.h"
#include "FileSystem/FileManager.h"

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

	StageManager& GetStage();
	FileManager& GetFile();
	SceneManager& GetScene();

private:
	FileManager m_file;
	StageManager m_stage;
	SceneManager m_scene;
};

