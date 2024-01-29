#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "Stage1_4.h"
#include "Stage1_3.h"
#include "Stage1_6.h"
#include "Stage1_8.h"
#include "Stage1_1.h"

#include "Player/Player.h"

#include "Enemy/EnemyMoveWall.h"
#include "Enemy/EnemyNormal.h"
#include "Enemy/EnemyCreate.h"

namespace
{
	// ���W�A���ł�90�x
	constexpr double kRad90 = DX_PI / 2;

	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// �����Ԋu
	constexpr int kCreateNormalFrame = 60 * 10;
	constexpr int kCreateDashFrame = 60 * 15;
	constexpr int kCreateLargeFrame = 60 * 10;

	// �f�B���C�t���[��
	constexpr int kDeleyFrame = 60 * -5;


	// �N���A����
	constexpr int kLeftExsitTime = 10;
	constexpr int kRightExsitTime = 5;
	constexpr int kUpExistTime = 90;
	constexpr int kDownKilledNum = 4;

	const std::string kLeftStName = "Stage1-3";
	const std::string kRightStName = "Stage1-6";
	const std::string kUpStName = "Stage1-8";
	const std::string kDownStName = "Stage1-1";

	const std::vector<std::string> kNames =
	{
		"Stage1-1",
		"Stage1-2",
		"Stage1-3",
		"Stage1-4",
		"Stage1-5",
		"Stage1-6",
		"Stage1-7"
	};
}

Stage1_4::Stage1_4(GameManager& mgr, Input& input) :
	StageBase(mgr, input)
{
	m_stageName = "Stage1-4";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);
	CheckStageConditions();

	StartCheck();
}

Stage1_4::~Stage1_4()
{
}

void Stage1_4::Init()
{
	// �o�ߎ��Ԃ̏�����
	m_frame = 0;
	// �o�߂��s������������
	m_isUpdateTime = true;

	// �����t���[���̏�����
	m_createNormalFrame = 0;
	m_createDashFrame = 0;
	m_createLargeFrame = 0;

	// �v���C���[�̏�����
	m_player->Init();

	// �G�̔z���������
	m_enemy.clear();

	// �Ǔ����G�̍쐬
	CreateMoveWall();

	CreateNormal(m_createNormalFrame, true);
	CreateDash(m_createDashFrame, true);
	CreateLarge(m_createLargeFrame, true);

	m_createLargeFrame = kDeleyFrame;
}

void Stage1_4::StartCheck()
{
	auto& stage = m_mgr.GetStage();
	m_isLeftClear = stage->IsClearStage(kLeftStName);
	m_isRightClear = stage->IsClearStage(kRightStName);
	m_isUpClear = stage->IsClearStage(kUpStName);
	m_isDownClear = stage->IsClearStage(kDownStName);
}

void Stage1_4::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kLeftStName) && input.IsTriggered("left"))
	{
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kRightStName) && input.IsTriggered("right"))
	{
		std::shared_ptr<Stage1_6> nextStage;
		nextStage = std::make_shared<Stage1_6>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_8> nextStage;
		nextStage = std::make_shared<Stage1_8>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kDownStName) && input.IsTriggered("down"))
	{
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}

void Stage1_4::CheckStageConditions()
{
	// FIXME:������Ɋւ��Ă̓X�e�[�W�S�̂̍��v���Ԃ��K�v�Ȃ��߁A���Ƃ�����

	CheckConditionsTime(kLeftStName, kLeftExsitTime, L"��");
	CheckConditionsTime(kRightStName, kRightExsitTime, L"�E");
	CheckConditionsSumTime(kUpStName, kNames, kUpExistTime, L"��");
	CheckConditionsKilled(kDownStName, kDownKilledNum, L"��");
}

int Stage1_4::DrawStageConditions(int drawY)
{
	int startY = drawY;
	int fontHandle = m_mgr.GetFont()->GetHandle(28);

	if (!m_isLeftClear)
	{
		DrawArrowConditions(kLeftStName, drawY, -kRad90);
		DrawTimeConditions(drawY, fontHandle, kLeftExsitTime);

		drawY += 68;
	}
	if (!m_isRightClear)
	{
		DrawArrowConditions(kRightStName, drawY, kRad90);
		DrawTimeConditions(drawY, fontHandle, kRightExsitTime);

		drawY += 68;
	}
	if (!m_isUpClear)
	{
		DrawArrowConditions(kUpStName, drawY, 0.0);
		DrawSumTimeConditions(kNames, drawY, fontHandle, kUpExistTime);

		drawY += 68;
	}
	if (!m_isDownClear)
	{
		DrawArrowConditions(kDownStName, drawY, DX_PI);
		DrawKilledConditions(drawY, fontHandle, kDownKilledNum);

		drawY += 68;
	}

	return drawY - startY - 68;
}

void Stage1_4::DrawArrow() const
{
	DrawLeftArrow(m_isLeftClear, kLeftStName);
	DrawRightArrow(m_isRightClear, kRightStName);
	DrawUpArrow(m_isUpClear, kUpStName);
	DrawDownArrow(m_isDownClear, kDownStName);
}

void Stage1_4::DrawKilledEnemyType() const
{
	DrawKilledEnemy("Normal", 0, 0xfffae7);
	DrawKilledEnemy("MoveWall", 36, 0xb6bbc4);
	DrawKilledEnemy("Dash", 72, 0x0b60b0);
	DrawKilledEnemy("Large", 112, 0xfffae7, 20);
}

void Stage1_4::CreateEnemy()
{
	m_createNormalFrame++;
	m_createDashFrame++;
	m_createLargeFrame++;

	if (m_createNormalFrame > kCreateNormalFrame)
	{
		CreateNormal(m_createNormalFrame);
	}

	if (m_createDashFrame > kCreateDashFrame)
	{
		CreateDash(m_createDashFrame);
	}

	if (m_createLargeFrame > kCreateLargeFrame)
	{
		CreateLarge(m_createLargeFrame);
	}
}

void Stage1_4::UpdateTime()
{
	m_frame++;
}
