#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileBase.h"

#include "Player/Player.h"
#include "Stage1_6.h"
#include "Stage1_4.h"
#include "Stage1_7.h"

namespace
{
	// ���W�A���ł�90�x
	constexpr double kRad90 = DX_PI / 2;

	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// �����Ԋu�t���[��
	constexpr int kCreateEneCreateFrame = 60 * 13;
	constexpr int kCreateNormalFrame = 60 * 16;

	// �f�B���C�t���[��
	constexpr int kDeleyFrame1 = 60 * -3;
	constexpr int kDeleyFrame2 = 60 * -6;

	// ������
	constexpr int kCreateNum = 2;


	// �N���A����
	constexpr int kLeftExsitTime = 5;
	constexpr int kUpExsitTime = 10;

	const std::string kLeftStName = "Stage1-4";
	const std::string kUpStName = "Stage1-7";
}

Stage1_6::Stage1_6(GameManager& mgr, Input& input) :
	StageBase(mgr, input)
{
	m_stageName = "Stage1-6";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);
	CheckStageConditions();

	StartCheck();
}

Stage1_6::~Stage1_6()
{
}

void Stage1_6::Init()
{
	m_frame = 0;
	m_isUpdateTime = true;

	m_createNormalFrame1 = 0;
	m_createNormalFrame2 = 0;
	m_createEneCreateFrame = 0;

	m_createNum = 0;

	m_player->Init();
	m_enemy.clear();

	CreateMoveWall();

	CreateEneCreate(m_createEneCreateFrame, true);
}

void Stage1_6::StartCheck()
{
	auto& stage = m_mgr.GetStage();
	m_isLeftClear = stage->IsClearStage(kLeftStName);
	m_isUpClear = stage->IsClearStage(kUpStName);
}

void Stage1_6::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kLeftStName) && input.IsTriggered("left"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_7> nextStage;
		nextStage = std::make_shared<Stage1_7>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
}

void Stage1_6::CheckStageConditions()
{
	CheckConditionsTime(kLeftStName, kLeftExsitTime, L"��");
	CheckConditionsTime(kUpStName, kUpExsitTime, L"��");
}

int Stage1_6::DrawStageConditions(int drawY)
{
	int startY = drawY;
	int fontHandle = m_mgr.GetFont()->GetHandle(28);
	if (!m_isLeftClear)
	{
		DrawArrowConditions(kLeftStName, drawY, -kRad90);
		DrawTimeConditions(drawY, fontHandle, kLeftExsitTime);

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

void Stage1_6::DrawArrow() const
{
	DrawLeftArrow(m_isLeftClear, kLeftStName);
	DrawUpArrow(m_isUpClear, kUpStName);
}

void Stage1_6::DrawKilledEnemyType() const
{
}

void Stage1_6::CreateEnemy()
{
	m_createNormalFrame1++;
	m_createNormalFrame2++;
	m_createEneCreateFrame++;

	if (m_createEneCreateFrame > kCreateEneCreateFrame)
	{
		CreateEneCreate(m_createEneCreateFrame);
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

void Stage1_6::UpdateTime()
{
	m_frame++;
}
