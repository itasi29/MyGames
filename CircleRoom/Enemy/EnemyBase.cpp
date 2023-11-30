#include <DxLib.h>
#include "Application.h"
#include "EnemyBase.h"

namespace
{
	// 壁からの法線ベクトル
	const Vec2 kNorVecLeft = Vec2{ 1.0f,  0.0f };
	const Vec2 kNorVecRight = Vec2{ -1.0f,  0.0f };
	const Vec2 kNorVecUp = Vec2{ 0.0f,  1.0f };
	const Vec2 kNorVecDown = Vec2{ 0.0f, -1.0f };

	// ずらす方向
	const Vec2 kShiftSide = Vec2{ 0.0f, 0.2f };
	const Vec2 kShiftVert = Vec2{ 0.2f, 0.0f };
}

EnemyBase::EnemyBase(const Size& windowSize, float fieldSize) :
	m_windowSize(windowSize),
	m_fieldSize(fieldSize),
	m_radius(0),
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

void EnemyBase::Reflection(bool isShift)
{
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;

#if false
	// 左
	if (m_pos.x - m_radius < centerX - m_fieldSize)
	{
		m_pos.x = centerX - m_fieldSize + m_radius;
		ReflectionCal(kNorVecLeft);
		ShiftReflection(kShiftSide);
	}
	// 右
	if (m_pos.x + m_radius > centerX + m_fieldSize)
	{
		m_pos.x = centerX + m_fieldSize - m_radius;
		ReflectionCal(kNorVecRight);
		ShiftReflection(kShiftSide);
	}
	// 上
	if (m_pos.y - m_radius < centerY - m_fieldSize)
	{
		m_pos.y = centerY - m_fieldSize + m_radius;
		ReflectionCal(kNorVecUp);
		ShiftReflection(kShiftVert);
	}
	// 下
	if (m_pos.y + m_radius > centerY + m_fieldSize)
	{
		m_pos.y = centerY + m_fieldSize - m_radius;
		ReflectionCal(kNorVecDown);
		ShiftReflection(kShiftVert);
	}
#else
	// 左
	if (m_pos.x < centerX - m_fieldSize)
	{
		m_pos.x = centerX - m_fieldSize;
		ReflectionCal(kNorVecLeft);
		ShiftReflection(kShiftSide);
	}
	// 右
	if (m_pos.x > centerX + m_fieldSize)
	{
		m_pos.x = centerX + m_fieldSize;
		ReflectionCal(kNorVecRight);
		ShiftReflection(kShiftSide);
	}
	// 上
	if (m_pos.y < centerY - m_fieldSize)
	{
		m_pos.y = centerY - m_fieldSize;
		ReflectionCal(kNorVecUp);
		ShiftReflection(kShiftVert);
	}
	// 下
	if (m_pos.y > centerY + m_fieldSize)
	{
		m_pos.y = centerY + m_fieldSize;
		ReflectionCal(kNorVecDown);
		ShiftReflection(kShiftVert);
	}
#endif
}

void EnemyBase::ReflectionCal(Vec2 norVec)
{
	// 法線ベクトルの2倍から現在のベクトルを引く
	m_vec = m_vec + norVec * norVec.Dot(-m_vec) * 2.0f;
}

void EnemyBase::ShiftReflection(Vec2 shift)
{
	// FIXME:現状はこれでいいけど、できたら参考元にできるように

	Vec2 temp = m_vec;
	
	// 進んでいる方向にshift分進ませる
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
