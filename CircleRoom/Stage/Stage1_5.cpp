#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_5.h"
#include "Stage1_4.h"

#include "Player/Player.h"
#include "Enemy/EnemyMoveWall.h"
#include "Boss/BossArmored.h"

namespace
{
	constexpr int kDownKilledNum = 5;
}

Stage1_5::Stage1_5(StageManager& mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = "Stage1-5";
	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.CreateData(m_stageName);

	m_isDownClear = m_mgr.IsClear(m_stageName, StageManager::kStageDown);
}

Stage1_5::~Stage1_5()
{
}

void Stage1_5::Init()
{
	// �o�ߎ��Ԃ̏�����
	m_frame = 0;

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

	// �X�^�[�g�ʒu�̐ݒ�
	m_boss = std::make_shared<BossArmored>(m_windowSize, m_fieldSize, 10, this);
	m_boss->Init(m_centerPos);
}

void Stage1_5::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.IsClear(m_stageName, StageManager::kStageDown) && input.IsPress("down"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, m_windowSize, m_fieldSize);

		SlideDown(nextStage);

		return;
	}
}

void Stage1_5::CheckStageConditions()
{
	// �����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.IsClear(m_stageName, StageManager::kStageDown))
	{
		if (m_mgr.GetEnemyTypeCount() >= kDownKilledNum)
		{
			m_mgr.SaveClear(m_stageName, StageManager::kStageRight);
		}
	}
}

void Stage1_5::DrawStageConditions(int drawY)
{
	if (!m_isDownClear)
	{
		DrawFormatString(128, drawY, 0xffffff, L"���@%d�̎�ނŎ���\n(%d / %d)",
			kDownKilledNum, m_mgr.GetEnemyTypeCount(), kDownKilledNum);

		drawY += 32;
	}

	// FIXME: �����Ƀ{�X��|���Ă�ꍇ�̃N���A���o�����A��ŏ����͕ʂ̂Ƃ��ɏ����悤�ɂ���
}

void Stage1_5::DrawArrow() const
{
	DrawDownArrow();
}

void Stage1_5::CreateEnemy()
{
}
