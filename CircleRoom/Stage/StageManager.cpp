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
	// �X�e�[�W1-1�ɓ���ǉ�
	m_stageClearTable["1-1"].push_back(false);
	m_stageClearTable["1-1"].push_back(false);
	// �X�e�[�W1-2�ɓ���ǉ�
	m_stageClearTable["1-2"].push_back(false);
	// �X�e�[�W1-3�ɓ���ǉ�
	m_stageClearTable["1-2"].push_back(false);

	Load(L"stg.inf");
}

StageManager::~StageManager()
{
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
	case StageManager::kDirLeft:
		// �ʒu�𒲐�����
		// ���Z�̊������+1���Ă���̂�1280�܂ŗ]����o����悤�ɂ��邽��
		m_pos.x = static_cast<float>((static_cast<int>(m_pos.x) + m_size.w) % (m_size.w + 1));

		m_vec.x = (0 - m_pos.x) / kStageMoveFrame;
		ResetVecY();
		break;
	case StageManager::kDirRight:
		// ������͈ʒu�𒲐����Ȃ�

		m_vec.x = static_cast<float>((m_size.w - static_cast<int>(m_pos.x)) % (m_size.w + 1) / kStageMoveFrame);
		ResetVecY();
		break;
	case StageManager::kDirUp:
		m_pos.y = static_cast<float>((static_cast<int>(m_pos.y) + m_size.h) % (m_size.h + 1));
		
		m_vec.y = (0 - m_pos.y) / kStageMoveFrame;
		ResetVecX();
		break;
	case StageManager::kDirDown:

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
	if (dir == kDirRight && m_vec.x > 0.0f)
	{
		return static_cast<int>(m_size.w);
	}
	if (dir == kDirLeft && m_vec.x < 0.0f)
	{
		return static_cast<int>(m_size.w);
	}

	return 0;
}

int StageManager::GetSlideVolumeY(MoveDir dir) const
{
	if (dir == kDirDown && m_vec.y > 0.0f)
	{
		return static_cast<int>(m_size.h);
	}
	if (dir == kDirUp && m_vec.y < 0.0f)
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
	//if (err == errno)
	//{
	//	// �ǂݍ��݂Ɏ��s�������ߏI��
	//	assert(false);
	//	return;
	//}
	// �w�b�_�̏�������
	StageInfHeader header;
	header.dataCount = m_stageClearTable.size();
	fwrite(&header, sizeof(header), 1, fp);

	// �f�[�^�{�̂���������ł���
	for (const auto& stg : m_stageClearTable)
	{
		const auto& stageStr = stg.first; // �X�e�[�W��������
		uint8_t size = stageStr.size();   // �X�e�[�W��������̃T�C�Y���擾
		fwrite(&size, sizeof(size), 1, fp); // �X�e�[�W��������̃o�C�g������������
		fwrite(stageStr.data(), stageStr.size(), 1, fp);    // ������̏�������

		uint8_t vectorSize = stg.second.size();	// �z��̐����擾
		fwrite(&vectorSize, sizeof(vectorSize), 1, fp);	// �z�񐔂���������
		for (const auto& inf : stg.second)
		{
			fwrite(&inf, sizeof(inf), 1, fp);	// �N���A������������
		}
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

		// �X�e�[�W�N���A�e�[�u��������Q��vector�̎Q�Ƃ��擾
		auto& infVec = m_stageClearTable[stgStr];

		// vector�̃T�C�Y������ϐ�
		uint8_t vectorSize;
		// vectro�̃T�C�Y���擾����
		FileRead_read(&vectorSize, sizeof(vectorSize), handle);

		// vector�̃T�C�Y�Ń��T�C�Y����
		infVec.resize(vectorSize);

		for (int j = 0; j < vectorSize; j++)
		{
			// �u�[���̏����擾����
			bool inf;

			FileRead_read(&inf, sizeof(inf), handle);

			infVec[j] = inf;
		}
	}
}

void StageManager::SaveClearInf(const std::string& stgName, const std::vector<bool>& inf)
{
	auto& data = m_stageClearTable[stgName];

	data.resize(inf.size());

	for (int i = 0; i < inf.size(); i++)
	{
		data[i] = inf[i];
	}
}

bool StageManager::GetClearInf(const std::string& stgName, std::vector<bool>& inf)
{
	auto it = m_stageClearTable.find(stgName);
	// �v�f���Ȃ��ꍇ��false��Ԃ�
	if (it == m_stageClearTable.end())
	{
		assert(false);
		return false;
	}

	const auto& data = m_stageClearTable[stgName];

	inf.resize(data.size());

	for (int i = 0; i < data.size(); i++)
	{
		inf[i] = data[i];
	}
	// �����܂ŗ�����ۑ��ł���
	return true;
}
