#pragma once
#include "../Utility/Vec2.h"
#include "../Utility/Rect.h"
class Input;
class Player
{
public:
	Player();
	~Player();

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
	// ���S���W
	Vec2 m_pos;
	// �ړ��x�N�g��
	Vec2 m_vec;

	// �����蔻��
	Rect m_colRect;
	// �����蔻��̔��a
	float m_colRaidus;

	// �������Ă��邩
	bool m_isExsit;

	void Move();
};

