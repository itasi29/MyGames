#pragma once
#include <string>
#include "Vec2.h"
#include "Rect.h"

struct Size;

class BossBase
{
public:
	BossBase(const Size& windowSize, float fieldSize, int hp);
	virtual ~BossBase();

	virtual void Init(Vec2& pos) = 0;
	void Update();
	void Draw();

	Rect GetRect() const { return m_rect; }

	/// <summary>
	/// HP��1���炷
	/// </summary>
	virtual void OnHp();

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
	// �X�N���[���T�C�Y
	const Size& m_windoSize;
	// �t�B�[���h�T�C�Y
	const float m_fieldSize;

	// �G�̖��O
	std::string m_name;
	// �ő�̗�
	const int m_maxHp;
	// �̗�
	int m_hp;

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

