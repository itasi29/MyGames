#include "ActorManager.h"

ActorManager::ActorManager(FileManager& fileMgr, SoundSystem& sndSys) :
	m_fileMgr(fileMgr),
	m_sndSys(sndSys)
{
}

ActorManager::~ActorManager()
{
}
