#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

/// <summary>
/// �����֐�
/// </summary>
/// <param name="str">���̕�����</param>
/// <param name="del">�������镶��</param>
/// <returns>�������������Q</returns>
std::vector<std::string> Split(std::string& str, char del);

enum class MapDir
{
	kUp,
	kDown,
	kRight,
	kLeft
};

enum class ConditionsType
{
	kTime,
	kKilled,
	kSumTime
};

// ���X�e�[�W�̏��\����
struct NextLevelInfo
{
	// �������
	MapDir dir;
	// ���O
	std::string name;
	// �������
	ConditionsType type;
	// �������
	int info;
	std::vector<std::string> infoGroup;
};

// �������\����
struct CreateInfo
{
	// ����������
	int startNum;
	// ���������Ԋu
	int startInterval;
	// �����f�B���C�t���[��
	int startDelayFrame;
	// �����Ԋu
	int createInterval;
};

// �G���\����
struct EnemyInfo
{
	// ���O
	std::string name;
	// ������
	int num;
	// �������
	std::vector<CreateInfo> info;
};

struct LevelData
{
	// ���X�e�[�W���
	int nextNum;
	std::vector<NextLevelInfo> levelInfo;
	// �G���
	int enemyNum;
	std::vector<EnemyInfo> enemyInfo;
	// �{�X�X�e�[�W���
	bool isBoss;
};

// �v���O������ WinMain ����n�܂�܂�
int main()
{

	// �ꎞ�ۑ��pstring
	std::string strBuf;
	// �J���}�����ꎞ�ۑ��pstring
	std::vector<std::string> strConmaBuf;

	// �X�e�[�W���ۑ�
	std::unordered_map<std::string, LevelData> m_levelData;

	// �t�@�C���ǂݍ���
	std::ifstream ifs("StageData.csv");
	if (!ifs)
	{
		assert(false);
		return 0;
	}

	/* �X�e�[�W���̎擾 */
	std::getline(ifs, strBuf);
	strConmaBuf = Split(strBuf, ',');
	int stageNum = std::stoi(strConmaBuf[0]);

	for (int i = 0; i < stageNum; i++)
	{
		/* ���̓ǂݍ��� */
		std::getline(ifs, strBuf);
		/* �X�e�[�W���ǂݍ��� */
		std::getline(ifs, strBuf);
		strConmaBuf = Split(strBuf, ',');

		auto& data = m_levelData[strConmaBuf[0]];

		/* �אڃX�e�[�W���ǂݍ��� */
		std::getline(ifs, strBuf);
		strConmaBuf = Split(strBuf, ',');
		// �אڃX�e�[�W���ǂݍ���
		data.nextNum = std::stoi(strConmaBuf[0]);
		data.levelInfo.resize(data.nextNum);
		/* �אڃX�e�[�W���ǂݍ��� */
		for (int j = 0; j < data.nextNum; j++)
		{
			std::getline(ifs, strBuf);
			strConmaBuf = Split(strBuf, ',');

			auto& levelInfo = data.levelInfo[j];

			// �X�e�[�W�����ǂݍ���
			levelInfo.dir = static_cast<MapDir>(std::stoi(strConmaBuf[0]));
			// ���X�e�[�W���ǂݍ���
			levelInfo.name = strConmaBuf[1];
			// �����ǂݍ���
			levelInfo.type = static_cast<ConditionsType>(std::stoi(strConmaBuf[2]));
			// �����ڍ�
			levelInfo.info = std::stoi(strConmaBuf[3]);

			if (levelInfo.type != ConditionsType::kSumTime) continue;

			int num = std::stoi(strConmaBuf[4]);
			levelInfo.infoGroup.resize(num);
			for (int k = 0; k < num ; k++)
			{
				levelInfo.infoGroup[k] = strConmaBuf[k + 5];
			}
		}

		/* �G��ސ��ǂݍ��� */
		std::getline(ifs, strBuf);
		strConmaBuf = Split(strBuf, ',');
		// ��ސ��ŃT�C�Y�ύX
		data.enemyNum = std::stoi(strConmaBuf[0]);
		data.enemyInfo.resize(data.enemyNum);
		/* �����G���ǂݍ��� */
		for (int j = 0; j < data.enemyNum; j++)
		{
			std::getline(ifs, strBuf);
			strConmaBuf = Split(strBuf, ',');

			auto& enemyInfo = data.enemyInfo[j];

			// ���O�ǂݍ���
			enemyInfo.name = strConmaBuf[0];
			// �����ʎ�ސ��ǂݍ���
			enemyInfo.num = std::stoi(strConmaBuf[1]);
			enemyInfo.info.resize(enemyInfo.num);
			/* �������ǂݍ��� */
			for (int k = 0; k < enemyInfo.num; k++)
			{
				std::getline(ifs, strBuf);
				strConmaBuf = Split(strBuf, ',');

				auto& info = enemyInfo.info[k];

				// �ŏ��ɐ������鐔
				info.startNum = std::stoi(strConmaBuf[0]);
				// �ŏ��̐����Ԋu
				info.startInterval = std::stoi(strConmaBuf[1]);
				// �ŏ��̃f�B���C�t���[��
				info.startDelayFrame = std::stoi(strConmaBuf[2]);
				// �����Ԋu
				info.createInterval = std::stoi(strConmaBuf[3]);
			}
		}

		/* �{�X�����t���O */
		std::getline(ifs, strBuf);
		strConmaBuf = Split(strBuf, ',');

		data.isBoss = std::stoi(strConmaBuf[0]);
	}

	// close�����̓f�X�g���N�^�ōs����
	printf_s("�X�e�[�W�� : %d\n", m_levelData.size());
	for (const auto& level : m_levelData)
	{
		printf_s("name : %s\n", level.first.c_str());
		auto& data = level.second;
		printf_s("�אڐ��F %d\n", data.nextNum);
		for (int i = 0; i < data.nextNum; i++)
		{
			auto& aaa = data.levelInfo[i];
			printf_s("�����F%d   name : %s   ���� : %d   ��� : %d\n", aaa.dir, aaa.name.c_str(), aaa.type, aaa.info);
		}
		printf_s("�G�� : %d\n", data.enemyNum);
		for (int i = 0; i < data.enemyNum; i++)
		{
			auto& aaa = data.enemyInfo[i];
			printf_s("name : %s   ���ʏ�� : %d\n", aaa.name.c_str(), aaa.num);
			for (int j = 0; j < aaa.num; j++)
			{
				auto& bbb = aaa.info[j];
				printf_s("������ : %d   S�����Ԋu : %d   S�f�B���C : %d   �Ԋu : %d", bbb.startNum, bbb.startInterval, bbb.startDelayFrame, bbb.createInterval);
			}
		}
		printf_s("�{�X���� : %d\n\n", data.isBoss);
	}

	return 0;				// �\�t�g�̏I��
}


std::vector<std::string> Split(std::string& str, char del)
{
	// ��؂�J�n�ʒu
	int first = 0;
	// ��؂�ŏI�ʒu
	int last = str.find_first_of(del);

	std::vector<std::string> result;
	
	while (first < str.size())
	{
		// ��؂������̂����ʂɓ���Ă���
		std::string subStr(str, first, last - first);

		result.push_back(subStr);

		// �ʒu�X�V
		first = last + 1;
		last = str.find_first_of(del, first);
		if (last == std::string::npos)
		{
			last = str.size();
		}
	}

	return result;
}