#include "Collidable.h"
#include "ColliderSphere.h"

using namespace MyEngine;

Collidable::Collidable(Priority priority, ObjectTag tag, const ColliderBase::Kind& kind)
{
}

Collidable::~Collidable()
{
}

std::shared_ptr<ColliderBase> Collidable::CreateColliderData(const ColliderBase::Kind& kind)
{
	if (kind == ColliderBase::Kind::Sphere)
	{
		m_collider = std::make_shared<ColliderSphere>();
	}

	return m_collider;
}
