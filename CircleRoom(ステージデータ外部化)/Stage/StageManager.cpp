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
#include "StageMap.h"
#include "GameData.h"

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
		{"�����i�[", "Reaper", "����"},
		{"�ߐڑ���", "�ؒf", "����"},
		{"����", "�T�[�N��", "�v�x��"},
		{"none", "���K", "none"}
	};
}

StageManager::StageManager(std::shared_ptr<SceneManager>& mgr) :
	m_mgr(mgr),
	m_size(Application::GetInstance().GetWindowSize()),
	m_isMove(false)
{
	m_map = std::make_shared<StageMap>();
	m_data = std::make_shared<GameData>();

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
}

void StageManager::Init(Input& input)
{
	m_data->Init();
	InitPos();
	m_stage = std::make_shared<StageBase>(GameManager::GetInstance(), input, m_map);
}

void StageManager::InitPos()
{
	if (m_data->IsClearStage("���K"))
	{
		const auto& stageName = GameManager::GetInstance().GetNowStage();
		if (stageName == "")
		{
			m_pos = m_map->GetPos("�T�[�N��");
		}
		else
		{
			m_pos = m_map->GetPos(stageName);
		}
	}
	else
	{
		m_pos = m_map->GetPos("���K");
	}
	m_targetPos = {};
	m_vec = {};
}

void StageManager::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void StageManager::Draw() const
{
	(this->*m_drawFunc)();
}

std::string StageManager::GetStageName() const
{
	return m_stage->GetStageName();
}

void StageManager::OffDrawWave() const
{
	m_stage->OffDrawWave();
}

void StageManager::ChangeStage(const std::string& nextStage, bool isGameScene)
{
	// ����܂��̓Q�[���V�[������̂݁A��������邾��
	if (isGameScene)
	{
//		m_stage = nextStage;
		m_stage->ChangeStage(nextStage);
		m_stage->Init();
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
		pos = m_map->GetPos(m_stage->GetStageName());

		DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), m_drawScreen, true);

	}
	m_stage->ChangeStage(nextStage);
	m_stage->Init();

	// �`��p�X�N���[���Ɏ��̃X�e�[�W�̕`��
	SetDrawScreen(m_drawScreen);
	ClearDrawScreen();
	m_stage->Draw();

	// �ړ����`��X�N���[���ɏꏊ���w�肵�Ă������̂�`��
	SetDrawScreen(m_screen);
	ClearDrawScreen();
	pos = m_map->GetPos(m_stage->GetStageName());

	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), m_drawScreen, true);

	// �L�[�v�ɂ�����̂�`��
	DrawGraph(0, 0, m_keepScreen, true);

	// �ړ��x�N�g���̌v�Z
	m_vec = (pos - m_pos).GetNormalized() * kSpeed;

	// ��������̏ꏊ�̕ۑ�
	m_targetPos = pos;

	// ���̃X�e�[�W�̕ۑ�
	m_data->SaveClearStage(nextStage);
	m_stage->StartCheck();

	m_isMove = true;

	GameManager::GetInstance().UpdateNowStage(nextStage);

	// ���݂̕`���ւƖ߂�(�{����)
	SetDrawScreen(DX_SCREEN_BACK);

	// �����o�֐��|�C���^�̍X�V
	m_updateFunc = &StageManager::StageMoveUpdate;
	m_drawFunc = &StageManager::StageMoveDraw;
}

void StageManager::ImmediatelyChange()
{
	if (!m_isMove) return;

	m_updateFunc = &StageManager::MoveGamePlaingUpdate;
	m_drawFunc = &StageManager::MoveGamePlaingDraw;
}

void StageManager::NormalUpdate(Input& input)
{
	m_stage->Update(input);
	m_stage->ChangeStage(input);
}

void StageManager::StageMoveUpdate(Input& input)
{
	m_stage->Update(input);
	m_stage->ChangeStage(input);

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
	// MEMO:�f�o�b�O�Ȃ̂Ŗ��萔��
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
	Vec2 pos = m_map->GetPos(m_stage->GetStageName());

	DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), m_drawScreen, true);

	// �L�[�v�ɂ�����̂�`��
	DrawGraph(0, 0, m_keepScreen, true);

	SetDrawScreen(m_mgr->GetScreenHandle());

	DrawGraph(static_cast<int>(-m_pos.x), static_cast<int>(-m_pos.y), m_screen, true);
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

			m_isMove = false;
		}

		// �X�s�[�h�̌v�Z
		float speed = (dis / kSlowDis) * kSpeed;
		// �X�s�[�h�̒���
		m_vec = vel.GetNormalized() * speed;
	}
}
