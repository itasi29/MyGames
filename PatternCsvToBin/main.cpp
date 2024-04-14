#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

namespace
{
	const char* const kInPath = "PatternMaster.csv";
	const char* const kOutPath = "PatternMaster.bin";

	enum Index : int {
		kNo,
		kCreateNum,
		kCreateFrame,
		kSpeedMul,
		kCreatePos
	};

	struct CreateData {
		int createFrame = 0;
		float speedMul = 0.0f;
		float createPos = 0.0f;
	};
	struct PatternData {
		int createNum = 0;
		std::vector<CreateData> data;
	};

	constexpr float kVar = 1.0f;
	struct Header
	{
		float var = kVar;
		int size = 0;
	};
}

using Data_t = std::unordered_map<int, PatternData>;

void LoadCsv(Data_t& data);
void WriteBin(Data_t& data);
std::vector<std::wstring> Split(const std::wstring& str, const char del);

int main()
{
	Data_t data;
	LoadCsv(data);
	WriteBin(data);

	return 0;
}

void LoadCsv(Data_t& data)
{
	// ファイル読み込み
	std::wifstream ifs(kInPath);
	if (!ifs) {
		assert(false);
		return;
	}

	std::wstring strBuf;
	std::vector<std::wstring> strConmaBuf;

	// 余分な部分の読み込み
	std::getline(ifs, strBuf);

	// データ確認用
	int count;
	bool isNextGet = true;
	// データ保存用
	int no;
	int createNum;

	// データすべて読み込み
	while (std::getline(ifs, strBuf)) {
		strConmaBuf = Split(strBuf, ',');

		if (isNextGet) {
			// 番号取得
			no = std::stoi(strConmaBuf[kNo]);

			// 生成数取得
			createNum = std::stoi(strConmaBuf[kCreateNum]);
			data[no].createNum = createNum;

			// カウント初期化
			count = 0;
			// 次データをとらないように
			isNextGet = false;
		}

		// 各種パラメータ取得
		CreateData temp;
		temp.createFrame = std::stoi(strConmaBuf[kCreateFrame]);
		temp.speedMul = std::stof(strConmaBuf[kSpeedMul]);
		temp.createPos = std::stof(strConmaBuf[kCreatePos]);

		// パラメータ代入
		data[no].data.push_back(temp);

		// 次データ移行確認
		++count;
		if (count >= createNum) {
			isNextGet = true;
		}
	}

	return;
}

void WriteBin(Data_t& data)
{
	FILE* fp;

	// ファイル読み込み
	auto err = fopen_s(&fp, kOutPath, "wb");
	if (err != 0) {
		assert(false);
		return;
	}

	Header header;
	header.var = kVar;
	header.size = static_cast<int>(data.size());
	fwrite(&header, sizeof(header), 1, fp);

	for (auto& ref : data) {
		// 番号の保存
		auto no = ref.first;
		fwrite(&no, sizeof(no), 1, fp);

		auto pattern = ref.second;
		// 生成数の保存
		fwrite(&pattern.createNum, sizeof(pattern.createNum), 1, fp);

		// 生成データの保存
		fwrite(pattern.data.data(), sizeof(CreateData) * pattern.createNum, 1, fp);
	}

	fclose(fp);

	return;
}

std::vector<std::wstring> Split(const std::wstring& str, const char del)
{
	// 区切り開始位置
	int first = 0;
	// 区切り最終位置
	int last = static_cast<int>(str.find_first_of(del));

	std::vector<std::wstring> result;

	while (first < str.size()) {
		// 区切ったものを結果に入れていく
		std::wstring subStr(str, first, last - first);

		result.push_back(subStr);

		// 位置更新
		first = last + 1;
		last = static_cast<int>(str.find_first_of(del, first));
		if (last == std::wstring::npos) {
			last = static_cast<int>(str.size());
		}
	}

	return result;
}