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
}

FileManager::~FileManager()
{
	for (auto& file : m_file)
	{
		file.second->Delete();
	}
}

std::shared_ptr<FileBase> FileManager::Load(const wchar_t* const name, bool isEternal)
{
	auto it = m_file.find(name);

	// ������ΎQ�ƃJ�E���^�𑝂₵�āA�R�s�[��Ԃ�
	if (it != m_file.end()) {
		auto& file = it->second;
		file->m_refCount++;
		return CastCopyFile(file);
	}

	// �����Ȃ���΍쐬���A�R�s�[��Ԃ�
	std::shared_ptr<FileBase> file = MakeFile(name);
	file->m_name = name;
	file->m_handle = GetHandle(name);
	file->m_refCount++;
	file->m_isEternal = isEternal;
	return CastCopyFile(file);
}

void FileManager::Delete(const wchar_t* const name)
{
	// �����Ȃ���ΏI��
	if (m_file.find(name) == m_file.end()) return;

	auto& file = m_file.at(name);
	// �i���t���O��true�Ȃ�I��
	if (file->m_isEternal) return;

	// �Q�ƃJ�E���^�����炷
	file->m_refCount--;
	// ���Ă�����̂������Ȃ�΃f�[�^������
	if (file->m_refCount <= 0) {
		m_file.erase(name);
		file->Delete();
		file->End();
	}
}

std::shared_ptr<FileBase> FileManager::MakeFile(const wchar_t* const name)
{
	auto type = m_path.at(name).type;

	if (type == LoadType::kImage) {
		return std::make_shared<FileGraph>(*this);
	}
	else if (type == LoadType::kSoudn) {
		return std::make_shared<FileSound>(*this);
	}
	else if (type == LoadType::kModel) {
		return std::make_shared<FileModel>(*this);
	}

	// �����܂ŗ����炨������
	assert(false);
	// �����悤��FileGraph��Ԃ��Ă���
	return std::make_shared<FileGraph>(*this);
}

/// <summary>
/// �^�C�v�ɍ��킹�ăN���X���L���X�g�E�R�s�[��Ԃ�
/// </summary>
/// <param name="file">�x�[�X</param>
/// <param name="name">���O</param>
/// <returns>�R�s�[</returns>
std::shared_ptr<FileBase> FileManager::CastCopyFile(std::shared_ptr<FileBase>& file)
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

	// �����܂ŗ����炨������
	assert(false);
	// �����悤��FileGraph��Ԃ��Ă���
	std::shared_ptr<FileGraph> fileGraph = std::dynamic_pointer_cast<FileGraph>(file);
	return std::make_shared<FileGraph>(*fileGraph);
}

/// <summary>
/// �n���h���̓���
/// �^�C�v�ɍ��킹�ă��[�h�̎d����ύX
/// </summary>
/// <param name="name">���O</param>
/// <returns>�n���h��</returns>
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
	int handle = FileRead_open(L"DataManager.bin");

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
		// ���O�̎擾
		FileRead_read(&size, sizeof(size), handle);
		name.resize(size);
		FileRead_read(name.data(), static_cast<int>(size), handle);
		// ��ނ̎擾
		FileRead_read(&type, sizeof(type), handle);
		// �p�X�̎擾
		FileRead_read(&size, sizeof(size), handle);
		path.resize(size);
		FileRead_read(path.data(), static_cast<int>(size), handle);
	}

	
	FileRead_close(handle);
}
