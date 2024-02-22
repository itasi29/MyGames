#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

/// <summary>
/// 分割関数
/// </summary>
/// <param name="str">元の文字列</param>
/// <param name="del">分割する文字</param>
/// <returns>分割した文字群</returns>
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

// 次ステージの情報構造体
struct NextLevelInfo
{
	// 方向情報
	MapDir dir;
	// 名前
	std::string name;
	// 条件種類
	ConditionsType type;
	// 条件情報
	int info;
	std::vector<std::string> infoGroup;
};

// 生成情報構造体
struct CreateInfo
{
	// 初期生成数
	int startNum;
	// 初期生成間隔
	int startInterval;
	// 初期ディレイフレーム
	int startDelayFrame;
	// 生成間隔
	int createInterval;
};

// 敵情報構造体
struct EnemyInfo
{
	// 名前
	std::string name;
	// 生成数
	int num;
	// 生成情報
	std::vector<CreateInfo> info;
};

struct LevelData
{
	// 次ステージ情報
	int nextNum;
	std::vector<NextLevelInfo> levelInfo;
	// 敵情報
	int enemyNum;
	std::vector<EnemyInfo> enemyInfo;
	// ボスステージ情報
	bool isBoss;
};

// プログラムは WinMain から始まります
int main()
{

	// 一時保存用string
	std::string strBuf;
	// カンマ分け一時保存用string
	std::vector<std::string> strConmaBuf;

	// ステージ情報保存
	std::unordered_map<std::string, LevelData> m_levelData;

	// ファイル読み込み
	std::ifstream ifs("StageData.csv");
	if (!ifs)
	{
		assert(false);
		return 0;
	}

	/* ステージ数の取得 */
	std::getline(ifs, strBuf);
	strConmaBuf = Split(strBuf, ',');
	int stageNum = std::stoi(strConmaBuf[0]);

	for (int i = 0; i < stageNum; i++)
	{
		/* 無の読み込み */
		std::getline(ifs, strBuf);
		/* ステージ名読み込み */
		std::getline(ifs, strBuf);
		strConmaBuf = Split(strBuf, ',');

		auto& data = m_levelData[strConmaBuf[0]];

		/* 隣接ステージ数読み込み */
		std::getline(ifs, strBuf);
		strConmaBuf = Split(strBuf, ',');
		// 隣接ステージ数読み込み
		data.nextNum = std::stoi(strConmaBuf[0]);
		data.levelInfo.resize(data.nextNum);
		/* 隣接ステージ情報読み込み */
		for (int j = 0; j < data.nextNum; j++)
		{
			std::getline(ifs, strBuf);
			strConmaBuf = Split(strBuf, ',');

			auto& levelInfo = data.levelInfo[j];

			// ステージ方向読み込み
			levelInfo.dir = static_cast<MapDir>(std::stoi(strConmaBuf[0]));
			// 次ステージ名読み込み
			levelInfo.name = strConmaBuf[1];
			// 条件読み込み
			levelInfo.type = static_cast<ConditionsType>(std::stoi(strConmaBuf[2]));
			// 条件詳細
			levelInfo.info = std::stoi(strConmaBuf[3]);

			if (levelInfo.type != ConditionsType::kSumTime) continue;

			int num = std::stoi(strConmaBuf[4]);
			levelInfo.infoGroup.resize(num);
			for (int k = 0; k < num ; k++)
			{
				levelInfo.infoGroup[k] = strConmaBuf[k + 5];
			}
		}

		/* 敵種類数読み込み */
		std::getline(ifs, strBuf);
		strConmaBuf = Split(strBuf, ',');
		// 種類数でサイズ変更
		data.enemyNum = std::stoi(strConmaBuf[0]);
		data.enemyInfo.resize(data.enemyNum);
		/* 生成敵情報読み込み */
		for (int j = 0; j < data.enemyNum; j++)
		{
			std::getline(ifs, strBuf);
			strConmaBuf = Split(strBuf, ',');

			auto& enemyInfo = data.enemyInfo[j];

			// 名前読み込み
			enemyInfo.name = strConmaBuf[0];
			// 同名別種類数読み込み
			enemyInfo.num = std::stoi(strConmaBuf[1]);
			enemyInfo.info.resize(enemyInfo.num);
			/* 生成情報読み込み */
			for (int k = 0; k < enemyInfo.num; k++)
			{
				std::getline(ifs, strBuf);
				strConmaBuf = Split(strBuf, ',');

				auto& info = enemyInfo.info[k];

				// 最初に生成する数
				info.startNum = std::stoi(strConmaBuf[0]);
				// 最初の生成間隔
				info.startInterval = std::stoi(strConmaBuf[1]);
				// 最初のディレイフレーム
				info.startDelayFrame = std::stoi(strConmaBuf[2]);
				// 生成間隔
				info.createInterval = std::stoi(strConmaBuf[3]);
			}
		}

		/* ボス生成フラグ */
		std::getline(ifs, strBuf);
		strConmaBuf = Split(strBuf, ',');

		data.isBoss = std::stoi(strConmaBuf[0]);
	}

	// close処理はデストラクタで行われる
	printf_s("ステージ数 : %d\n", m_levelData.size());
	for (const auto& level : m_levelData)
	{
		printf_s("name : %s\n", level.first.c_str());
		auto& data = level.second;
		printf_s("隣接数： %d\n", data.nextNum);
		for (int i = 0; i < data.nextNum; i++)
		{
			auto& aaa = data.levelInfo[i];
			printf_s("方向：%d   name : %s   条件 : %d   情報 : %d\n", aaa.dir, aaa.name.c_str(), aaa.type, aaa.info);
		}
		printf_s("敵数 : %d\n", data.enemyNum);
		for (int i = 0; i < data.enemyNum; i++)
		{
			auto& aaa = data.enemyInfo[i];
			printf_s("name : %s   同別情報数 : %d\n", aaa.name.c_str(), aaa.num);
			for (int j = 0; j < aaa.num; j++)
			{
				auto& bbb = aaa.info[j];
				printf_s("生成数 : %d   S生成間隔 : %d   Sディレイ : %d   間隔 : %d", bbb.startNum, bbb.startInterval, bbb.startDelayFrame, bbb.createInterval);
			}
		}
		printf_s("ボス生成 : %d\n\n", data.isBoss);
	}

	return 0;				// ソフトの終了
}


std::vector<std::string> Split(std::string& str, char del)
{
	// 区切り開始位置
	int first = 0;
	// 区切り最終位置
	int last = str.find_first_of(del);

	std::vector<std::string> result;
	
	while (first < str.size())
	{
		// 区切ったものを結果に入れていく
		std::string subStr(str, first, last - first);

		result.push_back(subStr);

		// 位置更新
		first = last + 1;
		last = str.find_first_of(del, first);
		if (last == std::string::npos)
		{
			last = str.size();
		}
	}

	return result;
}