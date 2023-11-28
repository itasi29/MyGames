#pragma once

#include "Vec2.h"
#include "Rect.h"

struct Size;

/// <summary>
/// Enemy�N���X�̊��
/// </summary>
class EnemyBase
{
public:
	EnemyBase(const Size& windowSize, float fieldSize);
	virtual ~EnemyBase();

	virtual void Init(Vec2 pos) = 0;
	void Update();
	void Draw();

	/// <summary>
	/// �����蔻��̒��S���W���擾
	/// </summary>
	/// <returns>�����蔻��̒��S���W</returns>
	Rect GetRect() const { return m_rect; }

protected:
	/// <summary>
	/// �ǂɓ��������甽�˂�����
	/// </summary>
	virtual void Reflection();
	/// <summary>
	/// ���˂�����v�Z
	/// </summary>
	void ReflectionCal(Vec2 norVec);
	void ShiftReflection(Vec2 shift);

	// normal�֐��ɕύX����֐�
	void ChangeNormalFunc();

protected:
	// �����o�֐��|�C���^
	using updateFunc_t = void(EnemyBase::*)();
	using drawFunc_t = void(EnemyBase::*)();

	updateFunc_t m_updateFunc;
	drawFunc_t m_drawFunc;

	// �X�V�֐�
	virtual void StartUpdate() = 0;
	virtual void NormalUpdate() = 0;

	// �`��֐�
	virtual void StartDraw() = 0;
	virtual void NormalDraw() = 0;

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
	Rect m_rect;
	// ���a
	float m_radius;

	// �t���[��
	int m_frame;
};

