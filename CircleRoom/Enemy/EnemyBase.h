#pragma once

#include "../Utility/Vec2.h"
#include "../Utility/Rect.h"

class Size;

/// <summary>
/// Enemy�N���X�̊��
/// </summary>
class EnemyBase
{
public:
	EnemyBase(const Size& windowSize, float fieldSize);
	virtual ~EnemyBase();

	virtual void Update() = 0;
	virtual void Draw();

protected:
	// �X�N���[���T�C�Y
	const Size& m_windowSize;
	// �t�B�[���h�̃T�C�Y
	float m_fieldSize;

	// ���S���W
	Vec2 m_pos;
	// �ړ��x�N�g��
	Vec2 m_vec;
	// �����蔻��
	Rect m_colPos;
	// ���a
	float m_radius;
	// ��������
	bool m_isExsit;

protected:
	virtual void Reflection();
};

