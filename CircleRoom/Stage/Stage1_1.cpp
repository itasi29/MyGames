#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_3.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"
#include "Enemy/EnemyMoveWall.h"

namespace
{
	// ���߂ɐ�������G�̐�
	constexpr int kStartCreatNum = 4;
	// ���߂̐����Ԋu�t���[��
	constexpr int kStartCreateFrame = 10;

	// �G�����Ԋu�t���[��
	constexpr int kCreateFrame = 60 * 6;

	// ���N���A�����@��������
	constexpr int kLeftExsitTime = 10;
	// ��N���A�����@��������
	constexpr int kUpExsitTime = 15;
}

Stage1_1::Stage1_1(StageManager& mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = "Stage1-1";
	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.CreateData(m_stageName);

	m_isLeftClear = m_mgr.IsClear(m_stageName, StageManager::kStageLeft);
	m_isUpClear = m_mgr.IsClear(m_stageName, StageManager::kStageUp);
}

Stage1_1::~Stage1_1()
{
}

void Stage1_1::Init()
{
	// �o�ߎ��Ԃ̏�����
	m_frame = 0;

	// �����t���[���̏�����
	m_createFrame = 0;
	// �������̏�����
	m_createNum = 0;

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

	// �G����̒ǉ�
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_windowSize, m_fieldSize));
	m_enemy.back()->Init(m_centerPos);
}

void Stage1_1::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.IsClear(m_stageName, StageManager::kStageLeft) && input.IsPress("left"))
	{
		// ���߂Ɏ��̃X�e�[�W���쐬����
		std::shared_ptr<Stage1_2> nextStage;
		nextStage = std::make_shared<Stage1_2>(m_mgr, m_windowSize, m_fieldSize);

		SlideLeft(nextStage);

		return;
	}
	if (m_mgr.IsClear(m_stageName, StageManager::kStageUp) && input.IsPress("up"))
	{
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr, m_windowSize, m_fieldSize);

		SlideUp(nextStage);

		return;
	}
}

void Stage1_1::CheckStageConditions()
{
	// �����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.IsClear(m_stageName, StageManager::kStageLeft))
	{
		// �����m�F
		if (m_mgr.GetBestTime(m_stageName) > kLeftExsitTime * 60)
		{
			m_mgr.SaveClear(m_stageName, StageManager::kStageLeft);
		}
	}
	// ����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.IsClear(m_stageName, StageManager::kStageUp))
	{
		if (m_mgr.GetBestTime(m_stageName) > kUpExsitTime * 60)
		{
			m_mgr.SaveClear(m_stageName, StageManager::kStageUp);
		}
	}
}

void Stage1_1::DrawStageConditions(int drawY)
{
	if (!m_isLeftClear)
	{
		DrawFormatString(128, drawY, 0xffffff, L"���@%d�b�Ԑ����c��\n(%d / %d)",
			kLeftExsitTime, m_mgr.GetBestTime(m_stageName) / 60, kLeftExsitTime);

		drawY += 32;
	}
	if (!m_isUpClear)
	{
		DrawFormatString(128, drawY, 0xffffff, L"��@%d�b�Ԑ����c��\n(%d / %d)",
			kUpExsitTime, m_mgr.GetBestTime(m_stageName) / 60, kUpExsitTime);
	}
}

void Stage1_1::DrawArrow() const
{
	DrawLeftArrow();

	DrawUpArrow();
}

void Stage1_1::CreateEnemy()
{
	// �������Ԃ̍X�V
	m_createFrame++;

	// ���߂͐����Ԋu������
	if (m_createNum < kStartCreatNum && m_createFrame > kStartCreateFrame)
	{
		CreateNormal();

		// ������������
		m_createNum++;

		return;
	}

	if (m_createFrame > kCreateFrame)
	{
		CreateNormal();
	}
}

void Stage1_1::CreateNormal()
{
	// �������Ԃ̏�����
	m_createFrame = 0;
	// �z��̍Ō�ɓG��ǉ�
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_windowSize, m_fieldSize));
	m_enemy.back()->Init(m_centerPos);
}

