#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_3.h"

#include "Player/Player.h"
#include "Enemy/EnemyMoveWall.h"
#include "Enemy/EnemyNormal.h"
#include "Enemy/EnemyLarge.h"

#include"Enemy/EnemyDivision.h"

namespace
{
	// ���W�A���ł�90�x
	constexpr double kRad90 = DX_PI / 2;

	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// ���F������̐F
	constexpr unsigned int kYellowColor = 0xffde00;;
	// �傫���G�����Ԋu�t���[��
	constexpr int kCreateLageFrame = 60 * 10;
	// �ʏ�I�����Ԋu�t���[��
	constexpr int kCreateNormalFrame = 60 * 10;
	// �����f�B���C�t���[��1
	constexpr int kDeleyFrame1 = 60 * -3;
	// �����f�B���C�t���[��2
	constexpr int kDeleyFrame2 = 60 * -5;

	// �ʏ평�񐶐���(Normal)
	constexpr int kCreateNum = 2;


	constexpr int kRightExsitTime = 5;
	constexpr int kUpExsitTime = 10;

	const std::string kRightStName = "Stage1-1";
	const std::string kUpStName = "Stage1-3";
}

Stage1_2::Stage1_2(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createLageFrame(0)
{
	m_stageName = "Stage1-2";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);
	CheckStageConditions();

	StartCheck();
}

Stage1_2::~Stage1_2()
{
}

void Stage1_2::Init()
{
	// �o�ߎ��Ԃ̏�����
	m_frame = 0;
	// �o�߂��s������������
	m_isUpdateTime = true;

	// �����t���[���̏�����
	m_createLageFrame = 0;
	m_createNormalFrame1 = kDeleyFrame1;
	m_createNormalFrame2 = kDeleyFrame2;

	// �������̏�����
	m_createNum = 0;

	// �v���C���[�̏�����
	m_player->Init();

	// �G�̔z���������
	m_enemy.clear();

	// �Ǔ����G�̍쐬
	CreateMoveWall();

	// Large�̐���
	CreateLarge(m_createLageFrame, true);
}

void Stage1_2::StartCheck()
{
	auto& stage = m_mgr.GetStage();
	m_isRightClear = stage->IsClearStage(kRightStName);
	m_isUpClear = stage->IsClearStage(kUpStName);
}

void Stage1_2::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kRightStName) && input.IsTriggered("right"))
	{
		// ���߂Ɏ��̃X�e�[�W���쐬����
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		// ���߂Ɏ��̃X�e�[�W���쐬����
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}

void Stage1_2::CheckStageConditions()
{
	CheckConditionsTime(kRightStName, kRightExsitTime, L"�E");
	CheckConditionsTime(kUpStName, kUpExsitTime, L"��");
}

int Stage1_2::DrawStageConditions(int drawY)
{
	int startY = drawY;
	int fontHandle = m_mgr.GetFont()->GetHandle(28);

	if (!m_isRightClear)
	{
		DrawArrowConditions(kRightStName, drawY, kRad90);
		DrawTimeConditions(drawY, fontHandle, kRightExsitTime);

		drawY += 68;
	}
	if (!m_isUpClear)
	{
		DrawArrowConditions(kUpStName, drawY, 0.0);
		DrawTimeConditions(drawY, fontHandle, kUpExsitTime);

		drawY += 68;
	}

	return drawY - startY - 68;
}

void Stage1_2::DrawArrow() const
{
	DrawRightArrow(m_isRightClear, kRightStName);
	DrawUpArrow(m_isUpClear, kUpStName);
}

void Stage1_2::DrawKilledEnemyType() const
{
	DrawKilledEnemy("Normal", 0, 0xffffff);

	DrawKilledEnemy("MoveWall", 36, 0x888888);

	DrawKilledEnemy("Large", 76, 0xffffff, 20);
}

void Stage1_2::CreateEnemy()
{
	m_createNormalFrame1++;
	m_createNormalFrame2++;
	m_createLageFrame++;

	if (m_createLageFrame > kCreateLageFrame)
	{
		CreateLarge(m_createLageFrame);
	}

	if (m_createNum < kCreateNum)
	{
		m_createNum++;
		CreateNormal(m_createNormalFrame1, true);
		m_createNormalFrame1 = kDeleyFrame1;
		m_createNormalFrame2 = kDeleyFrame2;
	}
	else
	{
		if (m_createNormalFrame1 > kCreateNormalFrame)
		{
			CreateNormal(m_createNormalFrame1);
		}
		if (m_createNormalFrame2 > kCreateNormalFrame)
		{
			CreateNormal(m_createNormalFrame2);
		}
	}
}

void Stage1_2::UpdateTime()
{
	m_frame++;
}
