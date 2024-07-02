#include "Enemy.h"

Enemy::Enemy() :
	MyEngine::Collidable(MyEngine::Collidable::Priority::High, ObjectTag::Enemy, MyEngine::ColliderBase::Kind::Sphere)
{
	m_colliderSphere = dynamic_cast<MyEngine::ColliderSphere*>(m_collider.get());
	m_colliderSphere->radius = 5.0f;
	m_rigid.SetPos(MyEngine::Vec3(8, 0, 0));
}

Enemy::~Enemy()
{
}
