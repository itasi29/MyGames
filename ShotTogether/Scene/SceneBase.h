#pragma once
#include <vector>
#include <memory>

class SceneManager;
class FileManager;
class SoundSystem;
class Camera;
class Input;
class FileBase;

/// <summary>
/// シーンの基底クラス
/// </summary>
class SceneBase
{
public:
	SceneBase(SceneManager& scnMgr, FileManager& fileMgr, SoundSystem& sndSys, Camera& camera);
	virtual ~SceneBase() {}

	// シーンに入るときの初期化処理
	virtual void Init() = 0;
	// 毎フレーム行う更新処理
	virtual void Update(Input& input) = 0;
	// 毎フレーム行う描画処理
	virtual void Draw() const = 0;
	// シーンを抜けるときの処理
	virtual void End() = 0;
	// シーン変更処理
	virtual std::shared_ptr<SceneBase> Change() = 0;

protected:
	SceneManager& m_scnMgr;
	FileManager& m_fileMgr;
	SoundSystem& m_sndSys;
	Camera& m_camera;

	std::vector<std::shared_ptr<FileBase>> m_file;
};

