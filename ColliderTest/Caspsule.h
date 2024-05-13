#pragma once
#include "CaspsuleCol.h"
#include "Geometry.h"

class CircleCol;

class Caspsule
{
public:
	Caspsule();
	~Caspsule();

	CaspsuleCol GetRect() const { return m_rect; }

	void Init(const Pos3& pos, const Vec3& dir, float len, float radius);
	void Update(bool isMove = false);
	void Draw() const;

	bool IsHit(const CircleCol& rect);

private:
	Pos3 m_pos;
	Vec3 m_dir;
	float m_len;
	float m_radius;
	CaspsuleCol m_rect;

	unsigned int m_color;
};

