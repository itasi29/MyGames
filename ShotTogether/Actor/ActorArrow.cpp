#include "ActorArrow.h"

namespace
{
	constexpr float kLimitDepth = 10.0f;
}

ActorArrow::ActorArrow(int handle, ActorManager& actorMgr, FileManager& fileMgr, SoundSystem& sndSys, const wchar_t* const name) :
	ActorBase(handle, actorMgr, fileMgr, sndSys, name)
{
}

ActorArrow::~ActorArrow()
{
}

void ActorArrow::Init(float x, float y)
{
	ActorBase::Init(x, y);
}

void ActorArrow::Update(Input& input)
{
	// ��艜�s���܂ōs�������������
	if (m_pos.z >= kLimitDepth)
	{
		m_isExist = false;
	}
}
