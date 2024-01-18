#include <DxLib.h>
#include <cassert>
#include <cmath>
#include "Application.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "Scene/SceneManager.h"
#include "Scene/OneShotScene.h"

#include "StageManager.h"
#include "StageBase.h"

// �t�@�C���p
struct StageInfHeader
{
	char id[4] = "sti"; // �Ō��'\0'�����Ă�̂�4�o�C�g
	float version = 1.0f;
	size_t dataCount = 0;
	// �󔒂�4�o�C�g(�p�f�B���O)
};

namespace
{
	// �X�e�[�W�Ԃ̈ړ��ɂ����鎞��
	constexpr int kStageMoveFrame = 30;
}

StageManager::StageManager() :
	m_frame(0),
	m_isStageMove(false),
	m_stageHandle(-1),
	m_size(Application::GetInstance().GetWindowSize())
{
	// ������(�ǂݍ��݂Ɏ��s�����ꍇ�͏��v���C�Ɠ����Ƃ���)
	m_stageSaveData.clear();
	m_killedEnemyNameTable.clear();
	m_killedEnemyCount = 0;
	m_clearBossTable.clear();
	m_ability = kNone;
	m_abilityActive.clear();

	Load(L"Data/Bin/stg.inf");
}

StageManager::~StageManager()
{
	Save("Data/Bin/stg.inf");
}

void StageManager::Init()
{
	auto& mgr = GameManager::GetInstance().GetFile();
	m_dashImg = mgr->LoadGraphic(L"UI/operationExplanation.png");
}

void StageManager::DeleteData()
{
	m_stageSaveData.clear();
	m_killedEnemyNameTable.clear();
	m_killedEnemyCount = 0;
}

void StageManager::Update(Input& input)
{
	UpdateMove();
	m_stage->Update(input);
	m_stage->ChangeStage(input);
}

void StageManager::Draw()
{
	if (m_isStageMove)
	{
		DrawMove();
	}
	else
	{
		m_stage->Draw();
	}
}

std::string StageManager::GetStageName() const
{
	return m_stage->GetStageName();
}

void StageManager::ChangeStage(std::shared_ptr<StageBase> nextStage)
{
	m_stage = nextStage;
}

void StageManager::StartMove(StageManager::StageDir dir, int handle)
{
	m_isStageMove = true;
	m_frame = 0;

	switch (dir)
	{
	case StageManager::kStageLeft:
		// �ʒu�𒲐�����
		// ���Z�̊������+1���Ă���̂�1280�܂ŗ]����o����悤�ɂ��邽��
		m_pos.x = static_cast<float>((static_cast<int>(m_pos.x) + m_size.w) % (m_size.w + 1));

		m_vec.x = (0 - m_pos.x) / kStageMoveFrame;
		ResetVecY();
		break;
	case StageManager::kStageRight:
		// ������͈ʒu�𒲐����Ȃ�

		m_vec.x = static_cast<float>((m_size.w - static_cast<int>(m_pos.x)) % (m_size.w + 1) / kStageMoveFrame);
		ResetVecY();
		break;
	case StageManager::kStageUp:
		m_pos.y = static_cast<float>((static_cast<int>(m_pos.y) + m_size.h) % (m_size.h + 1));
		
		m_vec.y = (0 - m_pos.y) / kStageMoveFrame;
		ResetVecX();
		break;
	case StageManager::kStageDown:

		m_vec.y = static_cast<float>((m_size.h - static_cast<int>(m_pos.y)) % (m_size.h + 1) / kStageMoveFrame);
		ResetVecX();
		break;
	default:
		assert(false);
		break;
	}

	// ���g�������Ă����炻�������
	if (m_stageHandle != 0)
	{
		DeleteGraph(m_stageHandle);
	}
	m_stageHandle = handle;
}

int StageManager::GetSlideVolumeX(StageDir dir) const
{
	if (dir == kStageRight && m_vec.x > 0.0f)
	{
		return static_cast<int>(m_size.w);
	}
	if (dir == kStageLeft && m_vec.x < 0.0f)
	{
		return static_cast<int>(m_size.w);
	}

	return 0;
}

int StageManager::GetSlideVolumeY(StageDir dir) const
{
	if (dir == kStageDown && m_vec.y > 0.0f)
	{
		return static_cast<int>(m_size.h);
	}
	if (dir == kStageUp && m_vec.y < 0.0f)
	{
		return static_cast<int>(m_size.h);
	}

	return 0;
}


