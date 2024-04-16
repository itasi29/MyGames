#pragma once
#include <memory>

class SceneBase;
class SceneBackground;
class FileManager;
class SoundSystem;
class Camera;
class Input;

/// <summary>
/// シーン管理クラス
/// </summary>
class SceneManager
{
public:
	SceneManager(FileManager& fileMgr, SoundSystem& sndSys);
	virtual ~SceneManager();

	void Init(Camera& camera);
	void Update(Input& input);
	void Draw() const;
	void End();

	/// <summary>
	/// シーンの変更
	/// </summary>
	void Change();

private:
	FileManager& m_fileMgr;
	SoundSystem& m_sndSys;

	std::shared_ptr<SceneBase> m_scene;
};

