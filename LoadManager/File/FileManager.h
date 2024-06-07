#pragma once
#include <memory>
#include <string>
#include <list>
#include <unordered_map>

class FileBase;

enum class LoadType
{
	Image,
	Sound,
	Model,
	PixelShader,
	VertexShader
};

struct PathData
{
	LoadType type = LoadType::Image;
	std::wstring path;
};

// パスデータ
using PathData_t = std::unordered_map<std::wstring, PathData>;
// ロード済みデータ
using LoadedData_t = std::unordered_map<std::wstring, std::shared_ptr<FileBase>>;
// モデルにはるデータ群
using TextureData_t = std::unordered_map<std::wstring, std::list<std::wstring>>;

/// <summary>
/// ファイル管理クラス
/// </summary>
class FileManager
{
private:
	FileManager();
	~FileManager();

	FileManager(const FileManager&) = delete;
	void operator=(const FileManager&) = delete;

public:
	static FileManager& GetInstance();

	void Init();
	void Final();

	void SetTexture(int modelH, const std::wstring& name);

	std::shared_ptr<FileBase> Load(const wchar_t* const name, bool isEternal = false);
	void Delete(const wchar_t* const name);

private:
	// ファイルの生成
	std::shared_ptr<FileBase> MakeFile(const wchar_t* const name);
	// ファイルのコピー
	std::shared_ptr<FileBase> CastCopyFile(std::shared_ptr<FileBase>& data);
	// ハンドルの取得
	int GetHandle(const wchar_t* name) const;

	// パスデータのロード
	void LoadPath();
	// テクスチャ貼り付けデータのロード
	void LoadSetTextureDate();

private:
	PathData_t m_path;
	LoadedData_t m_file;
	TextureData_t m_texData;
};

