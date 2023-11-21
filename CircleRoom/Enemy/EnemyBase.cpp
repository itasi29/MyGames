#include <DxLib.h>
#include "../Application.h"
#include "EnemyBase.h"

namespace
{
	// 壁からの法線ベクトル
	const Vec2 kNorVecLeft  = Vec2{  1.0f,  0.0f };
	const Vec2 kNorVecRight = Vec2{ -1.0f,  0.0f };
	const Vec2 kNorVecUp    = Vec2{  0.0f,  1.0f };
	const Vec2 kNorVecDown  = Vec2{  0.0f, -1.0f };
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

void EnemyBase::Reflection()
{
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;

	// 壁に当たったら反時計回りに90度回転
	// 左
	if (m_pos.x - m_radius < centerX - m_fieldSize)
	{
		m_pos.x = centerX - m_fieldSize + m_radius;
		ReflectionCal(kNorVecLeft);
	}
	// 右
	if (m_pos.x + m_radius > centerX + m_fieldSize)
	{
		m_pos.x = centerX + m_fieldSize - m_radius;
		ReflectionCal(kNorVecRight);
	}
	// 上
	if (m_pos.y - m_radius < centerY - m_fieldSize)
	{
		m_pos.y = centerY - m_fieldSize + m_radius;
		ReflectionCal(kNorVecUp);
	}
	// 下
	if (m_pos.y + m_radius > centerY + m_fieldSize)
	{
		m_pos.y = centerY + m_fieldSize - m_radius;
		ReflectionCal(kNorVecDown);
	}
}

void EnemyBase::ReflectionCal(Vec2 norVec)
{
	// 法線ベクトルの長さを現在のベクトルの長さと同じにする
	norVec = norVec * norVec.Dot(-m_vec);

	// 法線ベクトルの2倍から現在のベクトルを引く
	m_vec = m_vec + norVec * 2;
}

void EnemyBase::ChangeNormalFunc()
{
	m_updateFunc = &EnemyBase::NormalUpdate;
	m_drawFunc = &EnemyBase::NormalDraw;
}
