#pragma once
#include "Col/CaspsuleCol.h"
#include "Geometry.h"

class SphereCol;

class Caspsule
{
public:
	Caspsule();
	~Caspsule();

	CaspsuleCol GetCol() const { return m_col; }

	void Init(const Pos3& pos, const Vec3& dir, float len, float radius);
	void Update(bool isMove = false);
	void Draw() const;

	bool IsHit(const SphereCol& col);
	bool IsHit(const CaspsuleCol& col);

private:


private:
	Pos3 m_pos;
	Vec3 m_dir;
	float m_len;
	float m_radius;
	CaspsuleCol m_col;

	unsigned int m_color;
};

