#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "Stage1_5.h"
#include "Stage1_4.h"

#include "Player/Player.h"
#include "Enemy/EnemyMoveWall.h"
#include "Boss/BossArmored.h"
#include "Boss/BossStrongArmored.h"

namespace
{
	constexpr int kDownKilledNum = 5;

	const std::string kDownStName = "Stage1-4";
}

Stage1_5::Stage1_5(GameManager& mgr, float fieldSize) :
	StageBase(mgr, fieldSize),
	m_createFrame(0)
{
	m_stageName = "Stage1-5";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.GetStage().CreateData(m_stageName);

	StartCheck();
}

Stage1_5::~Stage1_5()
{
}

void Stage1_5::Init()
{
	// �o�ߎ��Ԃ̏�����
	m_frame = 0;
	// �o�߂��s������������
	m_isUpdateTime = false;

	// �����t���[���̏�����
	m_createFrame = 0;

	// �v���C���[�̏�����
	m_player->Init();

	// �G�̔z���������
	m_enemy.clear();

	// �Ǔ����G�̍쐬
	Vec2 vec;
	// �㑤
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_size, m_fieldSize));
	vec.x = 0;
	vec.y = -1;
	m_enemy.back()->Init(vec);
	// ����
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_size, m_fieldSize));
	vec.y = 1;
	m_enemy.back()->Init(vec);

	// �X�^�[�g�ʒu�̐ݒ�
	m_boss = std::make_shared<BossArmored>(m_size, m_fieldSize, this);
	m_boss->Init(m_centerPos);
}

void Stage1_5::StartCheck()
{
	m_isDownClear = m_mgr.GetStage().IsClearStage(kDownStName);
}

void Stage1_5::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.GetStage().IsClearStage(kDownStName) && input.IsTriggered("down"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, m_fieldSize);

		SlideDown(nextStage);

		return;
	}
}

void Stage1_5::CheckStageConditions()
{
	// �����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.GetStage().IsClearStage(kDownStName))
	{
		if (m_mgr.GetStage().GetEnemyTypeCount() >= kDownKilledNum)
		{
			m_mgr.GetStage().SaveClear(kDownStName);
		}
	}
}

void Stage1_5::DrawStageConditions(int drawY)
{
	if (!m_isDownClear)
	{
		DrawFormatString(128, drawY, 0xffffff, L"���@%d��ނ̓G�ɎE�����\n(%d / %d)",
			kDownKilledNum, m_mgr.GetStage().GetEnemyTypeCount(), kDownKilledNum);

		drawY += 32;
	}

	// FIXME: �����ɒǉ��ŏ����Ă��邯��ǂ��Ƃŕʂ̂Ƃ���ɏ�����ύX����
	if (m_mgr.GetStage().IsClearBoss("BossArmored"))
	{
		DrawString(128, drawY, L"clear", 0xffffff);
	}
}

void Stage1_5::DrawArrow() const
{
	DrawDownArrow(m_isDownClear, kDownStName);
}

void Stage1_5::DrawKilledEnemyType() const
{
	if (m_mgr.GetStage().IsKilledEnemy("MoveWall"))
	{
		DrawCircle(256, 28, 16, 0x888888, true);
	}
	else
	{
		DrawCircle(256, 28, 16, 0x888888, true);
	}

	if (m_mgr.GetStage().IsKilledEnemy("BossArmored"))
	{
		DrawCircle(256 + 48, 28, 16, 0x08ff08, true);
	}
	else
	{
		DrawCircle(256 + 48, 28, 16, 0x08ff08, false);
	}

	if (m_mgr.GetStage().IsKilledEnemy("BossStrongArmored"))
	{
		DrawCircle(256 + 96, 28, 16, 0xaaffaa, true);
	}
	else
	{
		DrawCircle(256 + 96, 28, 16, 0xaaffaa, false);
	}

	if (m_mgr.GetStage().IsKilledEnemy("SplitTwoBound"))
	{
		DrawCircle(256 + 144, 28, 14, 0xffffff, true);
	}
	else
	{
		DrawCircle(256 + 144, 28, 14, 0xffffff, false);
	}
}

void Stage1_5::CreateEnemy()
{
}

void Stage1_5::UpdateTime()
{
	if (m_isUpdateTime)
	{
		// ��b�ǉ�
		m_frame += 60;
		m_isUpdateTime = false;
	}
}

void Stage1_5::CreateStrongBoss()
{
	std::shared_ptr<BossStrongArmored> strong;
	strong = std::make_shared<BossStrongArmored>(m_size, m_fieldSize, this);
	strong->Init(m_boss->GetPos());

	m_boss = strong;
}
