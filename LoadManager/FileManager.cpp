#include "FileManager.h"

#include <DxLib.h>
#include <cassert>
#include "FileBase.h"
#include "FileGraph.h"
#include "FileSound.h"
#include "FileModel.h"

namespace
{
	struct Header
	{
		float var = 1.0f;
		int size = 0;
	};
}

FileManager::FileManager()
{
	LoadPath();
}

FileManager::~FileManager()
{
	for (auto& file : m_file)
	{
		file.second->Delete();
	}
}

std::shared_ptr<FileBase> FileManager::Load(const wchar_t* name, bool isEternal)
{
	auto it = m_file.find(name);

	// 見つかれば参照カウンタを増やして、コピーを返す
	if (it != m_file.end()) {
		auto& file = it->second;
		file->m_refCount++;
		return MakeFile(file);
	}

	// 見つかなければ作成し、コピーを返す
	std::shared_ptr<FileBase> file;
	file->m_name = name;
	file->m_handle = GetHandle(name);
	file->m_refCount++;
	file->m_isEternal = isEternal;
	return MakeFile(file);
}

void FileManager::Delete(const wchar_t* const name)
{
	// 見つかなければ終了
	if (m_file.find(name) == m_file.end()) return;

	auto& file = m_file.at(name);
	// 永続フラグがtrueなら終了
	if (file->m_isEternal) return;

	// 参照カウンタを減らす
	file->m_refCount--;
	// 見ているものが無くなればデータを消す
	if (file->m_refCount <= 0) {
		m_file.erase(name);
		file->Delete();
		file->End();
	}
}

/// <summary>
/// タイプに合わせてクラスをキャスト・コピーを返す
/// </summary>
/// <param name="file">ベース</param>
/// <param name="name">名前</param>
/// <returns>コピー</returns>
std::shared_ptr<FileBase> FileManager::MakeFile(std::shared_ptr<FileBase>& file)
{
	auto type = m_path.at(file->m_name).type;

	if (type == LoadType::kImage) {
		std::shared_ptr<FileGraph> fileGraph = std::dynamic_pointer_cast<FileGraph>(file);
		return std::make_shared<FileGraph>(*fileGraph);
	}
	else if (type == LoadType::kSoudn) {
		std::shared_ptr<FileSound> fileSound = std::dynamic_pointer_cast<FileSound>(file);
		return std::make_shared<FileSound>(*fileSound);
	}
	else if (type == LoadType::kModel) {
		std::shared_ptr<FileModel> fileModel = std::dynamic_pointer_cast<FileModel>(file);
		return std::make_shared<FileModel>(*fileModel);
	}
	else {
		assert(false);
		return std::make_shared<FileBase>(*file);
	}
}

/// <summary>
/// ハンドルの入手
/// タイプに合わせてロードの仕方を変更
/// </summary>
/// <param name="name">名前</param>
/// <returns>ハンドル</returns>
int FileManager::GetHandle(const wchar_t* name) const
{
	int handle = -1;
	auto path = m_path.at(name).path.c_str();
	auto type = m_path.at(name).type;

	if (type == LoadType::kImage) {
		handle = LoadGraph(path);
	}
	else if (type == LoadType::kSoudn) {
		handle = LoadSoundMem(path);
	}
	else if (type == LoadType::kModel) {
		handle = MV1LoadModel(path);
	}
	else {
		assert(false);
	}
	assert(handle > 0);

	return handle;
}

void FileManager::LoadPath()
{
	int handle = FileRead_open(L"Data/Bin/DataManager.bin");

	if (handle == 0) {
		assert(false);
		return;
	}

	Header header;
	FileRead_read(&header, sizeof(header), handle);

	size_t size;
	std::wstring name;
	LoadType type;
	std::wstring path;
	for (int i = 0; i < header.size; i++) {
		// 名前の取得
		FileRead_read(&size, sizeof(size), handle);
		name.resize(size);
		FileRead_read(name.data(), size, handle);
		// 種類の取得
		FileRead_read(&type, sizeof(type), handle);
		// パスの取得
		FileRead_read(&size, sizeof(size), handle);
		path.resize(size);
		FileRead_read(path.data(), size, handle);
	}

	
	FileRead_close(handle);
}
