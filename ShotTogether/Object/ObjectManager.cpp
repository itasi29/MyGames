#include "ObjectManager.h"
#include <DxLib.h>
#include <cassert>
#include <fstream>
#include <sstream>
#include "Utility/StringUtility.h"

namespace
{
	const enum ActorDataIndex : int
	{
		kName,
		kSpeed,
		kModelWidth,
		kModelHeight,
		kColWidth,
		kColHeight
	};

	const wchar_t* const kObjDataPath = L"Data/Master/ObjectData.csv";
}

ObjectManager::ObjectManager(FileManager& fileMgr) :
	m_fileMgr(fileMgr)
{
	LoadObjDta(kObjDataPath);
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::LoadObjDta(const wchar_t* const path)
{
	// �t�@�C���ǂݍ���
	std::wifstream ifs(path);
	if (!ifs) {
		assert(false);
		return;
	}

	std::wstring strBuf;
	std::vector<std::wstring> strConmaBuf;

	// �]���ȕ����̓ǂݍ���
	std::getline(ifs, strBuf);

	while (std::getline(ifs, strBuf))
	{
		strConmaBuf = StringUtility::Split(strBuf, ',');

		// ���O�擾
		std::wstring& name = strConmaBuf[kName];
	}
}
