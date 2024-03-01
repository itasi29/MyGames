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
	// �ꎞ�ۑ��pstring
	std::string strBuf;
	// �J���}�����ꎞ�ۑ��pstring
	std::vector<std::string> strConmaBufUp;
	std::vector<std::string> strConmaBufNow;
	std::vector<std::string> strConmaBufDown;

	// �t�@�C���ǂݍ���
	std::ifstream ifs("MapData.csv");
	if (!ifs)
	{
		assert(false);
		return;
	}

	std::getline(ifs, strBuf);
	strConmaBufNow = StringUtility::Split(strBuf, ',');

	// �s�񐔎擾
	m_line = std::stoi(strConmaBufNow[2]);
	m_row = std::stoi(strConmaBufNow[4]);
	// �T�C�Y�ύX
	m_data.resize(m_line * m_row);

	// �v�f
	int index = 0;
	// ��
	int line = 0;
	// �p���t���O
	bool isLoop = true;
	// �ŏ��t���O
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

		// ���O�擾
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
