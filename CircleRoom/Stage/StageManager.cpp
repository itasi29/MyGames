#include <DxLib.h>
#include <cassert>
#include <cmath>
#include "Application.h"

#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "Scene/SceneManager.h"
#include "Scene/OneShotScene.h"

#include "StageManager.h"
#include "StageBase.h"

namespace
{
	constexpr float kVersion = 1.3f;

	// �t�@�C���p
	struct StageInfHeader
	{
		char id[4] = "sti"; // �Ō��'\0'�����Ă�̂�4�o�C�g
		float version = kVersion;
		size_t dataCount = 0;
		// �󔒂�4�o�C�g(�p�f�B���O)
	};

	// �ړ��X�s�[�h
	constexpr float kSpeed = 100.0f;

	// ���������F������
	constexpr float kRecognitionDis = 1280.0f;
	// �������ɂȂ鋗��
	constexpr float kSlowDis = 640.0f;

	// �������x
	constexpr float kLimitSlowSpeed = 10.0f;

	// �X�e�[�W�̏c����
	constexpr int kLine = 4;
	constexpr int kRow = 3;

	// �X�e�[�W�Ԃ̊Ԋu
	constexpr int kStageMarginX = 1280;
	constexpr int kStageMarginY = 720;

	// ���O�̏ꏊ
	const std::string kStName[kLine][kRow] =
	{
		{"Stage1-5", "StageBoss", "Stage1-7"},
		{"Stage1-3", "Stage1-4", "Stage1-6"},
		{"Stage1-2", "Stage1-1", "Master"},
		{"none", "Tutorial", "none"}
	};
}

StageManager::StageManager(std::shared_ptr<SceneManager>& mgr) :
	m_mgr(mgr),
	m_size(Application::GetInstance().GetWindowSize()),
	m_isMove(false)
{
	// ������(�ǂݍ��݂Ɏ��s�����ꍇ�͏��v���C�Ɠ����Ƃ���)
	InitData();
	Load(L"Data/stg.inf");

	m_drawScreen = MakeScreen(m_size.w, m_size.h, true);
	m_screen = MakeScreen(m_size.w * kRow + kStageMarginX * kRow, m_size.h * kLine + kStageMarginY * kLine, true);
	m_keepScreen = MakeScreen(m_size.w * kRow + kStageMarginX * kRow, m_size.h * kLine + kStageMarginY * kLine, true);

	m_updateFunc = &StageManager::NormalUpdate;
	m_drawFunc = &StageManager::NormalDraw;
}

StageManager::~StageManager()
{
	DeleteGraph(m_drawScreen);
	DeleteGraph(m_screen);
	DeleteGraph(m_keepScreen);
	Save("Data/stg.inf");
}

void StageManager::Init()
{
	auto& mgr = GameManager::GetInstance().GetFile();
	m_dashImg = mgr->LoadGraphic(L"UI/dashExplanation.png");

	InitPos();
}

void StageManager::InitData()
{
	m_stageSaveData.clear();
	m_killedEnemyNameTable.clear();
	m_killedEnemyCount = 0;
	m_clearBossTable.clear();
	m_ability = kNone;
	m_abilityActive.clear();
	m_isBossIn = false;
}

void StageManager::InitPos()
{
	m_stage = nullptr;
	m_nextStage = nullptr;

	if (IsClearStage("Tutorial"))
	{
		const auto& stageName = GameManager::GetInstance().GetNowStage();
		if (stageName == "")
		{
			m_pos = GetPos("Stage1-1");
		}
		else
		{
			m_pos = GetPos(stageName);
		}
	}
	else
	{
		m_pos = GetPos("Tutorial");
	}
	m_targetPos = {};
	m_vec = {};
}

void StageManager::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void StageManager::Draw()
{
	(this->*m_drawFunc)();
}

std::string StageManager::GetStageName() const
{
	return m_stage->GetStageName();
}

void StageManager::ChangeStage(std::shared_ptr<StageBase> nextStage)
{
	// ����̂݁A��������邾��
	if (!m_stage)
	{
		m_stage = nextStage;
		return;
	}

	Vec2 pos;

	float dis = (m_targetPos - m_pos).Length();

	if (!m_isMove || dis < kRecognitionDis)
	{
		// �`��p�X�N���[���Ɍ��݂���X�e�[�W��`��
		SetDrawScreen(m_drawScreen);
		ClearDrawScreen();
		m_stage->Draw();

		// �L�[�v�p�̃X�N���[���ɏꏊ���w�肵�Ă������̂�`��
		SetDrawScreen(m_keepScreen);
		ClearDrawScreen();
		pos = GetPos(m_stage->GetStageName());

		DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), m_drawScreen, true);

		// ���݂̃X�e�[�W�̃|�C���^��ύX����
		m_stage = nextStage;
	}

	// �`��p�X�N���[���Ɏ��̃X�e�[�W�̕`��
	SetDrawScreen(m_drawScreen);
	ClearDrawScreen();
	nextStage->Draw();

	// �ړ����`��X�N���[���ɏꏊ���w�肵�Ă������̂�`��
	SetDrawScreen(m_screen);
	ClearDrawScreen();
	pos = GetPos(nextStage->GetStageName());

	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), m_drawScreen, true);

	// �L�[�v�ɂ�����̂�`��
	DrawGraph(0, 0, m_keepScreen, true);

	// �ړ��x�N�g���̌v�Z
	m_vec = (pos - m_pos).GetNormalized() * kSpeed;

	// ��������̏ꏊ�̕ۑ�
	m_targetPos = pos;

	// �����o�֐��|�C���^�̍X�V
	m_updateFunc = &StageManager::StageMoveUpdate;
	m_drawFunc = &StageManager::StageMoveDraw;

	// ���̃X�e�[�W�̕ۑ�
	m_nextStage = nextStage;
	SaveClear(m_nextStage->GetStageName());
	m_nextStage->StartCheck();

	// �����Ă��邱�Ƃ�
	m_isMove = true;

	// ���݂���X�e�[�W�̍X�V
	GameManager::GetInstance().UpdateNowStage(m_nextStage->GetStageName());

	// ���݂̕`���ւƖ߂�(�{����)
	SetDrawScreen(DX_SCREEN_BACK);
}

