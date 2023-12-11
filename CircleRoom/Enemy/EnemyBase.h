#pragma once

#include <string>
#include "Vec2.h"
#include "Utility/Collision.h"

struct Size;

/// <summary>
/// Enemy�N���X�̊��
/// </summary>
class EnemyBase
{
public:
	EnemyBase(const Size& windowSize, float fieldSize);
	virtual ~EnemyBase();

	virtual void Init(const Vec2&) {}
	void Update();
	void Draw();

	/// <summary>
	/// �����蔻��̒��S���W���擾
	/// </summary>
	/// <returns>�����蔻��̒��S���W</returns>
	Collision GetRect() const { return m_col; }

	/// <summary>
	/// �G�̖��O��Ԃ�
	/// </summary>
	/// <returns>���O</returns>
	std::string GetName() const { return m_name; }

	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>true: �����Ă� / false:����ł�</returns>
	bool IsExsit() const { return m_isExsit; }

protected:
	/// <summary>
	/// �ǂɓ��������甽�˂�����
	/// </summary>
	/// <param name="scale">���a�̑傫��</param>
	/// <param name="isShift">�ǂɓ����������ɔ��˂����邩</param>
	virtual bool Reflection(float scale = 0.0f, bool isShift = true);
	/// <summary>
	/// ���˂�����v�Z
	/// </summary>
	void ReflectionCal(const Vec2& norVec);
	void ShiftReflection(const Vec2& shift);

	/// <summary>
	/// normal�֐��ɕύX����
	/// </summary>
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
	virtual void StartDraw(); 
	virtual void NormalDraw();

protected:
	// ���̉�����܂ł̎���
	static const int kApeearFrame = 60;

	// �X�N���[���T�C�Y
	const Size& m_windowSize;
	// �t�B�[���h�̃T�C�Y
	const float m_fieldSize;

	// �G�̖��O
	std::string m_name;
	// �G�̃J���[(�O���t�ɕς������������)
	unsigned int m_color;

	// ���S���W
	Vec2 m_pos;
	// �ړ��x�N�g��
	Vec2 m_vec;
	// �����蔻��
	Collision m_col;
	// ���a
	float m_radius;

	// ��������
	bool m_isExsit;

	// �t���[��
	int m_frame;
};

