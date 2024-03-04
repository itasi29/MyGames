#include "GameData.h"

#include <DxLib.h>
#include <cassert>

#include "StringUtility.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "Scene/OneShotScene.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"

namespace
{
	constexpr float kVersion = 1.3f;

	// �t�@�C���p
	struct StageInfHeader
	{
		const char* const id = "sti"; // �Ō��'\0'�����Ă�̂�4�o�C�g
		float version = kVersion;
		size_t dataCount = 0;
	};

	const char* const kPath = "Data/Bin/stg.inf";
}

GameData::GameData() :
	m_ability(Ability::kNone),
	m_isInBoss(false),
	m_killedEnemyTypeNum(0)
{
}

GameData::~GameData()
{
	Save();
}

void GameData::Init()
{
	// �摜�ǂݍ���
	auto& mgr = GameManager::GetInstance().GetFile();
	m_explanationDash = mgr->LoadGraphic(L"UI/dashExplanation.png");

	// �f�[�^������
	InitData();
	// �f�[�^�ǂݍ���
	Load();
}

void GameData::InitData()
{
	m_saveData.clear();
	m_killedEnemyNameData.clear();
	m_killedEnemyTypeNum = 0;
	m_clearBossData.clear();
	m_ability = Ability::kNone;
	m_isActiveAbilitys.clear();
	m_isInBoss = false;
}

int GameData::GetBestTime(const std::string& name) const
{
	auto it = m_saveData.find(name);
	// �X�e�[�W���������Ȃ�������0��Ԃ�
	if (it == m_saveData.end())
	{
		return 0;
	}

	// ����������x�X�g�^�C����Ԃ�
	return m_saveData.at(name).bestTime;
}

bool GameData::IsClearStage(const std::string& name)
{
	auto it = m_saveData.find(name);
	// �X�e�[�W���������Ȃ�������0��Ԃ�
	if (it == m_saveData.end())
	{
		CreateData(name);
		return false;
	}

	// ����������N���A����Ԃ�
	return m_saveData.at(name).isClear;
}

bool GameData::IsClearBoss(const std::string& name) const
{
	for (const auto& boss : m_clearBossData)
	{
		// ���O����v������N���A�������Ƃ�����
		if (boss == name)
		{
			return true;
		}
	}

	// ��v���Ȃ�������N���A�������Ƃ��Ȃ�
	return false;
}

bool GameData::IsKilledEnemy(const std::string& name) const
{
	for (const auto& killedName : m_killedEnemyNameData)
	{
		// ��v����ΎE���ꂽ���Ƃ�����
		if (killedName == name)
		{
			return true;
		}
	}

	// ��v���Ȃ�����ΎE���ꂽ���Ƃ��Ȃ�
	return false;
}

void GameData::CreateData(const std::string& name)
{
	auto it = m_saveData.find(name);
	// �X�e�[�W���������牽�����Ȃ�
	if (it != m_saveData.end())
	{
		return;
	}

	// �Ȃ���΍쐬����
	auto& data = m_saveData[name];

	// ���̏�����
	data.bestTime = 0;
	data.isClear = false;
}

void GameData::SaveClearStage(const std::string& name)
{
	auto it = m_saveData.find(name);
	// �X�e�[�W���������Ȃ������牽�����Ȃ�
	if (it == m_saveData.end())
	{
		assert(false);
		return;
	}

	// �w��̂��̂��N���A�Ƃ���
	m_saveData[name].isClear = true;
}


void GameData::SaveClearBoss(const std::string& name)
{
	for (const auto& boss : m_clearBossData)
	{
		// �N���A�������Ƃ�����Ή������Ȃ�
		if (boss == name)
		{
			return;
		}
	}

	// ��񂪏���Ă��Ȃ��ꍇ�͒ǉ�����
	m_clearBossData.push_back(name);
	return;
}

