#pragma once
#include "MyEngine/Physics/Collidable.h"
#include "MyEngine/Physics/ColliderSphere.h"

class Enemy : public MyEngine::Collidable
{
public:
    Enemy();
    ~Enemy();

private:
    MyEngine::ColliderSphere* m_colliderSphere;
};

