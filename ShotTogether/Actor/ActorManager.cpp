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
	// ファイル読み込み
	std::wifstream ifs(path);
	if (!ifs) {
		assert(false);
		return;
	}

	std::wstring strBuf;
	std::vector<std::wstring> strConmaBuf;

	// 余分な部分の読み込み
	std::getline(ifs, strBuf);

	while (std::getline(ifs, strBuf))
	{
		strConmaBuf = StringUtility::Split(strBuf, ',');

		// 名前取得
		std::wstring& name = strConmaBuf[kName];

		// データの生成
		auto& data = m_actorData[name];

		// 各種パラメター代入
		data.speed = std::stof(strConmaBuf[kSpeed]);
		data.model.w = std::stof(strConmaBuf[kModelWidth]);
		data.model.h = std::stof(strConmaBuf[kModelHeight]);
		data.col.w = std::stof(strConmaBuf[kColWidth]);
		data.col.h = std::stof(strConmaBuf[kColHeight]);
	}
}