void GameData::SaveKilledEnemyType(const std::string& name)
{
	// �z��S���J��Ԃ�
	for (const auto& tableName : m_killedEnemyNameData)
	{
		// ���݂̃e�[�u���̖��O�ƎE�����G�̖��O�������ꍇ�͈�x�E�������Ƃ����邩��
		// ���̏������I������
		if (tableName == name)
		{
			return;
		}
	}

	// �����܂ŗ�����E�������Ƃ��Ȃ������G�ł���
	// ��ސ��J�E���g�̑���
	m_killedEnemyTypeNum++;
	// �e�[�u���ɓo�^
	m_killedEnemyNameData.push_back(name);

	// �A�r���e�B�̗L����
	if (name == "Dash")
	{
		m_isActiveAbilitys[Ability::kDash] = true;

		// �_�b�V�������摜
		auto& mgr = GameManager::GetInstance();
		mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(mgr, m_explanationDash->GetHandle()));
	}
}

bool GameData::SaveBestTime(const std::string& name, int bestTime)
{
	auto it = m_saveData.find(name);
	// �X�e�[�W���������Ȃ������牽�����Ȃ�
	if (it == m_saveData.end())
	{
		assert(false);
		return false;
	}

	auto& time = m_saveData[name].bestTime;

	// ���ݕۑ�����Ă���^�C�����X�V�^�C�����傫����΍X�V�͍s��Ȃ�
	if (time > bestTime)
	{
		return false;
	}

	// �^�C���̍X�V
	time = bestTime;
	return true;
}

void GameData::ChangeAbility(Ability ability)
{
	// �A�r���e�B���L���ɂȂ��Ă�����ύX������
	if (m_isActiveAbilitys[ability])
	{
		m_ability = ability;
	}
}

void GameData::Save()
{
	// �t�@�C���|�C���^
	FILE* fp = nullptr;
	auto err = fopen_s(&fp, kPath, "wb");
	if (err != 0)
	{
		assert(false);
		return;
	}
	StageInfHeader header;
	header.dataCount = m_saveData.size();
	fwrite(&header, sizeof(header), 1, fp);

	// �N���A�f�[�^�̏�������
	for (const auto& stage : m_saveData)
	{
		// �X�e�[�W��������̃T�C�Y���擾
		const auto& stageStr = stage.first;
		uint8_t size = static_cast<uint8_t>(stageStr.size());
		// �X�e�[�W��������̃o�C�g������������
		fwrite(&size, sizeof(size), 1, fp);
		// ������̏�������
		fwrite(stageStr.data(), stageStr.size(), 1, fp);

		// �f�[�^���̏�������
		fwrite(&stage.second, sizeof(stage.second), 1, fp);
	}

	// �f�[�^�{�̂���������ł���
	// �E���ꂽ�G�̎�ސ��̏�������
	fwrite(&m_killedEnemyTypeNum, sizeof(m_killedEnemyTypeNum), 1, fp);
	// ���O�f�[�^�̏�������
	for (const auto& name : m_killedEnemyNameData)
	{
		// ���O�̕����񐔂��擾
		uint8_t nameSize = static_cast<uint8_t>(name.size());
		// �����񐔂���������
		fwrite(&nameSize, sizeof(nameSize), 1, fp);
		// ������̏�������
		fwrite(name.data(), name.size(), 1, fp);
	}

	// �{�X�N���A���̏�������
	uint8_t size = static_cast<uint8_t>(m_clearBossData.size());
	fwrite(&size, sizeof(size), 1, fp);
	// �f�[�^�̏�������
	for (const auto& name : m_clearBossData)
	{
		uint8_t nameSize = static_cast<uint8_t>(name.size());
		fwrite(&nameSize, sizeof(nameSize), 1, fp);
		fwrite(name.data(), name.size(), 1, fp);
	}

	// �A�r���e�B�̏�������
	fwrite(&m_ability, sizeof(m_ability), 1, fp);

	// �A�r���e�B�̗L���e�[�u���̏�������
	size = static_cast<uint8_t>(m_isActiveAbilitys.size());
	fwrite(&size, sizeof(size), 1, fp);
	for (const auto& active : m_isActiveAbilitys)
	{
		// �A�r���e�B�^�C�v��������
		fwrite(&active.first, sizeof(active.first), 1, fp);
		// �L�����ǂ�����������
		fwrite(&active.second, sizeof(active.second), 1, fp);
	}

	// �{�X�X�e�[�W�ɓ��������Ƃ��邩�̏��
	fwrite(&m_isInBoss, sizeof(m_isInBoss), 1, fp);

	fclose(fp);
}

