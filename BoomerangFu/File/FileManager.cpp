﻿#include "FileManager.h"

#include <DxLib.h>
#include <cassert>
#include <fstream>
#include <sstream>
#include "FileBase.h"
#include "FileImage.h"
#include "FileSound.h"
#include "FileModel.h"
#include "Utility/StringUtility.h"

namespace
{
	const wchar_t* const kFileMasterPath = L"Data/Master/FileMaster.csv";
	const wchar_t* const kTextureDataMasterPath = L"Data/Master/TextureDataMaster.csv";
	const enum FileIndex : int
	{
		kName,
		kType,
		kPath,
		kExtension
	};
	const enum TextureIndex : int
	{
		kModelName,
		kSeetNum,
		kTextureName
	};

	struct Header
	{
		float var = 1.0f;
		int size = 0;
	};
}

FileManager::FileManager() :
	m_isEnd(false)
{
}

FileManager::~FileManager()
{
	End();
}

FileManager& FileManager::GetInstance()
{
	static FileManager mgr;
	return mgr;
}

void FileManager::Init()
{
	m_isEnd = false;

	LoadPath();
	LoadSetTextureDate();
}

void FileManager::End()
{
	if (m_isEnd) return;

	for (auto& file : m_file)
	{
		file.second->Delete();
		file.second->End();
	}

	m_isEnd = true;
}

std::shared_ptr<FileBase> FileManager::Load(const wchar_t* const name, bool isEternal)
{
	auto it = m_file.find(name);

	// 見つかれば参照カウンタを増やして、コピーを返す
	if (it != m_file.end()) 
	{
		auto& file = it->second;
		file->m_refCount++;
		return CastCopyFile(file);
	}

	// 見つかなければ作成し、コピーを返す
	std::shared_ptr<FileBase> file = MakeFile(name);
	m_file.insert(std::make_pair(name, file));
	file->m_name = name;
	file->m_handle = GetHandle(name);
	file->m_refCount++;
	file->m_isEternal = isEternal;

	return CastCopyFile(file);
}

void FileManager::SetTexture(int modelH, const std::wstring& name)
{
	// モデルにセットするテクスチャデータを取得
	const auto& texData = m_texData.at(name);

	int texIdx = 0;
	for (const auto& texName : texData)
	{
		// セットするテクスチャのファイルデータを取得
		const auto& file = m_file.at(texName);
		// 画像のハンドル取得
		int imgH = file->GetHandle();

		// テクスチャの変更
		MV1SetTextureGraphHandle(modelH, texIdx, imgH, false);

		// テクスチャ番号の更新
		texIdx++;
	}
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
	if (file->m_refCount <= 0) 
	{
		file->Delete();
		file->End();
		m_file.erase(name);
	}
}

/// <summary>
/// タイプに合わせてクラスを作成
/// </summary>
/// <param name="name">ファイル名</param>
/// <returns>Fileクラス</returns>
std::shared_ptr<FileBase> FileManager::MakeFile(const wchar_t* const name)
{
	auto type = m_path.at(name).type;

	if (type == LoadType::kImage)
	{
		return std::make_shared<FileImage>(*this);
	}
	else if (type == LoadType::kSound) 
	{
		return std::make_shared<FileSound>(*this);
	}
	else if (type == LoadType::kModel) 
	{
		return std::make_shared<FileModel>(*this);
	}

	// ここまで来たらおかしい
	assert(false);
	// 動くようにFileGraphを返しておく
	return std::make_shared<FileImage>(*this);
}

/// <summary>
/// タイプに合わせてクラスをキャスト・コピーを返す
/// </summary>
/// <param name="file">ベース</param>
/// <param name="name">名前</param>
/// <returns>コピー</returns>
std::shared_ptr<FileBase> FileManager::CastCopyFile(std::shared_ptr<FileBase>& file)
{
	auto type = m_path.at(file->m_name).type;

	if (type == LoadType::kImage)
	{
		std::shared_ptr<FileImage> fileGraph = std::dynamic_pointer_cast<FileImage>(file);
		return std::make_shared<FileImage>(*fileGraph);
	}
	else if (type == LoadType::kSound) 
	{
		std::shared_ptr<FileSound> fileSound = std::dynamic_pointer_cast<FileSound>(file);
		return std::make_shared<FileSound>(*fileSound);
	}
	else if (type == LoadType::kModel) 
	{
		std::shared_ptr<FileModel> fileModel = std::dynamic_pointer_cast<FileModel>(file);
		return std::make_shared<FileModel>(*fileModel);
	}

	// ここまで来たらおかしい
	assert(false);
	// 動くようにFileGraphを返しておく
	std::shared_ptr<FileImage> fileGraph = std::dynamic_pointer_cast<FileImage>(file);
	return std::make_shared<FileImage>(*fileGraph);
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

	if (type == LoadType::kImage) 
	{
		handle = LoadGraph(path);
	}
	else if (type == LoadType::kSound) 
	{
		handle = LoadSoundMem(path);
	}
	else if (type == LoadType::kModel) 
	{
		handle = MV1LoadModel(path);
	}
	else 
	{
		assert(false);
	}
	assert(handle > 0);

	return handle;
}

void FileManager::LoadPath()
{
	// ファイル読み込み
	std::wifstream ifs(kFileMasterPath);
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
		strConmaBuf = StringUtility::Split(strBuf, ',');

		// 名前取得
		const std::wstring& name = strConmaBuf[kName];

		// 種類取得
		const std::wstring& typeName = strConmaBuf[kType];
		LoadType type = LoadType::kImage;
		if (typeName == L"Image") 
		{
			type = LoadType::kImage;
		}
		else if (typeName == L"Sound") 
		{
			type = LoadType::kSound;
		}
		else if (typeName == L"Model") 
		{
			type = LoadType::kModel;
		}

		// パス取得
		std::wostringstream path;
		path << L"Data/";
		if (type == LoadType::kImage) 
		{
			path << L"Image/";
		}
		else if (type == LoadType::kSound)
		{
			path << L"Sound/";
		}
		else if (type == LoadType::kModel)
		{
			path << L"Model/";
		}
		path << strConmaBuf[kPath] << L"." << strConmaBuf[kExtension];

		auto& data = m_path[name];
		data.type = type;
		data.path = path.str();
	}
}

void FileManager::LoadSetTextureDate()
{
	// ファイル読み込み
	std::wifstream ifs(kTextureDataMasterPath);
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
		strConmaBuf = StringUtility::Split(strBuf, ',');

		// 名前取得
		std::wstring& name = strConmaBuf[kModelName];

		// データの生成
		auto& data = m_texData[name];

		// セット数取得
		int setNum = std::stoi(strConmaBuf[kSeetNum]);

		// セットするデータをすべて取得
		for (int i = 0; i < setNum; i++)
		{
			// データの取得
			std::wstring& texName = strConmaBuf[kTextureName + i];

			// 追加
			data.push_back(texName);
		}
	}
}

