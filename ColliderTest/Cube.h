#pragma once
#include "Geometry.h"
#include "Rect.h"

class Cube
{
public:
	Cube();
	~Cube();

	void Init(const Pos3& pos, const Size& size);
	void Update(bool isMove = false);
	void Draw() const;

	bool IsHit(const Rect& rect);

private:
	Pos3 m_pos;
	Rect m_rect;

	unsigned int m_color;
};

