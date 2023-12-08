#include "BossBase.h"

BossBase::BossBase(const Size& windowSize, float fieldSize, int hp) :
	m_windoSize(windowSize),
	m_fieldSize(fieldSize),
	m_maxHp(hp),
	m_hp(hp)
{
}

BossBase::~BossBase()
{
}

void BossBase::Update()
{
}

void BossBase::Draw()
{
}

void BossBase::OnHp()
{
	m_hp--;
}

void BossBase::StartDraw()
{
}

void BossBase::NormalDraw()
{
}

void BossBase::DrawHpBar()
{
}
