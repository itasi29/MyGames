#pragma once
#include "ActorBase.h"

class ActorTarget : public ActorBase
{
public:
	ActorTarget(int handle, ActorManager& actorMgr, FileManager& fileMgr, SoundSystem& sndSys, const wchar_t* const name);
	virtual ~ActorTarget();

	void Init(float x, float y) override;
	void Update(Input& input) override;

	void OnDeath();

private:
	using UpdateFunc_t = void(ActorTarget::*)(Input&);
	void UpdateNeutual(Input& input);
	void UpdateDeath(Input& input);


private:
	UpdateFunc_t m_updateFunc;
};

