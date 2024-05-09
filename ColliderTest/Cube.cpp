#include "Cube.h"
#include <DxLib.h>

namespace
{
	constexpr unsigned int kHitColor   = 0xff0000;
	constexpr unsigned int kNoHitColor = 0x0000ff;

}

Cube::Cube() :
	m_color(kNoHitColor)
{
}

Cube::~Cube()
{
}

void Cube::Init(const Pos3& pos, const Size& size)
{
	m_pos = pos;
	m_rect.Init(pos, size);
}

void Cube::Update(bool isMove)
{
	if (!isMove) return;


}

void Cube::Draw() const
{
	const auto& size = m_rect.GetSize();
	float halfW = size.w * 0.5f;
	float halfH = size.h * 0.5f;
	float halfD = size.d * 0.5f;

	// 手前左下の座標
	VECTOR FLBPos = VGet(m_pos.x - halfW, m_pos.y - halfH, m_pos.z - halfD);
	// 奥右上の座標
	VECTOR BRTPos = VGet(m_pos.x + halfW, m_pos.y + halfH, m_pos.z + halfD);

	DrawCube3D(FLBPos, BRTPos, m_color, 0x0f0f0f, true);
}

bool Cube::IsHit(const Rect& rect)
{
	return false;
}
