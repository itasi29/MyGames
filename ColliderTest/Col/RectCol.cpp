#include "RectCol.h"
#include <cmath>

RectCol::RectCol()
{
}

RectCol::~RectCol()
{
}

void RectCol::Init(const Pos3& pos, const Size& size)
{
	m_pos = pos;
	m_size = size;
}

void RectCol::UpdatePos(const Pos3& pos)
{
	m_pos = pos;
}

bool RectCol::IsHit(const RectCol& rect)
{
	// 相対ベクトルの作成
	auto vec = rect.GetPos() - m_pos;
	// 値の絶対値化
	vec.x = fabs(vec.x);
	vec.y = fabs(vec.y);
	vec.z = fabs(vec.z);

	// それぞれのサイズの半分のものを作成
	Size size = (rect.GetSize() + m_size) * 0.5f;

	// 各成分の判定
	bool isHitX = vec.x < size.w;
	bool isHitY = vec.y < size.h;
	bool isHitZ = vec.z < size.d;

	// 判定
	return isHitX && isHitY && isHitZ;
}
