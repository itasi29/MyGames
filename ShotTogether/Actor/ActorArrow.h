#pragma once
#include "ActorBase.h"

class ActorArrow : public ActorBase
{
public:
	ActorArrow(int handle, ActorManager& actorMgr, FileManager& fileMgr, SoundSystem& sndSys, const wchar_t* const name);
	virtual ~ActorArrow();

	void Init(float x, float y) override;
	void Update(Input& input) override;
};

