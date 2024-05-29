#include <iostream>
#include "Utility/CsvLoader.h"
#include <list>

namespace
{
	CsvHeader_t header1 =
	{
		{L"no", Type::kInt},
		{L"name", Type::kWstr},
		{L"age", Type::kInt},
		{L"height", Type::kFloat}
	};
	CsvHeader_t header2 =
	{
		{L"stageNo", Type::kInt},
		{L"stageName", Type::kWstr},
		{L"level", Type::kInt},
		{L"money", Type::kInt},
		{L"rate", Type::kFloat},
		{L"ajaja", Type::kDouble}
	};

	struct HumanData
	{
		int no;
		std::wstring name;
		int age;
		float height;
	};

	struct StageData
	{
		int no;
		std::wstring name;
		int level;
		int money;
		float rate;
		double ajaja;
	};
}

int main()
{
	auto csvData = CsvLoader::Load("Data/test.csv", header1);
	std::list<HumanData> human;
	for (auto& csv : csvData)
	{
		HumanData data;
		data.no		= csv.at(L"no").tInt;
		data.name	= csv.at(L"name").tWstr;
		data.age	= csv.at(L"age").tInt;
		data.height = csv.at(L"height").tFloat;
		human.emplace_back(data);
	}

	csvData = CsvLoader::Load("Data/test2.csv", header2);
	std::list<StageData> stage;
	for (auto& csv : csvData)
	{
		StageData data;
		data.no		= csv.at(L"stageNo").tInt;
		data.name	= csv.at(L"stageName").tWstr;
		data.level	= csv.at(L"level").tInt;
		data.money	= csv.at(L"money").tInt;
		data.rate	= csv.at(L"rate").tFloat;
		data.ajaja	= csv.at(L"ajaja").tDouble;
		stage.emplace_back(data);
	}

	return 0;
}