void StageManager::ImmediatelyChange()
{
	if (!m_isMove) return;

	m_updateFunc = &StageManager::MoveGamePlaingUpdate;
	m_drawFunc = &StageManager::MoveGamePlaingDraw;

	m_stage = m_nextStage;
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

	// �{�X�X�e�[�W�ɓ��������Ƃ��邩�̏��
	fwrite(&m_isBossIn, sizeof(m_isBossIn), 1, fp);

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

	// �{�X�X�e�[�W�ɓ��������Ƃ��邩�̏��
	FileRead_read(&m_isBossIn, sizeof(m_isBossIn), handle);

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

bool StageManager::UpdateBestTime(const std::string& stgName, int bestTime)
{
	auto it = m_stageSaveData.find(stgName);
	// �X�e�[�W���������Ȃ������牽�����Ȃ�
	if (it == m_stageSaveData.end())
	{
		assert(false);
		return false;
	}

	auto& time = m_stageSaveData[stgName].bestTime;

	// ���ݕۑ�����Ă���^�C�����X�V�^�C�����傫����΍X�V�͍s��Ȃ�
	if (time > bestTime)
	{
		return false;
	}

	// �^�C���̍X�V
	time = bestTime;
	return true;
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

void StageManager::NormalUpdate(Input& input)
{
	m_stage->Update(input);
	m_stage->ChangeStage(input);
}

void StageManager::StageMoveUpdate(Input& input)
{
	m_nextStage->Update(input);
	m_nextStage->ChangeStage(input);

	// �ꏊ�̍X�V
	m_pos += m_vec;

	CheckEnd();
}

void StageManager::MoveGamePlaingUpdate(Input& input)
{
	m_stage->Update(input);

	m_pos += m_vec;

	CheckEnd();
}

void StageManager::NormalDraw() const
{
	m_stage->Draw();
}

void StageManager::StageMoveDraw() const
{
	// MEMO:��ʏ�𓮂�������}�C�i�X�ɂ��Ă���
	DrawGraph(static_cast <int>(-m_pos.x), static_cast <int>(-m_pos.y), m_screen, true);

#ifdef _DEBUG
	DrawFormatString(100, 100, 0xffffff, L"dis:%.2f", (m_targetPos - m_pos).Length());
#endif
}

void StageManager::MoveGamePlaingDraw() const
{
	// �`��p�X�N���[���Ɏ��̃X�e�[�W�̕`��
	SetDrawScreen(m_drawScreen);
	ClearDrawScreen();
	m_stage->Draw();

	// �ړ����`��X�N���[���ɏꏊ���w�肵�Ă������̂�`��
	SetDrawScreen(m_screen);
	ClearDrawScreen();
	Vec2 pos = GetPos(m_stage->GetStageName());

	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), m_drawScreen, true);

	// �L�[�v�ɂ�����̂�`��
	DrawGraph(0, 0, m_keepScreen, true);

	SetDrawScreen(m_mgr->GetScreenHandle());

	DrawGraph(static_cast<int>(-m_pos.x), static_cast<int>(-m_pos.y), m_screen, true);
}

Vec2 StageManager::GetPos(const std::string& stage) const
{

	Vec2 pos;

	for (int x = 0; x < kRow; x++)
	{
		for (int y = 0; y < kLine; y++)
		{
			// �X�e�[�W������v������
			if (kStName[y][x] == stage)
			{
				// �ꏊ�̕ۑ�
				pos.x = static_cast <float>(m_size.w * x + kStageMarginX * x);
				pos.y = static_cast <float>(m_size.h * y + kStageMarginY * y);

				// �T���̏I��
				break;
			}
		}
	}

	return pos;
}

void StageManager::CheckEnd()
{
	// �����̌v�Z
	Vec2 vel = (m_targetPos - m_pos);
	float dis = vel.Length();

	// ���������鋗���ɂȂ�����
	if (dis < kSlowDis)
	{
		// ���x��0�ȉ��ɂȂ�����ړ��I��
		if (dis < kLimitSlowSpeed)
		{
			m_updateFunc = &StageManager::NormalUpdate;
			m_drawFunc = &StageManager::NormalDraw;

			m_pos = m_targetPos;

			m_stage = m_nextStage;

			m_isMove = false;
		}

		// �X�s�[�h�̌v�Z
		float speed = (dis / kSlowDis) * kSpeed;
		// �X�s�[�h�̒���
		m_vec = vel.GetNormalized() * speed;
	}
}
