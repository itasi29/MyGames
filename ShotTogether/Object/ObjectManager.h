#pragma once
#include <unordered_map>
#include <string>
#include "Common/Geometry.h"

class FileManager;

struct ObjData
{
	Size model;
	Size col;
};

class ObjectManager
{
public:
	ObjectManager(FileManager& fileMgr);
	~ObjectManager();

	ObjData GetObjData(const wchar_t* const name) const { return m_objData.at(name); }

private:
	void LoadObjDta(const wchar_t* const path);

private:
	FileManager& m_fileMgr;
	
	std::unordered_map<std::wstring, ObjData> m_objData;
};

