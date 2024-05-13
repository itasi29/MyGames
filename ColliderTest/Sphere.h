#pragma once
#include "CircleCol.h"
#include "Geometry.h"

class Sphere
{
public:
	Sphere();
	~Sphere();

	CircleCol GetRect() const { return m_rect; }

	void Init(const Pos3& pos, float radius);
	void Update(bool isMove = false);
	void Draw() const;

	bool IsHit(const CircleCol& rect);

private:
	Pos3 m_pos;
	float m_radius;
	CircleCol m_rect;

	unsigned int m_color;
};

