#pragma once
#include <vector>
#include <memory>

class SceneManager;
class FileBase;
class Input;

/// <summary>
/// �V�[���̊��N���X
/// </summary>
class SceneBase
{
public:
	SceneBase();
	virtual ~SceneBase() {}

	// �V�[���ɓ���Ƃ��̏���������
	virtual void Init() = 0;
	// ���t���[���s���X�V����
	virtual void Update(bool isFade) = 0;
	// ���t���[���s���`�揈��
	virtual void Draw() const = 0;
	// �V�[���𔲂���Ƃ��̏���
	virtual void End() = 0;
	// �V�[���ύX����
	virtual std::shared_ptr<SceneBase> Change() = 0;

protected:
	SceneManager& m_scnMgr;
	std::vector<std::shared_ptr<FileBase>> m_file;
};

