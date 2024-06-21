#include "ColliderSphere.h"

using namespace MyEngine;

ColliderSphere::ColliderSphere() :
	ColliderBase(ColliderBase::Kind::Sphere),
	m_radius(0.0f)
{
}
