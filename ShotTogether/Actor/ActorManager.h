#pragma once
#include <unordered_map>
#include <string>
#include "Common/Geometry.h"

class FileManager;
class SoundSystem;

struct ActorData
{
	float speed = 0.0f;
	Size model;
	Size col;
};

class ActorManager
{
public:
	ActorManager(FileManager& fileMgr, SoundSystem& sndSys);
	~ActorManager();

	ActorData GetActorData(const wchar_t* const name) const { return m_actorData.at(name); }

private:
	void LoadActorData(const wchar_t* const path);

private:
	FileManager& m_fileMgr;
	SoundSystem& m_sndSys;

	std::unordered_map<std::wstring, ActorData> m_actorData;
};

