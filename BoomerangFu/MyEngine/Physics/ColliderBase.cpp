#include "ColliderBase.h"

MyEngine::ColliderBase::ColliderBase(Kind kind) :
	m_kind(kind)
{
}

void MyEngine::ColliderBase::UpdateHit(const ColliderBase* collider, bool isHit)
{
	m_isPreHit[collider] = m_isHit[collider];
	m_isHit[collider] = isHit;
}
