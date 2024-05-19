#include "Sphere.h"
#include <DxLib.h>

namespace
{
	constexpr unsigned int kHitColor = 0xff0000;
	constexpr unsigned int kNoHitColor = 0x0000ff;
}

Sphere::Sphere() :
	m_color(kNoHitColor)
{
}

Sphere::~Sphere()
{
}

void Sphere::Init(const Pos3& pos, float radius)
{
	m_pos = pos;
	m_radius = radius;
	m_col.Init(pos, radius);
}

void Sphere::Update(bool isMove)
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

void Sphere::Draw() const
{
	DrawSphere3D(m_pos.GetVECTOR(), m_radius, 16, m_color, 0xffffff, false);
}

bool Sphere::IsHit(const SphereCol& col)
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
