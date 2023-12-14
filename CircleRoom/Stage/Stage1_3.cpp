#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_3.h"
#include "Stage1_1.h"
#include "Stage1_4.h"

#include "Player/Player.h"
#include "Enemy/EnemyMoveWall.h"
#include "Enemy/EnemyDash.h"

namespace
{
	// �N���A����
	constexpr int kDownExsitTime = 15;
	constexpr int kLeftKilledNum = 3;

	// �����Ԋu�t���[��
	constexpr int kCreateFrame = 60 * 6;
}

Stage1_3::Stage1_3(StageManager& mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = "Stage1-3";
	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.CreateData(m_stageName);
	CheckStageConditions();

	StartCheck();
}

Stage1_3::~Stage1_3()
{
}

void Stage1_3::Init()
{
	// �o�ߎ��Ԃ̏�����
	m_frame = 0;
	// �o�߂��s������������
	m_isUpdateTime = true;

	// �����t���[���̏�����
	m_createFrame = 0;

	// �v���C���[�̏�����
	m_player->Init();

	// �G�̔z���������
	m_enemy.clear();

	// �Ǔ����G�̍쐬
	Vec2 vec;
	// �㑤
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_windowSize, m_fieldSize));
	vec.x = 0;
	vec.y = -1;
	m_enemy.back()->Init(vec);
	// ����
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_windowSize, m_fieldSize));
	vec.y = 1;
	m_enemy.back()->Init(vec);

	m_enemy.push_back(std::make_shared<EnemyDash>(m_windowSize, m_fieldSize, m_player));
	m_enemy.back()->Init(m_centerPos);
	m_enemy.push_back(std::make_shared<EnemyDash>(m_windowSize, m_fieldSize, m_player));
	m_enemy.back()->Init(m_centerPos);
}

void Stage1_3::StartCheck()
{
	m_isDownClear = m_mgr.IsClearStage(m_stageName, StageManager::kStageDown);
	m_isLeftClear = m_mgr.IsClearStage(m_stageName, StageManager::kStageLeft);
}

void Stage1_3::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.IsClearStage(m_stageName, StageManager::kStageLeft) && input.IsPress("left"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, m_windowSize, m_fieldSize);

		SlideLeft(nextStage);

		return;
	}
	if (m_mgr.IsClearStage(m_stageName, StageManager::kStageDown) && input.IsPress("down"))
	{
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, m_windowSize, m_fieldSize);

		SlideDown(nextStage);

		return;
	}
}

void Stage1_3::CheckStageConditions()
{
	// �����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.IsClearStage(m_stageName, StageManager::kStageLeft))
	{
		if (m_mgr.GetEnemyTypeCount() >= kLeftKilledNum)
		{
			m_mgr.SaveClear(m_stageName, StageManager::kStageLeft);
		}
	}
	// �����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.IsClearStage(m_stageName, StageManager::kStageDown))
	{
		if (m_mgr.GetBestTime(m_stageName) > kDownExsitTime * 60)
		{
			m_mgr.SaveClear(m_stageName, StageManager::kStageDown);
		}
	}
}

void Stage1_3::DrawStageConditions(int drawY)
{
	if (!m_isLeftClear)
	{
		DrawFormatString(128, drawY, 0xffffff, L"���@%d��ނ̓G�ɎE�����\n(%d / %d)",
			kLeftKilledNum, m_mgr.GetEnemyTypeCount(), kLeftKilledNum);

		drawY += 32;
	}
	if (!m_isDownClear)
	{
		DrawFormatString(128, drawY, 0xffffff, L"���@%d�b�Ԑ����c��\n(%d / %d)", 
			kDownExsitTime, m_mgr.GetBestTime(m_stageName) / 60, kDownExsitTime);
	}
}

void Stage1_3::DrawArrow() const
{
	DrawLeftArrow(m_isLeftClear);
	DrawDownArrow(m_isDownClear);
}

void Stage1_3::DrawKilledEnemyType() const
{
	if (m_mgr.IsKilledEnemy("Dash"))
	{
		DrawCircle(256, 28, 16, 0x0808ff, true);
	}
	else
	{
		DrawCircle(256, 28, 16, 0x0808ff, false);
	}

	if (m_mgr.IsKilledEnemy("MoveWall"))
	{
		DrawCircle(256 + 48, 28, 16, 0x888888, true);
	}
	else
	{
		DrawCircle(256 + 48, 28, 16, 0x888888, false);
	}
}

void Stage1_3::CreateEnemy()
{
	m_createFrame++;

	if (m_createFrame > kCreateFrame)
	{
		m_createFrame = 0;
		m_enemy.push_back(std::make_shared<EnemyDash>(m_windowSize, m_fieldSize, m_player));
		m_enemy.back()->Init(m_centerPos);
	}
}

void Stage1_3::UpdateTime()
{
	m_frame++;
}
