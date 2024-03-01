#include <cassert>
#include <fstream>
#include <sstream>
#include "StringUtility.h"
#include "StageMap.h"

StageMap::StageMap()
{
}

void StageMap::Load()
{
	// 一時保存用string
	std::string strBuf;
	// カンマ分け一時保存用string
	std::vector<std::string> strConmaBufUp;
	std::vector<std::string> strConmaBufNow;
	std::vector<std::string> strConmaBufDown;

	// ファイル読み込み
	std::ifstream ifs("MapData.csv");
	if (!ifs)
	{
		assert(false);
		return;
	}

	std::getline(ifs, strBuf);
	strConmaBufNow = StringUtility::Split(strBuf, ',');

	// 行列数取得
	m_line = std::stoi(strConmaBufNow[2]);
	m_row = std::stoi(strConmaBufNow[4]);
	// サイズ変更
	m_data.resize(m_line * m_row);

	// 要素
	int index = 0;
	// 列数
	int line = 0;
	// 継続フラグ
	bool isLoop = true;
	// 最初フラグ
	bool isStart = true;

	std::getline(ifs, strBuf);
	strConmaBufUp = strConmaBufNow;
	strConmaBufNow = StringUtility::Split(strBuf, ',');
	while (isLoop)
	{
		strConmaBufUp = strConmaBufDown;
		std::getline(ifs, strBuf);
		strConmaBufNow = StringUtility::Split(strBuf, ',');
		std::getline(ifs, strBuf);
		strConmaBufDown = StringUtility::Split(strBuf, ',');

		// 名前取得
		for (int i = 0; i < m_row; i++)
		{
			m_data[index] = { strConmaBufNow[i * 2] , {}};

			CheckUp();
			CheckDown();
			CheckLeft();
			CheckRight();
		}
	}
}

void StageMap::CheckUp(int index, int row, int line, const std::vector<std::string>& str)
{
}

void StageMap::CheckDown(int index, int row, int line, const std::vector<std::string>& str)
{
}

void StageMap::CheckLeft(int index, int row, int line, const std::vector<std::string>& str)
{
}

void StageMap::CheckRight(int index, int row, int line, const std::vector<std::string>& str)
{
}
