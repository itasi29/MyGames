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
struct NextStageInfo
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

struct StageData
{
	// 次ステージ情報
	int nextNum;
	std::vector<NextStageInfo> stageInfo;
	// 敵情報
	int enemyNum;
	std::vector<EnemyInfo> enemyInfo;
	// ボスステージ情報
	bool isBoss;
};

/// <summary>
/// ステージの主な内容の読み込み
/// </summary>
/// <param name="strConmaBuf">コンマ分けした文字列</param>
/// <param name="m_stageData">これは向こうではメンバ変数に</param>
/// <param name="stageName">ステージ名</param>
/// <param name="isLoadAllEnemys">敵情報を全て取得したか</param>
/// <param name="enemyTypeIndex">敵種類の要素番号</param>
/// <param name="isLoadAllNextStages">隣接情報を全て取得したか</param>
/// <param name="nextStageIndex">隣接情報の要素番号</param>
void LoadImportantStageInfo(std::vector<std::string>& strConmaBuf, std::unordered_map<std::string, StageData>& m_stageData, std::string& stageName, bool& isLoadAllEnemys, int& enemyTypeIndex, bool& isLoadAllNextStages, int& nextStageIndex);

/// <summary>
/// 敵情報の読み込み
/// </summary>
/// <param name="strConmaBuf">コンマ分けした文字列</param>
/// <param name="data">ステージデータ</param>
/// <param name="isLoadAllEnemys">敵情報を全て取得したか</param>
/// <param name="enemyTypeIndex">敵種類の要素番号</param>
/// <param name="isLoadAllEnmeyInfo">敵1体の情報を全て取得したか</param>
/// <param name="enemyInfoIndex">敵1体の要素番号</param>
void LoadEnemys(std::vector<std::string>& strConmaBuf, StageData& data, bool& isLoadAllEnemys, int& enemyTypeIndex, bool& isLoadAllEnmeyInfo, int& enemyInfoIndex);

/// <summary>
/// 隣接情報の読み込み
/// </summary>
/// <param name="strConmaBuf">コンマ分けした文字列</param>
/// <param name="data">ステージデータ</param>
/// <param name="isLoadAllNextStages">隣接情報を全て取得したか</param>
/// <param name="nextStageIndex">隣接情報の要素番号</param>
void LoadNextStages(std::vector<std::string>& strConmaBuf, StageData& data, bool& isLoadAllNextStages, int& nextStageIndex);

namespace
{
	// インデックス番号
	constexpr int kIndexStageName			= 0;
	constexpr int kIndexEnemyTypeNum		= 1;
	constexpr int kIndexEnemyName			= 2;
	constexpr int kIndexEnemyInfoNum		= 3;
	constexpr int kIndexStartCreateNum		= 4;
	constexpr int kIndexStartCreateFrame	= 5;
	constexpr int kIndexStartDelayFrame		= 6;
	constexpr int kIndexCreateFrame			= 7;
	constexpr int kIndexIsCreateBoss		= 8;
	constexpr int kIndexNextStageNum		= 9;
	constexpr int kIndexNextStageName		= 10;
	constexpr int kIndexDir					= 11;
	constexpr int kIndexInfoType			= 12;
	constexpr int kIndexInfo				= 13;
	constexpr int kIndexInfoGroupNum		= 14;
	constexpr int kIndexInfoGroup			= 15;
}

