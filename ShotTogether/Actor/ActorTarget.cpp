#include "ActorTarget.h"

ActorTarget::ActorTarget(int handle, ActorManager& actorMgr, FileManager& fileMgr, SoundSystem& sndSys, const wchar_t* const name) :
	ActorBase(handle, actorMgr, fileMgr, sndSys, name),
	m_updateFunc(&ActorTarget::UpdateNeutual)
{
}

ActorTarget::~ActorTarget()
{
}

void ActorTarget::Init(float x, float y)
{
}

void ActorTarget::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void ActorTarget::OnDeath()
{
}

void ActorTarget::UpdateNeutual(Input& input)
{
}

void ActorTarget::UpdateDeath(Input& input)
{
	if (m_isAnimOneLoop)
	{

	}
}
