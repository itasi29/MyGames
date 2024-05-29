#pragma once
#include <unordered_map>
#include <string>
#include <list>
#include <vector>

enum Type
{
	kInt,
	kFloat,
	kDouble,
	kWstr
};

struct Value
{
	int tInt;
	float tFloat;
	double tDouble;
	std::wstring tWstr;
};

using CsvHeader_t = std::unordered_map<std::wstring, Type>;
using CsvData_t = std::unordered_map<std::wstring, Value>;
using CsvOut_t = std::list<CsvData_t>;

using ConmaStr_t = std::vector<std::wstring>;
using CsvIndex_t = std::unordered_map<std::wstring, int>;

class CsvLoader final
{
public:
	static CsvOut_t Load(const std::string& path, const CsvHeader_t& header);

private:
	static void MakeIndex(const CsvHeader_t& header, const ConmaStr_t& strConmaBuf, CsvIndex_t& indexData);
	static CsvData_t LoadInfo(const CsvHeader_t& header, const ConmaStr_t& strConmaBuf, const CsvIndex_t& indexDat);
};