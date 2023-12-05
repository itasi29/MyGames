#pragma once
class Input;
class SceneManager;
class StageManager;

/// <summary>
/// �V�[�����N���X
/// �����ȃV�[���̊�ɂȂ�N���X
/// </summary>
class Scene
{
protected:
	SceneManager& m_scnMgr;
	StageManager& m_stgMgr;
public:
	/// <summary>
	/// ��������SceneManager, StageManager�̎Q�Ƃ��󂯎���Ă���
	/// </summary>
	/// <param name="manager">SceneManager�̎Q��</param>
	/// <param name="manager">StageManager�̎Q��</param>
	Scene(SceneManager& scnMgr, StageManager& stgMgr);
	virtual ~Scene() {};

	/// <summary>
	/// �V�[���̍X�V
	/// </summary>
	/// <param name="input">���̓I�u�W�F�N�g</param>
	virtual void Update(Input& input) = 0;
	/// <summary>
	/// �V�[���̕`��
	/// </summary>
	virtual void Draw() = 0;
};

