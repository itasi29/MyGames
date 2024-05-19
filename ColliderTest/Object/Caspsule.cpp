#include "Caspsule.h"
#include <DxLib.h>

namespace
{
	constexpr unsigned int kHitColor = 0xff0000;
	constexpr unsigned int kNoHitColor = 0x0000ff;

}

Caspsule::Caspsule() :
	m_color(kNoHitColor)
{
}

Caspsule::~Caspsule()
{
}

void Caspsule::Init(const Pos3& pos, const Vec3& dir, float len, float radius)
{
	m_pos = pos;
	m_dir = dir;
	m_len = len;
	m_radius = radius;
	m_col.Init(pos, dir, len , radius);
}

void Caspsule::Update(bool isMove)
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
	m_col.UpdatePos(m_pos, m_dir);
}

void Caspsule::Draw() const
{
	// •ûŒüƒxƒNƒgƒ‹(”¼•ª‚Ì‘å‚«‚³)
	Vec3 dirVec = m_dir.GetNormalized() * m_len * 0.5f;

	Pos3 pos1 = m_pos + dirVec;
	Pos3 pos2 = m_pos - dirVec;

	DrawCapsule3D(pos1.GetVECTOR(), pos2.GetVECTOR(), m_radius, 16, m_color, 0, false);
}

bool Caspsule::IsHit(const SphereCol& col)
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

bool Caspsule::IsHit(const CaspsuleCol& col)
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
