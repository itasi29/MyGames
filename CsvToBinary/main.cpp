#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

namespace
{
    enum Index : int
    {
        kName,
        kType,
        kPath,
        kExtension
    };

    enum class LoadType
    {
        kImage,
        kSound,
        kModel
    };
    struct Path
    {
        LoadType type = LoadType::kImage;
        std::wstring path = L"";
    };

    constexpr float kVar = 1.0f;
    struct Header
    {
        float var = kVar;
        int size = 0;
    };
}


// パスデータ
using PathData_t = std::unordered_map<std::wstring, Path>;

void LoadCsv(PathData_t& data);
void WriteBin(PathData_t& data);
std::vector<std::wstring> Split(const std::wstring& str, const char del);

int main()
{
    PathData_t datas;
    LoadCsv(datas);
    WriteBin(datas);

	return 0;
}

void LoadCsv(PathData_t& datas)
{
    std::string path;
    std::cin >> path;

    // ファイル読み込み
    std::wifstream ifs(path.c_str());
    if (!ifs)
    {
        assert(false);
        return;
    }

    std::wstring strBuf;
    std::vector<std::wstring> strConmaBuf;

    // 余分な部分の読み込み
    std::getline(ifs, strBuf);

    // データすべて読み込み
    while (std::getline(ifs, strBuf))
    {
        strConmaBuf = Split(strBuf, ',');

        // 名前取得
        std::wstring& name = strConmaBuf[kName];

        // 種類取得
        std::wstring& typeName = strConmaBuf[kType];
        LoadType type = LoadType::kImage;
        if (typeName == L"Image") {
            type = LoadType::kImage;
        }
        else if (typeName == L"Sound") {
            type = LoadType::kSound;
        }
        else if (typeName == L"Model") {
            type = LoadType::kModel;
        }

        // パス取得
        std::wostringstream path;
        path << L"Data/";
        if (type == LoadType::kImage) {
            path << L"Image/";
        }
        else if (type == LoadType::kSound) {
            path << L"Sound/";
        }
        else if (type == LoadType::kModel) {
            path << L"Model/";
        }
        path << strConmaBuf[kPath] << L"." << strConmaBuf[kExtension];

        auto& data = datas[name];
        data.type = type;
        data.path = path.str();
    }
}

void WriteBin(PathData_t& datas)
{
    FILE* fp;

    // ファイル読み込み
    auto err = fopen_s(&fp, "DataManager.bin", "wb");
    if (err != 0)
    {
        assert(false);
        return;
    }

    Header header;
    header.var = kVar;
    header.size = datas.size();
    fwrite(&header, sizeof(header), 1, fp);

    for (const auto& data : datas)
    {
        auto& name = data.first;
        // 名前の保存
        size_t size = name.size();
        fwrite(&size, sizeof(size), 1, fp);
        fwrite(name.c_str(), size, 1, fp);

        // 種類の保存
        auto& type = data.second.type;
        fwrite(&(type), sizeof(type), 1, fp);
        // パスの保存
        auto& path = data.second.path;
        size = path.size();
        fwrite(&size, sizeof(size), 1, fp);
        fwrite(path.c_str(), size, 1, fp);
    }

    fclose(fp);
}

std::vector<std::wstring> Split(const std::wstring& str, const char del)
{
    // 区切り開始位置
    int first = 0;
    // 区切り最終位置
    int last = str.find_first_of(del);

    std::vector<std::wstring> result;

    while (first < str.size())
    {
        // 区切ったものを結果に入れていく
        std::wstring subStr(str, first, last - first);

        result.push_back(subStr);

        // 位置更新
        first = last + 1;
        last = str.find_first_of(del, first);
        if (last == std::wstring::npos)
        {
            last = str.size();
        }
    }

    return result;
}