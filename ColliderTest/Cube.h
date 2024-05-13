#pragma once
#include "Geometry.h"
#include "RectCol.h"

class Cube
{
public:
	Cube();
	~Cube();

	RectCol GetRect() const { return m_rect; }

	void Init(const Pos3& pos, const Size& size);
	void Update(bool isMove = false);
	void Draw() const;

	bool IsHit(const RectCol& rect);

private:
	Pos3 m_pos;
	RectCol m_rect;

	unsigned int m_color;
};

