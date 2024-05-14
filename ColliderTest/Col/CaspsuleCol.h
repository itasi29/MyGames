#pragma once
#include "Geometry.h"

class SphereCol;

class CaspsuleCol
{
public:
	CaspsuleCol();
	~CaspsuleCol();

	void Init(const Pos3& pos, const Vec3& dir, float len, float radius);
	void UpdatePos(const Pos3& pos, const Vec3& dir);

	bool IsHit(const SphereCol& col);
	bool IsHit(const CaspsuleCol& col);

	const Pos3& GetPos() const { return m_pos; }
	const Vec3& GetDir() const { return m_dir; }
	float GetLength() const { return m_len; }
	float GetRadius() const { return m_radius; }

private:
	Pos3 m_pos;
	Vec3 m_dir;
	float m_len;
	float m_radius;
};

