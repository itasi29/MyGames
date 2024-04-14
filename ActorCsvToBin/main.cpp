#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

namespace
{
	const char* const kInPath = "ActorMaster.csv";
	const char* const kOutPath = "ActorMaster.bin";

	enum Index : int {
		kName,
		kSpeed,
		kJump,
		kWidth,
		kHeight,
		kScale
	};

	struct ActorData {
		float speed = 0.0f;
		float jump = 0.0f;
		float width = 0.0f;
		float height = 0.0f;
		float scale = 0.0f;
	};

    constexpr float kVar = 1.0f;
    struct Header
    {
        float var = kVar;
        int size = 0;
    };
}

using Data_t = std::unordered_map<std::wstring, ActorData>;

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

    // データすべて読み込み
    while (std::getline(ifs, strBuf)) {
        strConmaBuf = Split(strBuf, ',');

        // 名前取得
        std::wstring& name = strConmaBuf[kName];

        // 各種パラメータ取得
        float speed = std::stof(strConmaBuf[kSpeed]);
        float jump = std::stof(strConmaBuf[kJump]);
        float width = std::stof(strConmaBuf[kWidth]);
        float height = std::stof(strConmaBuf[kHeight]);
        float scale = std::stof(strConmaBuf[kScale]);
        
        // 保存
        auto& ref = data[name];
        ref.speed = speed;
        ref.jump = jump;
        ref.width = width;
        ref.height = height;
        ref.scale = scale;
    }
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

    for (const auto& ref : data) {
        auto& name = ref.first;
        // 名前の保存
        int size = static_cast<int>(name.size());
        fwrite(&size, sizeof(size), 1, fp);
        fwrite(name.data(), size * sizeof(wchar_t), 1, fp);

        // データ保存
        fwrite(&ref.second, sizeof(ref.second), 1, fp);
    }

    fclose(fp);
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