#pragma once
#include "Geometry.h"

class RectCol
{
public:
	RectCol();
	~RectCol();

	void Init(const Pos3& pos, const Size& size);
	void UpdatePos(const Pos3& pos);

	bool IsHit(const RectCol& rect);

	const Pos3& GetPos()  const { return m_pos;  }
	const Size& GetSize() const { return m_size; }

private:
	Pos3 m_pos;
	Size m_size;
};

