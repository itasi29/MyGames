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

// �p�X�f�[�^
using PathData_t = std::unordered_map<std::wstring, PathData>;
// ���[�h�ς݃f�[�^
using LoadedData_t = std::unordered_map<std::wstring, std::shared_ptr<FileBase>>;
// ���f���ɂ͂�f�[�^�Q
using TextureData_t = std::unordered_map<std::wstring, std::list<std::wstring>>;

/// <summary>
/// �t�@�C���Ǘ��N���X
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
	// �t�@�C���̐���
	std::shared_ptr<FileBase> MakeFile(const wchar_t* const name);
	// �t�@�C���̃R�s�[
	std::shared_ptr<FileBase> CastCopyFile(std::shared_ptr<FileBase>& data);
	// �n���h���̎擾
	int GetHandle(const wchar_t* name) const;

	// �p�X�f�[�^�̃��[�h
	void LoadPath();
	// �e�N�X�`���\��t���f�[�^�̃��[�h
	void LoadSetTextureDate();

private:
	PathData_t m_path;
	LoadedData_t m_file;
	TextureData_t m_texData;
};

