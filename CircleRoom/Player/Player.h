#pragma once
#include "../Utility/Vec2.h"
#include "../Utility/Rect.h"

class Input;
class Application;
class Size;

class Player
{
public:
	Player(Application& app);
	~Player();

	/// <summary>
	/// �v���C���[�̈ʒu�A�����̏�����
	/// </summary>
	void Init();
	void Update(Input& input);
	void Draw();

	/// <summary>
	/// ���S���W���擾
	/// </summary>
	/// <returns>���S���W</returns>
	Vec2 GetPos() const { return m_pos; }

	/// <summary>
	/// �����蔻��̒��S���W���擾
	/// </summary>
	/// <returns>�����蔻��̒��S���W</returns>
	Rect GetColPos() const { return m_colRect; }
	/// <summary>
	/// �����蔻��̔��a���擾
	/// </summary>
	/// <returns>�����蔻��̔��a</returns>
	float GetColRadius() const { return m_colRaidus; }

private:
	// �A�v���P�[�V�����N���X
	Application& m_app;
	// Windows�̃T�C�Y
	Size& m_size;

	// ���S���W
	Vec2 m_pos;
	// �O���W
	Vec2 m_frontPos;
	// �E�����W
	Vec2 m_rightPos;
	// �������W
	Vec2 m_leftPos;

	// �ړ��x�N�g��
	Vec2 m_vec;

	// �����蔻��
	Rect m_colRect;
	// �����蔻��̔��a
	float m_colRaidus;

	// �������Ă��邩
	bool m_isExsit;

	/// <summary>
	/// �v���C���[�̈ړ�
	/// </summary>
	/// <param name="input">���͏��</param>
	void Move(Input& input);
};

