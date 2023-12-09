#include <DxLib.h>
#include "Application.h"
#include "EnemyBase.h"

namespace
{
	// �ǂ���̖@���x�N�g��
	const Vec2 kNorVecLeft = Vec2{ 1.0f,  0.0f };
	const Vec2 kNorVecRight = Vec2{ -1.0f,  0.0f };
	const Vec2 kNorVecUp = Vec2{ 0.0f,  1.0f };
	const Vec2 kNorVecDown = Vec2{ 0.0f, -1.0f };

	// ���炷����
	const Vec2 kShiftSide = Vec2{ 0.0f, 0.2f };
	const Vec2 kShiftVert = Vec2{ 0.2f, 0.0f };
}

EnemyBase::EnemyBase(const Size& windowSize, float fieldSize) :
	m_windowSize(windowSize),
	m_fieldSize(fieldSize),
	m_color(0),
	m_radius(0),
	m_isExsit(true),
	m_frame(0)
{
	m_updateFunc = &EnemyBase::StartUpdate;
	m_drawFunc = &EnemyBase::StartDraw;
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Update()
{
	(this->*m_updateFunc)();
}

void EnemyBase::Draw()
{
	(this->*m_drawFunc)();
}

bool EnemyBase::Reflection(bool isShift)
{
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;

	// ��
	if (m_pos.x < centerX - m_fieldSize)
	{
		m_pos.x = centerX - m_fieldSize;
		if (isShift)
		{
			ReflectionCal(kNorVecLeft);
			ShiftReflection(kShiftSide);
		}
		else
		{
			m_vec = { m_vec.y, -m_vec.x};
		}

		return true;
	}
	// �E
	if (m_pos.x > centerX + m_fieldSize)
	{
		m_pos.x = centerX + m_fieldSize;
		if (isShift)
		{
			ReflectionCal(kNorVecRight);
			ShiftReflection(kShiftSide);
		}
		else
		{
			m_vec = { m_vec.y, -m_vec.x };
		}

		return true;
	}
	// ��
	if (m_pos.y < centerY - m_fieldSize)
	{
		m_pos.y = centerY - m_fieldSize;
		if (isShift)
		{
			ReflectionCal(kNorVecUp);
			ShiftReflection(kShiftVert);
		}
		else
		{
			m_vec = { m_vec.y, -m_vec.x };
		}

		return true;
	}
	// ��
	if (m_pos.y > centerY + m_fieldSize)
	{
		m_pos.y = centerY + m_fieldSize;
		if (isShift)
		{
			ReflectionCal(kNorVecDown);
			ShiftReflection(kShiftVert);
		}
		else
		{
			m_vec = { m_vec.y, -m_vec.x };
		}

		return true;
	}

	return false;
}

void EnemyBase::ReflectionCal(const Vec2& norVec)
{
	// �@���x�N�g����2�{���猻�݂̃x�N�g��������
	m_vec = m_vec + norVec * norVec.Dot(-m_vec) * 2.0f;
}

void EnemyBase::ShiftReflection(const Vec2& shift)
{
	// FIXME:����͂���ł������ǁA�ł�����Q�l���ɂł���悤��

	Vec2 temp = m_vec;
	
	// �i��ł��������shift���i�܂���
	if (temp.x > 0)
	{
		temp.x += shift.x;
	}
	else
	{
		temp.x -= shift.x;
	}

	if (temp.y > 0)
	{
		temp.y += shift.y;
	}
	else
	{
		temp.y -= shift.y;
	}

	m_vec = temp.GetNormalized() * m_vec.Length();
}

void EnemyBase::ChangeNormalFunc()
{
	m_updateFunc = &EnemyBase::NormalUpdate;
	m_drawFunc = &EnemyBase::NormalDraw;
}

void EnemyBase::StartDraw()
{
	float rate = static_cast<float>(m_frame) / static_cast<float>(kApeearFrame);
	int alpha = static_cast<int>(255 * rate);
	SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), m_color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EnemyBase::NormalDraw()
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), m_color, true);

#ifdef _DEBUG
	// �����蔻��̕`��
	m_col.Draw(0xff0000, false);
#endif
}
