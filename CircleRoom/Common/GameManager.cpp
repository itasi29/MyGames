#include "GameManager.h"
#include <DxLib.h>
#include <cassert>

#include "Scene/SceneManager.h"
#include "Stage/StageManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/FontSystem.h"

GameManager::GameManager() :
	m_file(std::make_shared<FileManager>()),
	m_stage(std::make_shared<StageManager>()),
	m_scene(std::make_shared<SceneManager>()),
	m_sound(std::make_shared<SoundSystem>()),
	m_font(std::make_shared<FontSystem>())
{
	// �����f�[�^
	m_data.playTime = 0;
	m_data.deathCount = 0;
	m_data.dashCount = 0;
	m_data.volume = { 255, 255 };
	Load();
}

GameManager::~GameManager()
{
	m_file->End();
	m_sound->End();

	Save();
}

void GameManager::Init()
{
	m_stage->Init();
	m_sound->Init();
	m_font->Init();
	m_scene->Init();
}

void GameManager::DeleteData()
{
	// �v���C�f�[�^�̏�����
	m_data.playTime = 0;
	m_data.deathCount = 0;
	m_data.dashCount = 0;	
}

GameManager& GameManager::GetInstance()
{
	static GameManager instance;
	return instance;
}

std::shared_ptr<FileManager>& GameManager::GetFile()
{
	return m_file;
}

std::shared_ptr<StageManager>& GameManager::GetStage()
{
	return m_stage;
}

std::shared_ptr<SceneManager>& GameManager::GetScene()
{
	return m_scene;
}

std::shared_ptr<SoundSystem>& GameManager::GetSound()
{
	return m_sound;
}

std::shared_ptr<FontSystem>& GameManager::GetFont()
{
	return m_font;
}

AccountData GameManager::GetData() const
{
	return m_data;
}

void GameManager::Save()
{
	FILE* fp = nullptr;
	auto err = fopen_s(&fp, "Data/Bin/sdt.inf", "wb");
	if (err != errno)
	{
		// �ǂݍ��݂Ɏ��s�������ߏI��
		assert(false);
		return;
	}

	// �A�J�E���g�f�[�^�̏�������
	fwrite(&m_data, sizeof(m_data), 1, fp);

	fclose(fp);
}

void GameManager::Load()
{
	auto handle = FileRead_open(L"Data/Bin/sdt.inf");
	// �G���[(�t�@�C�����Ȃ�)�ꍇ�͏������Ȃ�
	if (handle == 0)
	{
		return;
	}

	// �A�J�E���g�f�[�^�̓ǂݍ���
	FileRead_read(&m_data, sizeof(m_data), handle);

	FileRead_close(handle);
}