void StageManager::Save(const std::string& path)
{
	FILE* fp = nullptr; // �t�@�C���|�C���^
	auto err = fopen_s(&fp, path.c_str(), "wb");
	if (err != errno)
	{
		// �ǂݍ��݂Ɏ��s�������ߏI��
		assert(false);
		return;
	}
	// �w�b�_�̏�������
	StageInfHeader header;
	header.dataCount = m_stageSaveData.size();
	fwrite(&header, sizeof(header), 1, fp);

	// �N���A�f�[�^�̏�������
	for (const auto& stage : m_stageSaveData)
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
	fwrite(&m_killedEnemyCount, sizeof(m_killedEnemyCount), 1, fp);
	// ���O�f�[�^�̏�������
	for (const auto& name : m_killedEnemyNameTable)
	{
		// ���O�̕����񐔂��擾
		uint8_t nameSize = static_cast<uint8_t>(name.size());
		// �����񐔂���������
		fwrite(&nameSize, sizeof(nameSize), 1, fp);
		// ������̏�������
		fwrite(name.data(), name.size(), 1, fp);
	}

	// �{�X�N���A���̏�������
	uint8_t size = static_cast<uint8_t>(m_clearBossTable.size());
	fwrite(&size, sizeof(size), 1, fp);
	// �f�[�^�̏�������
	for (const auto& name : m_clearBossTable)
	{
		uint8_t nameSize = static_cast<uint8_t>(name.size());
		fwrite(&nameSize, sizeof(nameSize), 1, fp);
		fwrite(name.data(), name.size(), 1, fp);
	}

	// �A�r���e�B�̏�������
	fwrite(&m_ability, sizeof(m_ability), 1, fp);

	// �A�r���e�B�̗L���e�[�u���̏�������
	size = static_cast<uint8_t>(m_abilityActive.size());
	fwrite(&size, sizeof(size), 1, fp);
	for (const auto& active : m_abilityActive)
	{
		// �A�r���e�B�^�C�v��������
		fwrite(&active.first, sizeof(active.first), 1, fp);
		// �L�����ǂ�����������
		fwrite(&active.second, sizeof(active.second), 1, fp);
	}

	fclose(fp);
}

void StageManager::Load(const std::wstring& path)
{
	auto handle = FileRead_open(path.c_str());
	// �G���[(�t�@�C�����Ȃ�)�ꍇ�͏������Ȃ�
	if (handle == 0)
	{
		return;
	}
	// �w�b�_���̓ǂݍ���
	StageInfHeader header;
	FileRead_read(&header, sizeof(header), handle);

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
		auto& data = m_stageSaveData[stgStr];

		// �f�[�^���̓ǂݍ���
		FileRead_read(&data, sizeof(data), handle);
	}

	// �E���ꂽ�G�̎�ސ��̎擾
	FileRead_read(&m_killedEnemyCount, sizeof(m_killedEnemyCount), handle);
	// ���O���Q�̐�����ސ��Ń��T�C�Y
	m_killedEnemyNameTable.resize(m_killedEnemyCount);
	for (int i = 0; i < m_killedEnemyCount; i++)
	{
		// ���O������T�C�Y������ϐ�
		uint8_t nameSize;
		// �T�C�Y�̓ǂݍ���
		FileRead_read(&nameSize, sizeof(nameSize), handle);

		auto& name = m_killedEnemyNameTable[i];
		// �T�C�Y�ŕ���������T�C�Y
		name.resize(nameSize);
		// ������̓ǂݍ���
		FileRead_read(name.data(), static_cast<int>(name.size() * sizeof(char)), handle);
	}

	// �{�X�N���A���̎擾
	uint8_t size;
	FileRead_read(&size, sizeof(size), handle);
	m_clearBossTable.resize(size);
	for (int i = 0; i < size; i++)
	{
		uint8_t nameSize;
		FileRead_read(&nameSize, sizeof(nameSize), handle);
		auto& name = m_clearBossTable[i];
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

		m_abilityActive[ability] = active;
	}

	// �t�@�C���͕���
	FileRead_close(handle);
}

void StageManager::CreateData(const std::string& stgName)
{
	auto it = m_stageSaveData.find(stgName);
	// �X�e�[�W���������牽�����Ȃ�
	if (it != m_stageSaveData.end())
	{
		return;
	}

	// �Ȃ���΍쐬����
	auto& data = m_stageSaveData[stgName];

	// ���̏�����
	data.bestTime = 0;
	data.isClear = false;
}

bool StageManager::IsClearStage(const std::string& stgName)
{
	auto it = m_stageSaveData.find(stgName);
	// �X�e�[�W���������Ȃ�������0��Ԃ�
	if (it == m_stageSaveData.end())
	{
		CreateData(stgName);
		return false;
	}

	// ����������N���A����Ԃ�
	return m_stageSaveData.at(stgName).isClear;
}