// プログラムは WinMain から始まります
int main()
{

	// 一時保存用string
	std::string strBuf;
	// カンマ分け一時保存用string
	std::vector<std::string> strConmaBuf;

	// ステージ情報保存
	std::unordered_map<std::string, StageData> m_stageData;

	// ファイル読み込み
	std::ifstream ifs("StageData.csv");
	if (!ifs)
	{
		assert(false);
		return 0;
	}

	// ステージ名保存
	std::string stageName;
	// 敵種類数情報
	int enemyTypeIndex = 0;
	bool isLoadAllEnemys = true;
	// 敵1体の情報
	int enemyInfoIndex = 0;
	bool isLoadAllEnmeyInfo = true;
	// 隣接ステージ情報
	int nextStageIndex = 0;
	bool isLoadAllNextStages = true;

	// 最初は対応表情報が入っているだけなので無視する
	std::getline(ifs, strBuf);
	// 情報を読み切るまでループ
	while (std::getline(ifs, strBuf))
	{
		strConmaBuf = Split(strBuf, ',');

		LoadImportantStageInfo(strConmaBuf, m_stageData, stageName, isLoadAllEnemys, enemyTypeIndex, isLoadAllNextStages, nextStageIndex);
		auto& data = m_stageData[stageName];
		LoadEnemys(strConmaBuf, data, isLoadAllEnemys, enemyTypeIndex, isLoadAllEnmeyInfo, enemyInfoIndex);
		LoadNextStages(strConmaBuf, data, isLoadAllNextStages, nextStageIndex);
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

void LoadImportantStageInfo(std::vector<std::string>& strConmaBuf, std::unordered_map<std::string, StageData>& m_stageData, std::string& stageName, bool& isLoadAllEnemys, int& enemyTypeIndex, bool& isLoadAllNextStages, int& nextStageIndex)
{
	// 全ての情報を読み込んでいる場合のみ次の情報群に移行する
	if (!isLoadAllEnemys || !isLoadAllNextStages) return;
	
	// ステージ名読み込み
	stageName = strConmaBuf[kIndexStageName];
	// 敵種類数読み込み
	int enemyTypeNum = std::stoi(strConmaBuf[kIndexEnemyTypeNum]);
	// 隣接ステージ数読み込み
	int nextStageNum = std::stoi(strConmaBuf[kIndexNextStageNum]);
	// ボス生成フラグ読み込み
	bool isCreateBoss = static_cast<bool>(std::stoi(strConmaBuf[kIndexIsCreateBoss]));

	// 情報の代入
	auto& data = m_stageData[stageName];
	data.enemyNum = enemyTypeNum;
	data.enemyInfo.resize(enemyTypeNum);
	data.nextNum = nextStageNum;
	data.stageInfo.resize(nextStageNum);
	data.isBoss = isCreateBoss;

	// 情報読み込んでいないとする
	isLoadAllEnemys = false;
	enemyTypeIndex = 0;
	isLoadAllNextStages = false;
	nextStageIndex = 0;
}

void LoadEnemys(std::vector<std::string>& strConmaBuf, StageData& data, bool& isLoadAllEnemys, int& enemyTypeIndex, bool& isLoadAllEnmeyInfo, int& enemyInfoIndex)
{
	// 敵情報すべて読み込んでいたら早期リターン
	if (isLoadAllEnemys) return;

	auto& enemy = data.enemyInfo[enemyTypeIndex];

	if (isLoadAllEnmeyInfo)
	{
		// 名前読み込み
		std::string enemyName = strConmaBuf[kIndexEnemyName];
		// 同名別条件数読み込み
		int enemyInfoNum = std::stoi(strConmaBuf[kIndexEnemyInfoNum]);

		// 情報代入
		enemy.name = enemyName;
		enemy.num = enemyInfoNum;
		enemy.info.resize(enemyInfoNum);

		// 情報読み込み初期化
		isLoadAllEnmeyInfo = false;
		enemyInfoIndex = 0;
	}

	auto& info = enemy.info[enemyInfoIndex];

	// 初期生成数読み込み
	int startNum = std::stoi(strConmaBuf[kIndexStartCreateNum]);
	// 初期生成間隔読み込み
	int startCreateFrame = std::stoi(strConmaBuf[kIndexStartCreateFrame]);
	// 初期遅延フレーム
	int startDelayFrame = std::stoi(strConmaBuf[kIndexStartDelayFrame]);
	// 生成間隔
	int CreateFrame = std::stoi(strConmaBuf[kIndexCreateFrame]);

	// 情報代入
	info.startNum = startNum;
	info.startInterval = startCreateFrame;
	info.startDelayFrame = startDelayFrame;
	info.createInterval = CreateFrame;

	// 情報更新
	enemyInfoIndex++;
	if (enemyInfoIndex >= enemy.num)
	{
		isLoadAllEnmeyInfo = true;

		enemyTypeIndex++;
		if (enemyTypeIndex >= data.enemyNum)
		{
			isLoadAllEnemys = true;
		}
	}
}

void LoadNextStages(std::vector<std::string>& strConmaBuf, StageData& data, bool& isLoadAllNextStages, int& nextStageIndex)
{
	// 隣接情報すべて読み込んでいたら早期リターン
	if (isLoadAllNextStages) return;

	// ステージ名読み込み
	std::string name = strConmaBuf[kIndexNextStageName];
	// 方向読み込み
	int dir = std::stoi(strConmaBuf[kIndexDir]);
	// 条件タイプ読み込み
	int infoType = std::stoi(strConmaBuf[kIndexInfoType]);
	// 条件情報読み込み
	int info = std::stoi(strConmaBuf[kIndexInfo]);

	// 情報代入
	auto& stage = data.stageInfo[nextStageIndex];
	stage.name = name;
	stage.dir = static_cast<MapDir>(dir);
	stage.type = static_cast<ConditionsType>(infoType);
	stage.info = info;

	// 合計時間系のみ追加情報読み込み
	if (static_cast<ConditionsType>(infoType) == ConditionsType::kSumTime)
	{
		// 情報群数読み込み
		int infoNum = std::stoi(strConmaBuf[kIndexInfoGroupNum]);
		stage.infoGroup.resize(infoNum);

		// 情報群読み込み
		for (int i = 0; i < infoNum; i++)
		{
			stage.infoGroup[i] = strConmaBuf[i + kIndexInfoGroup];
		}
	}

	// 情報更新
	nextStageIndex++;
	if (nextStageIndex >= data.nextNum)
	{
		isLoadAllNextStages = true;
	}
}