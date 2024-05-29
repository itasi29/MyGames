#include "CsvLoader.h"
#include "StringUtility.h"
#include <cassert>
#include <fstream>
#include <sstream>

CsvOut_t CsvLoader::Load(const std::string& path, const CsvHeader_t& header)
{
	// ファイル読み込み
	std::wifstream ifs(path.c_str());
	if (!ifs)
	{
		assert(false);
		return CsvOut_t();
	}

	/* 変数 */
	std::wstring strBuf;	// CSVから1行読み込んだ文字列を保存
	ConmaStr_t strConmaBuf;	// 読み込んだ文字列を','で分けた状態で保存
	CsvIndex_t indexData;		// 要素情報

	/* indexDataの作成 */
	std::getline(ifs, strBuf);
	// HACK: なぜか知らないが頭3つにゴミが入るため消す
	strBuf = strBuf.substr(3);
	strConmaBuf = StringUtility::Split(strBuf, ',');
	MakeIndex(header, strConmaBuf, indexData);

	/* データの読み込み */
	CsvOut_t result;
	while (std::getline(ifs, strBuf))
	{
		strConmaBuf = StringUtility::Split(strBuf, ',');

		result.emplace_back(LoadInfo(header, strConmaBuf, indexData));
	}

    return result;
}


void CsvLoader::MakeIndex(const CsvHeader_t& header, const ConmaStr_t& strConmaBuf, CsvIndex_t& indexData)
{
	int index = 0;
	// 頭に書かれている文字列をすべて読み込む
	for (auto& headerName : strConmaBuf)
	{
		// 渡されたHeader情報を回す
		for (auto& header : header)
		{
			// 頭に書かれた文字列と渡されたHeaderの文字列が同じ
			if (headerName == header.first)
			{
				// 要素として追加
				indexData[headerName] = index;
				// 要素番号を次へ
				++index;
				// 次の頭の文字列へ
				break;
			}
		}
	}
}

CsvData_t CsvLoader::LoadInfo(const CsvHeader_t& header, const ConmaStr_t& strConmaBuf, const CsvIndex_t& indexData)
{
	// Header情報をすべて回す
	CsvData_t datas;
	for (const auto& info : header)
	{
		auto& data = datas[info.first];
		int index = indexData.at(info.first);

		// 型に合わせてデータを取得
		if (info.second == Type::kInt)
		{
			data.tInt = std::stoi(strConmaBuf[index]);
		}
		else if (info.second == Type::kFloat)
		{
			data.tFloat = std::stof(strConmaBuf[index]);
		}
		else if (info.second == Type::kDouble)
		{
			data.tDouble = std::stod(strConmaBuf[index]);
		}
		else if (info.second == Type::kWstr)
		{
			data.tWstr = strConmaBuf[index];
		}
	}
	return datas;
}

