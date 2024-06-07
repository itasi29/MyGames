#include "FileManager.h"
#include <DxLib.h>
#include <cassert>
#include <fstream>
#include <sstream>
#include "Utility/StringUtility.h"
#include "FileBase.h"
#include "FileImage.h"
#include "FileSound.h"
#include "FileModel.h"
#include "FilePixelShader.h"
#include "FileVertexShader.h"

namespace
{
	const wchar_t* const kFileMasterPath = L"Data/Master/FileMaster.csv";
	const wchar_t* const kTextureDataMasterPath = L"Data/Master/TextureDataMaster.csv";
	const enum FileIndex : int
	{
		Name,
		Type,
		Path,
		Extension
	};
	const enum TextureIndex : int
	{
		ModelName,
		SeetNum,
		TextureName
	};

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
}

FileManager& FileManager::GetInstance()
{
	static FileManager instance;
	return instance;
}

void FileManager::Init()
{
	LoadPath();
}

void FileManager::Final()
{
	for (auto& file : m_file)
	{
		file.second->Delete();
		file.second->End();
	}
}

void FileManager::SetTexture(int modelH, const std::wstring& name)
{
	// ���f���ɃZ�b�g����e�N�X�`���f�[�^���擾
	const auto& texData = m_texData.at(name);

	int texIdx = 0;
	for (const auto& texName : texData)
	{
		// �Z�b�g����e�N�X�`���̃t�@�C���f�[�^���擾
		const auto& file = m_file.at(texName);
		// �摜�̃n���h���擾
		int imgH = file->GetHandle();

		// �e�N�X�`���̕ύX
		MV1SetTextureGraphHandle(modelH, texIdx, imgH, false);

		// �e�N�X�`���ԍ��̍X�V
		texIdx++;
	}
}

std::shared_ptr<FileBase> FileManager::Load(const wchar_t* const name, bool isEternal)
{
	auto it = m_file.find(name);

	// ������ΎQ�ƃJ�E���^�𑝂₵�āA�R�s�[��Ԃ�
	if (it != m_file.end()) 
	{
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
	if (file->m_refCount <= 0)
	{
		m_file.erase(name);
		file->Delete();
		file->End();
	}
}

std::shared_ptr<FileBase> FileManager::MakeFile(const wchar_t* const name)
{
	auto type = m_path.at(name).type;

	if (type == LoadType::Image)
	{
		return std::make_shared<FileImage>();
	}
	else if (type == LoadType::Sound)
	{
		return std::make_shared<FileSound>();
	}
	else if (type == LoadType::Model) 
	{
		return std::make_shared<FileModel>();
	}
	else if (type == LoadType::PixelShader)
	{
		return std::make_shared<FilePixelShader>();
	}
	else if (type == LoadType::VertexShader)
	{
		return std::make_shared<FileVertexShader>();
	}

	// �����܂ŗ����炨������
	assert(false);
	// �����悤��FileGraph��Ԃ��Ă���
	return std::make_shared<FileImage>();
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

	if (type == LoadType::Image)
	{
		std::shared_ptr<FileImage> fileGraph = std::dynamic_pointer_cast<FileImage>(file);
		return std::make_shared<FileImage>(*fileGraph);	// �R�s�[�R���X�g���N�^
	}
	else if (type == LoadType::Sound)
	{
		std::shared_ptr<FileSound> fileSound = std::dynamic_pointer_cast<FileSound>(file);
		return std::make_shared<FileSound>(*fileSound);
	}
	else if (type == LoadType::Model) 
	{
		std::shared_ptr<FileModel> fileModel = std::dynamic_pointer_cast<FileModel>(file);
		return std::make_shared<FileModel>(*fileModel);
	}
	else if (type == LoadType::PixelShader)
	{
		std::shared_ptr<FilePixelShader> filePixelShader = std::dynamic_pointer_cast<FilePixelShader>(file);
		return std::make_shared<FilePixelShader>(*filePixelShader);
	}
	else if (type == LoadType::VertexShader)
	{
		std::shared_ptr<FileVertexShader> fileVertexShader = std::dynamic_pointer_cast<FileVertexShader>(file);
		return std::make_shared<FileVertexShader>(*fileVertexShader);
	}

	// �����܂ŗ����炨������
	assert(false);
	// �����悤��FileGraph��Ԃ��Ă���
	std::shared_ptr<FileImage> fileGraph = std::dynamic_pointer_cast<FileImage>(file);
	return std::make_shared<FileImage>(*fileGraph);
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

	if (type == LoadType::Image) 
	{
		handle = LoadGraph(path);
	}
	else if (type == LoadType::Sound)
	{
		handle = LoadSoundMem(path);
	}
	else if (type == LoadType::Model) 
	{
		handle = MV1LoadModel(path);
	}
	else if (type == LoadType::PixelShader)
	{
		handle = LoadPixelShader(path);
	}
	else if (type == LoadType::VertexShader)
	{
		handle = LoadVertexShader(path);
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
	// �t�@�C���ǂݍ���
	std::wifstream ifs(kFileMasterPath);
	if (!ifs)
	{
		assert(false);
		return;
	}

	std::wstring strBuf;
	std::vector<std::wstring> strConmaBuf;

	// �]���ȕ����̓ǂݍ���
	std::getline(ifs, strBuf);

	// �f�[�^���ׂēǂݍ���
	while (std::getline(ifs, strBuf))
	{
		strConmaBuf = StringUtility::Split(strBuf, ',');

		// ���O�擾
		const std::wstring& name = strConmaBuf[Name];

		// ��ގ擾
		const std::wstring& typeName = strConmaBuf[Type];
		LoadType type = LoadType::Image;
		if (typeName == L"Image")
		{
			type = LoadType::Image;
		}
		else if (typeName == L"Sound")
		{
			type = LoadType::Sound;
		}
		else if (typeName == L"Model")
		{
			type = LoadType::Model;
		}

		// �p�X�擾
		std::wostringstream path;
		path << L"Data/";
		if (type == LoadType::Image)
		{
			path << L"Image/";
		}
		else if (type == LoadType::Sound)
		{
			path << L"Sound/";
		}
		else if (type == LoadType::Model)
		{
			path << L"Model/";
		}
		path << strConmaBuf[Path] << L"." << strConmaBuf[Extension];

		auto& data = m_path[name];
		data.type = type;
		data.path = path.str();
	}
}
void FileManager::LoadSetTextureDate()
{
	// �t�@�C���ǂݍ���
	std::wifstream ifs(kTextureDataMasterPath);
	if (!ifs)
	{
		assert(false);
		return;
	}

	std::wstring strBuf;
	std::vector<std::wstring> strConmaBuf;

	// �]���ȕ����̓ǂݍ���
	std::getline(ifs, strBuf);

	// �f�[�^���ׂēǂݍ���
	while (std::getline(ifs, strBuf))
	{
		strConmaBuf = StringUtility::Split(strBuf, ',');

		// ���O�擾
		std::wstring& name = strConmaBuf[ModelName];

		// �f�[�^�̐���
		auto& data = m_texData[name];

		// �Z�b�g���擾
		int setNum = std::stoi(strConmaBuf[SeetNum]);

		// �Z�b�g����f�[�^�����ׂĎ擾
		for (int i = 0; i < setNum; i++)
		{
			// �f�[�^�̎擾
			std::wstring& texName = strConmaBuf[TextureName + i];

			// �ǉ�
			data.push_back(texName);
		}
	}
}
