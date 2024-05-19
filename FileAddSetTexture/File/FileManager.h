#pragma once
#include <memory>
#include <string>
#include <list>
#include <unordered_map>

class FileBase;

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

// �p�X�f�[�^
using PathData_t = std::unordered_map<std::wstring, Path>;
// ���[�h�ς݃f�[�^
using LoadedData_t = std::unordered_map<std::wstring, std::shared_ptr<FileBase>>;
// ���f���ɂ͂�f�[�^�Q
using TextureData_t = std::unordered_map<std::wstring, std::list<std::wstring>>;

/// <summary>
/// �t�@�C���Ǘ��N���X
/// </summary>
class FileManager
{
public:
	FileManager();
	~FileManager();

	// ����������
	void Init();
	// �I������
	void End();

	/// <summary>
	/// �f�[�^�̓ǂݍ���
	/// </summary>
	/// <param name="name">�ǂݍ��݂����f�[�^��</param>
	/// <param name="isEternal">true : �i�� / false : �g���Ă�����̂���������폜</param>
	/// <returns>�f�[�^</returns>
	std::shared_ptr<FileBase> Load(const wchar_t* const name, bool isEternal = false);
	/// <summary>
	/// ���f���f�[�^�Ƀe�N�X�`���f�[�^��\��t����
	/// </summary>
	/// <param name="modelH">���f���n���h��</param>
	/// <param name="name">�f�[�^��</param>
	void SetTexture(int modelH, const std::wstring& name);
	/// <summary>
	///	�f�[�^�̍폜
	/// </summary>
	/// <param name="name">���������f�[�^��</param>
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
	bool m_isEnd;

	PathData_t m_path;
	LoadedData_t m_file;
	TextureData_t m_texData;
};

