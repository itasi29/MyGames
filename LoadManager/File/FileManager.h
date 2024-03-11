#pragma once
#include <memory>
#include <string>
#include <list>
#include <unordered_map>

class FileBase;

enum class LoadType
{
	kImage,
	kSoudn,
	kModel
};

struct Path
{
	LoadType type;
	std::wstring path;
};

// �p�X�f�[�^
using PathData_t = std::unordered_map<std::wstring, Path>;
// �ꊇ���[�h�f�[�^�Q
using BatchLoadData_t = std::unordered_map<std::wstring, std::list<std::wstring>>;
// ���[�h�ς݃f�[�^
using LoadedData_t = std::unordered_map<std::wstring, std::shared_ptr<FileBase>>;

/// <summary>
/// �t�@�C���Ǘ��N���X
/// </summary>
class FileManager
{
public:
	FileManager();
	~FileManager();

	std::shared_ptr<FileBase> Load(const wchar_t* const name, bool isEternal = false);
	void Delete(const wchar_t* const name);

private:
	std::shared_ptr<FileBase> MakeFile(const wchar_t* const name);
	std::shared_ptr<FileBase> CastCopyFile(std::shared_ptr<FileBase>& data);
	int GetHandle(const wchar_t* name) const;

	void LoadPath();

private:
	PathData_t m_path;
	LoadedData_t m_file;
};

