#pragma once
#include "MyEngine/Physics/Collidable.h"
#include "MyEngine/Physics/ColliderSphere.h"

class Player : public MyEngine::Collidable
{
public:
	Player();
	~Player();

	void Update();

	virtual void OnCollideEnter(const Collidable& colider);
	virtual void OnCollideStay(const Collidable& colider);
	virtual void OnCollideExit(const Collidable& colider);

private:
	MyEngine::ColliderSphere* m_colliderSphere;
};

