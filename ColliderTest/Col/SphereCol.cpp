#include "SphereCol.h"

SphereCol::SphereCol()
{
}

SphereCol::~SphereCol()
{
}

void SphereCol::Init(const Pos3& pos, float radius)
{
	m_pos = pos;
	m_radius = radius;
}

void SphereCol::UpdatePos(const Pos3& pos)
{
	m_pos = pos;
}

bool SphereCol::IsHit(const SphereCol& circle)
{
	// ���΃x�N�g��
	Vec3 vec = circle.GetPos() - m_pos;
	// ���΃x�N�g���̋���(2��)
	float vecSqLen = vec.SqLength();

	// ���a�̍��v��2��
	float radius = m_radius + circle.GetRadius();
	radius = radius * radius;

	// ����
	return vecSqLen < radius;
}
