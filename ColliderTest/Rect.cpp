#include "Rect.h"

Rect::Rect()
{
}

Rect::~Rect()
{
}

void Rect::Init(const Pos3& pos, const Size& size)
{
	m_pos = pos;
	m_size = size;
}

void Rect::UpdatePos(const Pos3& pos)
{
	m_pos = pos;
}
