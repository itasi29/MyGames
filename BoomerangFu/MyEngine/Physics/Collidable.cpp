#include "Collidable.h"
#include "ColliderSphere.h"

using namespace MyEngine;

Collidable::Collidable(Priority priority, ObjectTag tag, const ColliderBase::Kind& kind) :
	m_priority(priority),
	m_tag(tag)
{
	CreateColliderData(kind);
}

Collidable::~Collidable()
{
}

void MyEngine::Collidable::AddCollider(const ColliderBase::Kind& kind)
{
	CreateColliderData(kind);
}

void Collidable::CreateColliderData(const ColliderBase::Kind& kind)
{
	if (kind == ColliderBase::Kind::Sphere)
	{
		m_collider = std::make_shared<ColliderSphere>();
	}
}
