#pragma once
#include "Geometry.h"

class CircleCol
{
public:
	CircleCol();
	~CircleCol();

	void Init(const Pos3& pos, float radius);
	void UpdatePos(const Pos3& pos);

	bool IsHit(const CircleCol& circle);

	const Pos3& GetPos()  const { return m_pos; }
	float GetRadius() const { return m_radius; }

private:
	Pos3 m_pos;
	float m_radius;
};

