#pragma once
#include "Geometry.h"

class Rect
{
public:
	Rect();
	~Rect();

	void Init(const Pos3& pos, const Vec3& size);
	void Update(const Pos3& pos);

private:
	Pos3 m_pos;
	Vec3 m_size;
};

