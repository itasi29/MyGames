#pragma once
#include "Common/Geometry.h"

class ActorManager;
class SoundSystem;
class FileBase;
class FileManager;

class ObjectBase
{
public:
	ObjectBase();
	virtual ~ObjectBase();



private:
	Vec3 m_vel;
	Pos3 m_pos;
	Rect m_rect;
};

