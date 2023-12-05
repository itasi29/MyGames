#include <DxLib.h>
#include "Input.h"

#include "StageManager.h"
#include "Stage1_3.h"
#include "Stage1_1.h"

#include "Player/Player.h"
#include "Enemy/EnemyMoveWall.h"

namespace
{
	constexpr int kDownExsitTime = 15;
}

Stage1_3::Stage1_3(StageManager& mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = "Stage1-3";
	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.CreateData(m_stageName);
}

Stage1_3::~Stage1_3()
{
}

void Stage1_3::CheckStageConditions()
{
	// �����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.IsClear(m_stageName, StageManager::kStageDown))
	{
		if (m_frame > kDownExsitTime * 60)
		{
			m_mgr.SaveClear(m_stageName, StageManager::kStageDown);
		}
	}
}

void Stage1_3::DrawStageConditions(bool isPlaying)
{
	if (isPlaying)
	{
		DrawFormatString(128, 96, 0xffffff, L"���@%d�b�Ԑ����c��\n(%d / %d)", 
			kDownExsitTime, m_mgr.GetBestTime(m_stageName) / 60, kDownExsitTime);
	}
	else
	{
		DrawFormatString(128, 80, 0xffffff, L"���@%d�b�Ԑ����c��\n(%d / %d)", 
			kDownExsitTime, m_mgr.GetBestTime(m_stageName) / 60, kDownExsitTime);
	}
}

void Stage1_3::Init()
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
}

void Stage1_3::CreateEnemy()
{
	// �Ƃ肠�����Ȃɂ��������Ȃ�
}

void Stage1_3::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.IsClear(m_stageName, StageManager::kStageDown) && input.IsPress("down"))
	{
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, m_windowSize, m_fieldSize);

		SlideDown(nextStage);
	}
}