bool StageManager::IsClearBoss(const std::string& name) const
{
	for (const auto& boss : m_clearBossTable)
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

bool StageManager::IsKilledEnemy(const std::string& name) const
{
	for (const auto& killedName : m_killedEnemyNameTable)
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

int StageManager::GetBestTime(const std::string& stgName) const
{
	auto it = m_stageSaveData.find(stgName);
	// �X�e�[�W���������Ȃ�������0��Ԃ�
	if (it == m_stageSaveData.end())
	{
		assert(false);
		return 0;
	}

	// ����������x�X�g�^�C����Ԃ�
	return m_stageSaveData.at(stgName).bestTime;
}

int StageManager::GetEnemyTypeCount() const
{
	return m_killedEnemyCount;
}

Ability StageManager::GetAbility() const
{
	return m_ability;
}

void StageManager::SaveClear(const std::string& stgName)
{
	auto it = m_stageSaveData.find(stgName);
	// �X�e�[�W���������Ȃ������牽�����Ȃ�
	if (it == m_stageSaveData.end())
	{
		assert(false);
		return;
	}

	// �w��̂��̂��N���A�Ƃ���
	m_stageSaveData[stgName].isClear = true;
}

void StageManager::UpdateClearBoss(const std::string& name)
{
	for (const auto& boss : m_clearBossTable)
	{
		// �N���A�������Ƃ�����Ή������Ȃ�
		if (boss == name)
		{
			return;
		}
	}

	// ��񂪏���Ă��Ȃ��ꍇ�͒ǉ�����
	m_clearBossTable.push_back(name);
	return;
}

void StageManager::UpdateBestTime(const std::string& stgName, int bestTime)
{
	auto it = m_stageSaveData.find(stgName);
	// �X�e�[�W���������Ȃ������牽�����Ȃ�
	if (it == m_stageSaveData.end())
	{
		assert(false);
		return;
	}

	auto& time = m_stageSaveData[stgName].bestTime;

	// ���ݕۑ�����Ă���^�C�����X�V�^�C�����傫����΍X�V�͍s��Ȃ�
	if (time > bestTime)
	{
		return;
	}

	// �^�C���̍X�V
	time = bestTime;
}

void StageManager::UpdateEnemyType(const std::string& name)
{
	// �z��S���J��Ԃ�
	for (const auto& tableName : m_killedEnemyNameTable)
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
	m_killedEnemyCount++;
	// �e�[�u���ɓo�^
	m_killedEnemyNameTable.push_back(name);

	// �A�r���e�B�̗L����
	if (name == "Dash")
	{
		m_abilityActive[kDash] = true;

		// �_�b�V�������摜
		auto& mgr = GameManager::GetInstance();
		mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(mgr, m_dashImg->GetHandle()));
	}
}


void StageManager::ChangeAbility(Ability ability)
{
	// �A�r���e�B���L���ɂȂ��Ă�����ύX������
	if (m_abilityActive[ability])
	{
		m_ability = ability;
	}
}

void StageManager::UpdateMove()
{
	if (!m_isStageMove) return;

	// �t���[���̍X�V
	m_frame++;
	// �ꏊ�̍X�V
	m_pos += m_vec;

	// ���t���[���������瓮���������Ƃ���
	if (m_frame >= kStageMoveFrame)
	{
		m_isStageMove = false;

		// �ʒu�A�x�N�g�������������Ă���
		m_pos = { 0.0f, 0.0f };
		m_vec = { 0.0f, 0.0f };
	}
}

void StageManager::DrawMove()
{
	DrawRectGraph(0, 0, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_size.w, m_size.h,
		m_stageHandle, true);

#ifdef _DEBUG
	DrawFormatString(32, 32, 0xff0808, L"�X�e�[�W�ړ��� %d", m_frame);
	DrawFormatString(32, 48, 0xff0808, L"���W(%.2f, %.2f)", m_pos.x, m_pos.y);
#endif
}

void StageManager::ResetVecX()
{
	// ���ɓ����Ă���
	if (m_vec.x < 0.0f)
	{
		m_vec.x = -m_pos.x / kStageMoveFrame;
		return;
	}
	// �E�ɓ����Ă���
	if (m_vec.x > 0.0f)
	{
		m_vec.x = static_cast<float>((m_size.w - static_cast<int>(m_pos.x)) % (m_size.w + 1) / kStageMoveFrame);
		return;
	}
}

void StageManager::ResetVecY()
{
	// ��ɓ����Ă���
	if (m_vec.y < 0.0f)
	{
		m_vec.y = -m_pos.y / kStageMoveFrame;
		return;
	}
	// ���ɓ����Ă���
	if (m_vec.y > 0.0f)
	{
		m_vec.y = static_cast<float>((m_size.h - static_cast<int>(m_pos.y)) % (m_size.h + 1) / kStageMoveFrame);
		return;
	}
}