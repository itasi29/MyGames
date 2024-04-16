#pragma once
#include <memory>

class SceneBase;
class SceneBackground;
class FileManager;
class SoundSystem;
class Camera;
class Input;

/// <summary>
/// �V�[���Ǘ��N���X
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
	/// �V�[���̕ύX
	/// </summary>
	void Change();

private:
	FileManager& m_fileMgr;
	SoundSystem& m_sndSys;

	std::shared_ptr<SceneBase> m_scene;
};

