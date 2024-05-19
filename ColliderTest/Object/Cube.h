#pragma once
#include "Geometry.h"
#include "Col/RectCol.h"

class Cube
{
public:
	Cube();
	~Cube();

	RectCol GetCol() const { return m_col; }

	void Init(const Pos3& pos, const Size& size);
	void Update(bool isMove = false);
	void Draw() const;

	bool IsHit(const RectCol& col);

private:
	Pos3 m_pos;
	RectCol m_col;

	unsigned int m_color;
};

