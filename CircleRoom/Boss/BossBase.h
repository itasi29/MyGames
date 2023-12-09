#pragma once
#include <string>
#include "Vec2.h"
#include "Rect.h"

struct Size;

class BossBase
{
public:
	BossBase(const Size& windowSize, float fieldSize, int maxHp);
	virtual ~BossBase();

	virtual void Init(Vec2& pos) = 0;
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

	/// <summary>
	/// HP��1���炷
	/// </summary>
	virtual void OnAttack(const Collision& rect);

protected:
	/// <summary>
	/// �ǂɓ��������甽�˂�����
	/// </summary>
	virtual bool Reflection();
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
	using updateFunc_t = void(BossBase::*)();
	using drawFunc_t = void(BossBase::*)();

	updateFunc_t m_updateFunc;
	drawFunc_t m_drawFunc;

	virtual void StartUpdate() = 0;
	virtual void NormalUpdate() = 0;

	virtual void StartDraw();
	virtual void NormalDraw();

	/// <summary>
	/// HP�o�[�̕`��
	/// </summary>
	void DrawHpBar();

protected:
	// ���̉�����܂ł̎���
	static const int kApeearFrame = 30;

	// �X�N���[���T�C�Y
	const Size& m_windowSize;
	// �t�B�[���h�T�C�Y
	const float m_fieldSize;

	// �G�̖��O
	std::string m_name;
	// �F
	unsigned int m_color;
	// �ő�̗�
	const int m_maxHp;
	// �̗�
	int m_hp;

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