void GameData::Load()
{
	const auto& wPath = StringUtility::StringToWString(kPath);

	auto handle = FileRead_open(wPath.c_str());
	// �G���[(�t�@�C�����Ȃ�)�ꍇ�͏������Ȃ�
	if (handle == 0)
	{
		return;
	}
	// �w�b�_���̓ǂݍ���
	StageInfHeader header;
	FileRead_read(&header, sizeof(header), handle);

	if (header.version != kVersion)
	{
		FileRead_close(handle);
		return;
	}

	// �f�[�^�̓ǂݍ���
	for (int i = 0; i < header.dataCount; i++)
	{
		// �X�e�[�W��������T�C�Y������ϐ�
		uint8_t stgStrSize = 0;
		// �X�e�[�W��������T�C�Y�ǂݍ���
		FileRead_read(&stgStrSize, sizeof(stgStrSize), handle);
		// �X�e�[�W�������������ϐ�
		std::string stgStr;
		// �X�e�[�W��������T�C�Y�Ń��T�C�Y
		stgStr.resize(stgStrSize);
		// �X�e�[�W���ǂݍ���
		FileRead_read(stgStr.data(), static_cast<int>(stgStr.size() * sizeof(char)), handle);

		// �X�e�[�W�N���A�e�[�u��������Q��vector<StageData>�̎Q�Ƃ��擾
		auto& data = m_saveData[stgStr];

		// �f�[�^���̓ǂݍ���
		FileRead_read(&data, sizeof(data), handle);
	}

	// �E���ꂽ�G�̎�ސ��̎擾
	FileRead_read(&m_killedEnemyTypeNum, sizeof(m_killedEnemyTypeNum), handle);
	// ���O���Q�̐�����ސ��Ń��T�C�Y
	m_killedEnemyNameData.resize(m_killedEnemyTypeNum);
	for (int i = 0; i < m_killedEnemyTypeNum; i++)
	{
		// ���O������T�C�Y������ϐ�
		uint8_t nameSize;
		// �T�C�Y�̓ǂݍ���
		FileRead_read(&nameSize, sizeof(nameSize), handle);

		auto& name = m_killedEnemyNameData[i];
		// �T�C�Y�ŕ���������T�C�Y
		name.resize(nameSize);
		// ������̓ǂݍ���
		FileRead_read(name.data(), static_cast<int>(name.size() * sizeof(char)), handle);
	}

	// �{�X�N���A���̎擾
	uint8_t size;
	FileRead_read(&size, sizeof(size), handle);
	m_clearBossData.resize(size);
	for (int i = 0; i < size; i++)
	{
		uint8_t nameSize;
		FileRead_read(&nameSize, sizeof(nameSize), handle);
		auto& name = m_clearBossData[i];
		name.resize(nameSize);
		FileRead_read(name.data(), static_cast<int>(name.size() * sizeof(char)), handle);
	}

	// �A�r���e�B�̎擾
	FileRead_read(&m_ability, sizeof(m_ability), handle);

	// �A�r���e�B�L���e�[�u���̎擾
	FileRead_read(&size, sizeof(size), handle);
	for (int i = 0; i < size; i++)
	{
		Ability ability;
		bool active;
		FileRead_read(&ability, sizeof(ability), handle);
		FileRead_read(&active, sizeof(active), handle);

		m_isActiveAbilitys[ability] = active;
	}

	// �{�X�X�e�[�W�ɓ��������Ƃ��邩�̏��
	FileRead_read(&m_isInBoss, sizeof(m_isInBoss), handle);

	// �t�@�C���͕���
	FileRead_close(handle);
}
