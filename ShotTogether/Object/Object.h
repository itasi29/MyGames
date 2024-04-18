#pragma once
#include <vector>
#include <memory>
#include "Common/Geometry.h"

class ObjectManager;
class FileBase;
class FileManager;

/// <summary>
/// “®‚«‚ğ‚½‚È‚¢•¨‘Ì
/// </summary>
class Object
{
public:
	Object(int handle, ObjectManager& objMgr, FileManager& fileMgr, const wchar_t* const name);
	virtual ~Object();

	virtual void Init(const Pos3& pos);
	virtual void Draw() const;

	Vec3 GetPos() const { return m_pos; }
	Rect GetRect() const { return m_rect; }
	bool IsExist() const { return m_isExist; }

private:
	std::vector<std::shared_ptr<FileBase>> m_file;

	int m_handle;

	bool m_isExist;

	Vec3 m_vel;
	Pos3 m_pos;
	Rect m_rect;
};

