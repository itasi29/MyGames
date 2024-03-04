#include <cassert>
#include <fstream>
#include <sstream>
#include "StringUtility.h"
#include "Application.h"
#include "StageMap.h"

namespace
{
	constexpr int kStageMarginW = 1280;
	constexpr int kStageMarginH = 720;
}

StageMap::StageMap() :
	m_line(0),
	m_row(0)
{
	Load();
}

bool StageMap::IsConect(const std::string& stage, MapDir dir) const
{
	auto& data = m_data[GetIndex(stage)].info;

	if (dir == MapDir::kUp)
	{
		return data.isUp;
	}
	else if (dir == MapDir::kDown)
	{
		return data.isDown;
	}
	else if (dir == MapDir::kRight)
	{
		return data.isRight;
	}
	else if (dir == MapDir::kLeft)
	{
		return data.isLeft;
	}
}

std::string StageMap::GetConectName(const std::string& stage, MapDir dir) const
{
#ifdef _DEBUG
	assert(IsConect(stage, dir));
#endif
	auto& data = m_data[GetIndex(stage)].info;

	if (dir == MapDir::kUp)
	{
		return data.upName;
	}
	else if (dir == MapDir::kDown)
	{
		return data.downName;
	}
	else if (dir == MapDir::kRight)
	{
		return data.rightName;
	}
	else if (dir == MapDir::kLeft)
	{
		return data.leftName;
	}
}

/// <summary>
/// �X�e�[�W�ʒu��Ԃ�
/// </summary>
/// <param name="stage">�X�e�[�W��</param>
/// <returns>�ʒu</returns>
Vec2 StageMap::GetPos(const std::string& stage) const
{
	Vec2 pos = {};
	bool isEnd = false;

	for (int x = 0; x < m_line; x++)
	{
		for (int y = 0; y < m_row; y++)
		{
			if (m_data[x + y * m_line].name == stage)
			{
				const auto& wSize = Application::GetInstance().GetWindowSize();

				pos.x = static_cast<float>(wSize.w * x + kStageMarginW * x);
				pos.y = static_cast<float>(wSize.h * y + kStageMarginH * y);

				isEnd = true;
				break;
			}
		}

		if (isEnd) break;
	}

	return pos;
}

/// <summary>
/// �X�e�[�W���ɑΉ�����v�f�ԍ���Ԃ�
/// </summary>
/// <param name="stage">�X�e�[�W��</param>
/// <returns>�v�f�ԍ�</returns>
int StageMap::GetIndex(const std::string& stage) const
{
	for (int i = 0; i < static_cast<int>(m_data.size()); i++)
	{
		if (m_data[i].name != stage) continue;

		return i;
	}
}

void StageMap::Load()
{
	// �ꎞ�ۑ��pstring
	std::string strBuf;
	// �J���}�����ꎞ�ۑ��pstring
	std::vector<std::string> strUpConectNameBuf;
	std::vector<std::string> strUpConectInfoBuf;
	std::vector<std::string> strNowBuf;
	std::vector<std::string> strDownConectInfoBuf;
	std::vector<std::string> strDownConectNameBuf;

	// �t�@�C���ǂݍ���
	std::ifstream ifs("MapData.csv");
	if (!ifs)
	{
		assert(false);
		return;
	}

	std::getline(ifs, strBuf);
	strNowBuf = StringUtility::Split(strBuf, ',');

	// �s�񐔎擾
	m_line = std::stoi(strNowBuf[1]);
	m_row = std::stoi(strNowBuf[3]);
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

	while (isLoop)
	{
		if (isStart)
		{
			// ���ݏ��
			std::getline(ifs, strBuf);
			strNowBuf = StringUtility::Split(strBuf, ',');
		}
		else
		{
			// ���X�V
			strUpConectNameBuf = strNowBuf;
			strUpConectInfoBuf = strDownConectInfoBuf;
			strNowBuf = strDownConectNameBuf;
		}

		// ���ڑ����
		std::getline(ifs, strBuf);
		// �����������Ă�����p��
		isLoop = !(strBuf.empty());
		if (isLoop)
		{
			strDownConectInfoBuf = StringUtility::Split(strBuf, ',');
			// �����O���
			std::getline(ifs, strBuf);
			strDownConectNameBuf = StringUtility::Split(strBuf, ',');
		}
		
		// �f�[�^�擾
		for (int i = 0; i < m_line; i++)
		{
			int namePos = i * 2;

			auto& data = m_data[index];
			data.name = strNowBuf[namePos];

			CheckUp(data, isStart, namePos, strUpConectInfoBuf, strUpConectNameBuf);
			CheckDown(data, isLoop, namePos, strDownConectInfoBuf, strDownConectNameBuf);
			CheckLeft(data, namePos, strNowBuf);
			CheckRight(data, namePos, strNowBuf);

			// �v�f�ԍ��X�V
			index++;
		}

		isStart = false;
	}

	return;
}

/// <summary>
/// ������m�F
/// </summary>
/// <param name="data">�}�b�v�f�[�^</param>
/// <param name="isStart">���[�v�ŏ���</param>
/// <param name="index">�v�f�ԍ�</param>
/// <param name="strInfo">�ڑ���񕶎���</param>
/// <param name="strName">�ڑ����O������</param>
void StageMap::CheckUp(MapInfo& data, bool isStart, int index, const std::vector<std::string>& strInfo, const std::vector<std::string>& strName)
{
	auto& flag = data.info.isUp;
	if (isStart)
	{
		flag = false;
	}
	else
	{
		flag = static_cast<bool>(std::stoi(strInfo[index]));
		if (!flag) return;
		data.info.upName = strName[index];
	}
}

/// <summary>
/// �������m�F
/// </summary>
/// <param name="data">�}�b�v�f�[�^</param>
/// <param name="isStart">���[�v�p����</param>
/// <param name="index">�v�f�ԍ�</param>
/// <param name="strInfo">�ڑ���񕶎���</param>
/// <param name="strName">�ڑ����O������</param>
void StageMap::CheckDown(MapInfo& data, bool isLoop, int index, const std::vector<std::string>& strInfo, const std::vector<std::string>& strName)
{
	auto& flag = data.info.isDown;
	if (!isLoop)
	{
		flag = false;
	}
	else
	{
		flag = static_cast<bool>(std::stoi(strInfo[index]));
		if (!flag) return;
		data.info.downName = strName[index];
	}
}

/// <summary>
/// �������m�F
/// </summary>
/// <param name="data">�}�b�v�f�[�^</param>
/// <param name="index">�v�f�ԍ�</param>
/// <param name="str">���ݏ�񕶎���</param>
void StageMap::CheckLeft(MapInfo& data, int index, const std::vector<std::string>& str)
{
	auto& flag = data.info.isLeft;
	if (index - 1 < 0)
	{
		flag = false;
	}
	else
	{
		flag = static_cast<bool>(std::stoi(str[index - 1]));
		if (!flag) return;
		data.info.leftName = str[index - 2];
	}
}

/// <summary>
/// �E�����m�F
/// </summary>
/// <param name="data">�}�b�v�f�[�^</param>
/// <param name="index">�v�f�ԍ�</param>
/// <param name="str">���ݏ�񕶎���</param>
void StageMap::CheckRight(MapInfo& data, int index, const std::vector<std::string>& str)
{
	auto& flag = data.info.isRight;
	if (index + 1 >= m_line * 2 - 1)
	{
		flag = false;
	}
	else
	{
		flag = static_cast<bool>(std::stoi(str[index + 1]));
		if (!flag) return;
		data.info.rightName = str[index + 2];
	}
}
