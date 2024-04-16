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
/// �V�[���̊��N���X
/// </summary>
class SceneBase
{
public:
	SceneBase(SceneManager& scnMgr, FileManager& fileMgr, SoundSystem& sndSys, Camera& camera);
	virtual ~SceneBase() {}

	// �V�[���ɓ���Ƃ��̏���������
	virtual void Init() = 0;
	// ���t���[���s���X�V����
	virtual void Update(Input& input) = 0;
	// ���t���[���s���`�揈��
	virtual void Draw() const = 0;
	// �V�[���𔲂���Ƃ��̏���
	virtual void End() = 0;
	// �V�[���ύX����
	virtual std::shared_ptr<SceneBase> Change() = 0;

protected:
	SceneManager& m_scnMgr;
	FileManager& m_fileMgr;
	SoundSystem& m_sndSys;
	Camera& m_camera;

	std::vector<std::shared_ptr<FileBase>> m_file;
};

