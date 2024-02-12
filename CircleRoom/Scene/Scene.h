#pragma once
#include <memory>

class Input;
class GameManager;
class SoundSystem;

/// <summary>
/// �V�[�����N���X
/// �����ȃV�[���̊�ɂȂ�N���X
/// </summary>
class Scene
{
protected:
	GameManager& m_mgr;
	std::shared_ptr<SoundSystem>& m_sound;
public:
	/// <summary>
	/// ��������GameManager�̎Q�Ƃ��󂯎���Ă���
	/// </summary>
	/// <param name="manager">SceneManager�̎Q��</param>
	Scene(GameManager& mgr);
	virtual ~Scene() {};

	/// <summary>
	/// �V�[���̍X�V
	/// </summary>
	/// <param name="input">���̓I�u�W�F�N�g</param>
	virtual void Update(Input& input) = 0;
	/// <summary>
	/// �V�[���̕`��
	/// </summary>
	virtual void Draw() const = 0;
};

