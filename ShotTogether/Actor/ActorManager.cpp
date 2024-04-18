#include "ActorManager.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <vector>
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

	const wchar_t* const kActorDataPath = L"Data/Master/ActorData.csv";
}

ActorManager::ActorManager(FileManager& fileMgr, SoundSystem& sndSys) :
	m_fileMgr(fileMgr),
	m_sndSys(sndSys)
{
	LoadActorData(kActorDataPath);
}

ActorManager::~ActorManager()
{
}

void ActorManager::LoadActorData(const wchar_t* const path)
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

		// �f�[�^�̐���
		auto& data = m_actorData[name];

		// �e��p�����^�[���
		data.speed = std::stof(strConmaBuf[kSpeed]);
		data.model.w = std::stof(strConmaBuf[kModelWidth]);
		data.model.h = std::stof(strConmaBuf[kModelHeight]);
		data.col.w = std::stof(strConmaBuf[kColWidth]);
		data.col.h = std::stof(strConmaBuf[kColHeight]);
	}
}
