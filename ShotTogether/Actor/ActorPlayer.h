#pragma once
#include "ActorBase.h"

class ActorPlayer : public ActorBase
{
public:
	ActorPlayer(int handle, ActorManager& actorMgr, FileManager& fileMgr, SoundSystem& sndSys, const wchar_t* const name);
	virtual ~ActorPlayer();

	void Init(float x, float y) override;
	void Update(Input& input) override;

private:
	using UpdateFunc_t = void(ActorPlayer::*)(Input&);
	void UpdateNeutual(Input& input);
	void UpdateMoveRight(Input& input);
	void UpdateMoveLeft(Input& input);
	void UpdateShot(Input& input);
	void UpdateReload(Input& input);

	void OnNeutual();
	void OnMove(bool isRight);
	void OnShot();
	void OnReload();

private:
	UpdateFunc_t m_updateFunc;
};

