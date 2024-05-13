#include "CircleCol.h"

CircleCol::CircleCol()
{
}

CircleCol::~CircleCol()
{
}

void CircleCol::Init(const Pos3& pos, float radius)
{
	m_pos = pos;
	m_radius = radius;
}

void CircleCol::UpdatePos(const Pos3& pos)
{
	m_pos = pos;
}

bool CircleCol::IsHit(const CircleCol& circle)
{
	// 相対ベクトル
	Vec3 vec = circle.GetPos() - m_pos;
	// 相対ベクトルの距離(2乗)
	float vecSqLen = vec.SqLength();

	// 半径の合計の2乗
	float radius = m_radius + circle.GetRadius();
	radius = radius * radius;

	// 判定
	return vecSqLen < radius;
}
