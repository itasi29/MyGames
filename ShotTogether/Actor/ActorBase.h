#pragma once
#include <vector>
#include <memory>
#include "Common/Geometry.h"

class Input;
class ActorManager;
class SoundSystem;
class FileBase;
class FileManager;

/// <summary>
/// “®‚«‚ğ‚Â•¨‘Ì‚ÌŠî’êƒNƒ‰ƒX
/// </summary>
class ActorBase
{
public:
	ActorBase(int handle, ActorManager& actorMgr, FileManager& fileMgr, SoundSystem& sndSys, const wchar_t* const name);
	virtual ~ActorBase();

	virtual void Init(float x, float y);
	virtual void Update(Input& input);
	virtual void Draw() const;

	Vec3 GetPos() const { return m_pos; }
	Rect GetRect() const { return m_rect; }
	bool IsExist() const { return m_isExist; }

protected:
	void ChangeAnim(int animIndex);

protected:
	SoundSystem& m_sndSys;
	std::vector<std::shared_ptr<FileBase>> m_file;

	int m_handle;

	int m_attachIndex;
	float m_nowAnimTime;
	float m_totalAnimTime;
	bool m_isAnimOneLoop;

	bool m_isExist;

	Vec3 m_vel;
	Pos3 m_pos;
	Rect m_rect;
};

