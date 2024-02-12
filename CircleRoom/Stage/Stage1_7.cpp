#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileBase.h"

#include "Player/Player.h"
#include "Stage1_7.h"
#include "Stage1_8.h"
#include "Stage1_6.h"

namespace
{
	// ���W�A���ł�90�x
	constexpr double kRad90 = DX_PI / 2;

	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// �����Ԋu�t���[��
	constexpr int kCreateFrame = 60 * 10;

	// �f�B���C�t���[��
	constexpr int kDeleyFrame = 60 * -4;

	// ������
	constexpr int kCreateNum = 3;


	// �N���A����
	constexpr int kDownExsitTime = 10;
	constexpr int kLeftExsitTime = 60;

	const std::string kLeftStName = "StageBoss";
	const std::string kDownStName = "Stage1-6";

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

Stage1_7::Stage1_7(GameManager& mgr, Input& input) :
	StageBase(mgr, input)
{
	m_stageName = "Stage1-7";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);
	CheckStageConditions(m_mgr.GetStage()->GetBestTime(m_stageName));

	StartCheck();
}

Stage1_7::~Stage1_7()
{
}

void Stage1_7::Init()
{
	StageBase::Init();

	// �����t���[���̏�����
	m_createFrame = 0;
	// �������̏�����
	m_createNum = 0;

	// �Ǔ����G�̍쐬
	CreateMoveWall();
}

void Stage1_7::StartCheck()
{
	m_isLeftClear = m_mgr.GetStage()->IsClearStage(kLeftStName);
	m_isDownClear = m_mgr.GetStage()->IsClearStage(kDownStName);
}

void Stage1_7::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kLeftStName) && input.IsTriggered("left"))
	{
		std::shared_ptr<Stage1_8> nextStage;
		nextStage = std::make_shared<Stage1_8>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kDownStName) && input.IsTriggered("down"))
	{
		std::shared_ptr<Stage1_6> nextStage;
		nextStage = std::make_shared<Stage1_6>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}

void Stage1_7::CheckStageConditions(int timeFrame)
{
	CheckConditionsSumTime(kLeftStName, kNames, timeFrame, kLeftExsitTime, L"��");
	CheckConditionsKilled(kDownStName, kDownExsitTime, L"��");
}

int Stage1_7::DrawStageConditions(int drawY) const
{
	int startY = drawY;
	int fontHandle = m_mgr.GetFont()->GetHandle(28);
	if (!m_isLeftClear)
	{
		DrawArrowConditions(kLeftStName, drawY, -kRad90);
		DrawSumTimeConditions(kNames, drawY, fontHandle, kLeftExsitTime);

		drawY += 102;
	}
	if (!m_isDownClear)
	{
		DrawArrowConditions(kDownStName, drawY, DX_PI);
		DrawKilledConditions(drawY, fontHandle, kDownExsitTime);

		drawY += 68;
	}

	return drawY - startY - 68;
}

void Stage1_7::DrawArrow() const
{
	DrawLeftArrow(m_isLeftClear, kLeftStName, true, m_mgr.GetStage()->IsClearStage(kLeftStName));
	DrawDownArrow(m_isDownClear, kDownStName);
}

void Stage1_7::DrawKilledEnemyType(int x, int y) const
{
	DrawKilledEnemy("MoveWall", x, y, 0, 0xb6bbc4);
	DrawKilledEnemy("Division", x, y, 42, 0x80bcbd);
	DrawKilledEnemy("Split", x, y, 84, 0xd5f0c1, 12);
}

void Stage1_7::CreateEnemy()
{
	m_createFrame++;

	if (m_createNum < kCreateNum)
	{
		m_createNum++;
		CreateDivision(m_createFrame, true);
		m_createFrame = kDeleyFrame;
		return;
	}

	if (m_createFrame > kCreateFrame)
	{
		CreateDivision(m_createFrame);
	}
}

void Stage1_7::UpdateTime()
{
	m_frame++;
}
