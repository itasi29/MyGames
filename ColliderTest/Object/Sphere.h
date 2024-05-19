#pragma once
#include "Col/SphereCol.h"
#include "Geometry.h"

class Sphere
{
public:
	Sphere();
	~Sphere();

	SphereCol GetCol() const { return m_col; }

	void Init(const Pos3& pos, float radius);
	void Update(bool isMove = false);
	void Draw() const;

	bool IsHit(const SphereCol& circle);

private:
	Pos3 m_pos;
	float m_radius;
	SphereCol m_col;

	unsigned int m_color;
};

