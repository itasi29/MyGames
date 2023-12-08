#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_4.h"
#include "Stage1_3.h"
#include "Stage1_5.h"

#include "Player/Player.h"
#include "Enemy/EnemyDivision.h"

namespace
{
	constexpr int kRightKilledNum = 3;
	constexpr int kUpKilledNum = 5;
}

Stage1_4::Stage1_4(StageManager& mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = "Stage1-4";
	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.CreateData(m_stageName);

	m_isRightClear = m_mgr.IsClear(m_stageName, StageManager::kStageRight);
	m_isUpClear = m_mgr.IsClear(m_stageName, StageManager::kStageUp);
}

Stage1_4::~Stage1_4()
{
}

void Stage1_4::Init()
{
	// �o�ߎ��Ԃ̏�����
	m_frame = 0;

	// �����t���[���̏�����
	m_createFrame = 0;

	// �v���C���[�̏�����
	m_player->Init();

	// �G�̔z���������
	m_enemy.clear();

	Vec2 vec;

	// �X�^�[�g�ʒu�̐ݒ�
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;
	vec = { centerX, centerY };
	m_enemy.push_back(std::make_shared<EnemyDivision>(m_windowSize, m_fieldSize, this));
	m_enemy.back()->Init(vec);
}

void Stage1_4::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.IsClear(m_stageName, StageManager::kStageRight) && input.IsPress("right"))
	{
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr, m_windowSize, m_fieldSize);

		SlideRight(nextStage);

		return;
	}
	if (m_mgr.IsClear(m_stageName, StageManager::kStageUp) && input.IsPress("up"))
	{
		std::shared_ptr<Stage1_5> nextStage;
		nextStage = std::make_shared<Stage1_5>(m_mgr, m_windowSize, m_fieldSize);

		SlideUp(nextStage);

		return;
	}
}

void Stage1_4::CheckStageConditions()
{
	// �E���܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.IsClear(m_stageName, StageManager::kStageRight))
	{
		if (m_mgr.GetKilledEnemyCount() >= kRightKilledNum)
		{
			m_mgr.SaveClear(m_stageName, StageManager::kStageRight);
		}
	}
	if (!m_mgr.IsClear(m_stageName, StageManager::kStageUp))
	{
		if (m_mgr.GetKilledEnemyCount() >= kUpKilledNum)
		{
			m_mgr.SaveClear(m_stageName, StageManager::kStageUp);
		}
	}
}

void Stage1_4::DrawStageConditions(int drawY)
{
	if (!m_isRightClear)
	{
		DrawFormatString(128, drawY, 0xffffff, L"�E�@%d�̎�ނŎ���\n(%d / %d)",
			kRightKilledNum, m_mgr.GetKilledEnemyCount(), kRightKilledNum);

		drawY += 32;
	}
	if (!m_isUpClear)
	{
		DrawFormatString(128, drawY, 0xffffff, L"��@%d�̎�ނŎ���\n(%d / %d)",
			kUpKilledNum, m_mgr.GetKilledEnemyCount(), kUpKilledNum);
	}
}

void Stage1_4::DrawArrow() const
{
	DrawRightArrow();
	DrawUpArrow();
}

void Stage1_4::CreateEnemy()
{
}
