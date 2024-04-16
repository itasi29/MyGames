#pragma once

class FileManager;
class SoundSystem;

class ActorManager
{
public:
	ActorManager(FileManager& fileMgr, SoundSystem& sndSys);
	~ActorManager();

private:
	FileManager& m_fileMgr;
	SoundSystem& m_sndSys;
};

