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
	m_col.Init(pos, size);
}

void Cube::Update(bool isMove)
{
	if (!isMove) return;

	const float kSpeed = 0.1f;

	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_RIGHT))
	{
		m_pos.x += kSpeed;
	}
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_LEFT))
	{
		m_pos.x -= kSpeed;
	}
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_UP))
	{
		m_pos.y += kSpeed;
	}
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_DOWN))
	{
		m_pos.y -= kSpeed;
	}
	m_col.UpdatePos(m_pos);
}

void Cube::Draw() const
{
	const auto& size = m_col.GetSize();
	float halfW = size.w * 0.5f;
	float halfH = size.h * 0.5f;
	float halfD = size.d * 0.5f;

	float right = m_pos.x + halfW;
	float left = m_pos.x - halfW;
	float top = m_pos.y + halfH;
	float bottom = m_pos.y - halfH;
	float front = m_pos.z - halfD;
	float back = m_pos.z + halfD;

#if true
	// â°ÇÃê¸
	DrawLine3D(VGet(left, bottom, front), VGet(right, bottom, front), m_color);
	DrawLine3D(VGet(left, top, front), VGet(right, top, front), m_color);
	DrawLine3D(VGet(left, bottom, back), VGet(right, bottom, back), m_color);
	DrawLine3D(VGet(left, top, back), VGet(right, top, back), m_color);
	// ècÇÃê¸
	DrawLine3D(VGet(left, top, front), VGet(left, bottom, front), m_color);
	DrawLine3D(VGet(right, top, front), VGet(right, bottom, front), m_color);
	DrawLine3D(VGet(left, top, back), VGet(left, bottom, back), m_color);
	DrawLine3D(VGet(right, top, back), VGet(right, bottom, back), m_color);
	// ëOå„ÇÃê¸
	DrawLine3D(VGet(left, top, front), VGet(left, top, back), m_color);
	DrawLine3D(VGet(left, bottom, front), VGet(left, bottom, back), m_color);
	DrawLine3D(VGet(right, top, front), VGet(right, top, back), m_color);
	DrawLine3D(VGet(right, bottom, front), VGet(right, bottom, back), m_color);
#else
	DrawCube3D(VGet(left, top, back), VGet(right, bottom, front), m_color, 0, true);
#endif
}

bool Cube::IsHit(const RectCol& col)
{
	bool isHit = m_col.IsHit(col);

	if (isHit)
	{
		m_color = kHitColor;
	}
	else
	{
		m_color = kNoHitColor;
	}

	return isHit;
}
