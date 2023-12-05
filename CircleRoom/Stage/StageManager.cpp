#include <DxLib.h>
#include <cassert>
#include <cmath>
#include "Application.h"

#include "StageManager.h"
#include "StageBase.h"

namespace
{
	// �t�@�C���p
	struct StageInfHeader
	{
		char id[4] = "sti"; // �Ō��'\0'�����Ă�̂�4�o�C�g
		float version = 1.0f;
		size_t dataCount = 0;
		// �󔒂�4�o�C�g(�p�f�B���O)
	};

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

	Load(L"stg.inf");
}

StageManager::~StageManager()
{
	m_stage->SaveInf();
	Save("stg.inf");
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

void StageManager::ChangeStage(std::shared_ptr<StageBase> nextStage)
{
	m_stage = nextStage;
}

void StageManager::StartMove(MoveDir dir, int handle)
{
	m_isStageMove = true;
	m_frame = 0;

	switch (dir)
	{
	case StageManager::kMoveDirLeft:
		// �ʒu�𒲐�����
		// ���Z�̊������+1���Ă���̂�1280�܂ŗ]����o����悤�ɂ��邽��
		m_pos.x = static_cast<float>((static_cast<int>(m_pos.x) + m_size.w) % (m_size.w + 1));

		m_vec.x = (0 - m_pos.x) / kStageMoveFrame;
		ResetVecY();
		break;
	case StageManager::kMoveDirRight:
		// ������͈ʒu�𒲐����Ȃ�

		m_vec.x = static_cast<float>((m_size.w - static_cast<int>(m_pos.x)) % (m_size.w + 1) / kStageMoveFrame);
		ResetVecY();
		break;
	case StageManager::kMoveDirUp:
		m_pos.y = static_cast<float>((static_cast<int>(m_pos.y) + m_size.h) % (m_size.h + 1));
		
		m_vec.y = (0 - m_pos.y) / kStageMoveFrame;
		ResetVecX();
		break;
	case StageManager::kMoveDirDown:

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

int StageManager::GetSlideVolumeX(MoveDir dir) const
{
	if (dir == kMoveDirRight && m_vec.x > 0.0f)
	{
		return static_cast<int>(m_size.w);
	}
	if (dir == kMoveDirLeft && m_vec.x < 0.0f)
	{
		return static_cast<int>(m_size.w);
	}

	return 0;
}

int StageManager::GetSlideVolumeY(MoveDir dir) const
{
	if (dir == kMoveDirDown && m_vec.y > 0.0f)
	{
		return static_cast<int>(m_size.h);
	}
	if (dir == kMoveDirUp && m_vec.y < 0.0f)
	{
		return static_cast<int>(m_size.h);
	}

	return 0;
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
		uint8_t size = stageStr.size();
		// �X�e�[�W��������̃o�C�g������������
		fwrite(&size, sizeof(size), 1, fp); 
		// ������̏�������
		fwrite(stageStr.data(), stageStr.size(), 1, fp);    

		// �f�[�^�Q�̎Q��
		auto& data = stage.second;
		// �x�X�g�^�C���̏�������
		fwrite(&data.bestTime, sizeof(data.bestTime), 1, fp);
		// �z�񐔂���������
		uint8_t dataSize = data.isClears.size();
		fwrite(&dataSize, sizeof(dataSize), 1, fp);
		for (const auto& isClear : data.isClears)
		{
			// �N���A������������
			fwrite(&isClear, sizeof(isClear), 1, fp);
		}
	}

	// �f�[�^�{�̂���������ł���
	// �E���ꂽ�G�̎�ސ��̏�������
	fwrite(&m_killedEnemyCount, sizeof(m_killedEnemyCount), 1, fp);
	// ���O�f�[�^�̏�������
	for (const auto& name : m_killedEnemyNameTable)
	{
		// ���O�̕����񐔂��擾
		uint8_t nameSize = name.size();
		// �����񐔂���������
		fwrite(&nameSize, sizeof(nameSize), 1, fp);
		// ������̏�������
		fwrite(name.data(), name.size(), 1, fp);
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
		FileRead_read(stgStr.data(), stgStr.size() * sizeof(char), handle);

		// �X�e�[�W�N���A�e�[�u��������Q��vector<StageData>�̎Q�Ƃ��擾
		auto& data = m_stageSaveData[stgStr];

		// �x�X�g�^�C���ǂݍ���
		FileRead_read(&data.bestTime, sizeof(data.bestTime), handle);

		// �z�񐔓ǂݍ���
		uint8_t dataSize;
		FileRead_read(&dataSize, sizeof(dataSize), handle);

		// vector�̃��T�C�Y
		data.isClears.resize(dataSize);
		for (int j = 0; j < dataSize; j++)
		{
			// �N���A���̎擾
			bool isClear;

			FileRead_read(&isClear, sizeof(isClear), handle);

			data.isClears[j] = isClear;
		}
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
		FileRead_read(name.data(), name.size() * sizeof(char), handle);
		;
	}

	// �t�@�C���͕���
	FileRead_close(handle);
}

void StageManager::SaveClearInf(const std::string& stgName, const StageData& data)
{
	auto& saveData = m_stageSaveData[stgName];

	saveData.bestTime = data.bestTime;

	saveData.isClears.resize(data.isClears.size());

	for (int i = 0; i < data.isClears.size(); i++)
	{
		saveData.isClears[i] = data.isClears[i];
	}
}

bool StageManager::GetClearInf(const std::string& stgName, StageData& data)
{
	auto it = m_stageSaveData.find(stgName);
	// �v�f���Ȃ��ꍇ��false��Ԃ����������ɏI������
	if (it == m_stageSaveData.end())
	{
		return false;
	}

	data = m_stageSaveData[stgName];

	// �����܂ŗ�����ۑ��ł���
	return true;
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

int StageManager::GetKilledEnemyCount() const
{
	return m_killedEnemyCount;
}

void StageManager::UpdateKilledEnemy(std::string name)
{
	// �z��S���J��Ԃ�
	for (auto& tableName : m_killedEnemyNameTable)
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
}
