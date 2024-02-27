#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileBase.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_4.h"

#include "Player/Player.h"

namespace
{
	// ���W�A���ł�90�x
	constexpr double kRad90 = DX_PI / 2;

	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// ���F������̐F
	constexpr unsigned int kYellowColor = 0xffde00;;

	// ���߂ɐ�������G�̐�
	constexpr int kStartCreatNum = 4;
	// ���߂̐����Ԋu�t���[��
	constexpr int kStartCreateFrame = 10;

	// �G�����Ԋu�t���[��
	constexpr int kCreateFrame = 60 * 6;

	// ���N���A�����@��������
	constexpr int kLeftExsitTime = 5;
	// ��N���A�����@��������
	constexpr int kUpKilledNum = 4;

	// ���̕����̖��O
	const std::string kLeftStName = "����";
	// ��̕����̖��O
	const std::string kUpStName = "�ؒf";
}

Stage1_1::Stage1_1(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createFrame(0)
{
	m_stageName = "�T�[�N��";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);
	// 1-1�Ɋւ��Ă͏��߂���N���A���Ă��邱�ƂƂ���
	//m_mgr.GetStage()->SaveClear(m_stageName);
	CheckStageConditions(m_mgr.GetStage()->GetBestTime(m_stageName));

	StartCheck();
}

Stage1_1::~Stage1_1()
{
}

void Stage1_1::Init()
{
	StageBase::Init();

	// �����t���[���̏�����
	m_createFrame = 0;
	// �������̏�����
	m_createNum = 0;

	// �Ǔ����G�̍쐬
	CreateMoveWall();
}

void Stage1_1::StartCheck()
{
	auto& stage = m_mgr.GetStage();
	m_isLeftClear = stage->IsClearStage(kLeftStName);
	m_isUpClear = stage->IsClearStage(kUpStName);
}

void Stage1_1::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kLeftStName) && input.IsTriggered("left"))
	{
		std::shared_ptr<Stage1_2> nextStage;
		nextStage = std::make_shared<Stage1_2>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}

void Stage1_1::CheckStageConditions(int timeFrame)
{
	CheckConditionsTime(kLeftStName, timeFrame, kLeftExsitTime, L"��");
	CheckConditionsKilled(kUpStName, kUpKilledNum, L"��");
}

int Stage1_1::DrawStageConditions(int drawY) const
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
		DrawKilledConditions(drawY, fontHandle, kUpKilledNum);

		drawY += 68;
	}

	return drawY - startY - 68;
}

void Stage1_1::DrawArrow() const
{
	DrawLeftArrow(m_isLeftClear, kLeftStName);
	DrawUpArrow(m_isUpClear, kUpStName);
}

void Stage1_1::DrawEnemyKilledInfo(int x, int y) const
{
	DrawKilledEnemy("Normal", x, y, 0);
	DrawKilledEnemy("MoveWall", x, y, 36);
}

void Stage1_1::CreateEnemy()
{
	// �������Ԃ̍X�V
	m_createFrame++;

	// ���߂͐����Ԋu������
	if (m_createNum < kStartCreatNum && m_createFrame > kStartCreateFrame)
	{
		CreateNormal(m_createFrame, true);
		m_createNum++;

		return;
	}

	if (m_createFrame > kCreateFrame)
	{
		CreateNormal(m_createFrame);
	}
}

void Stage1_1::UpdateTime()
{
	m_timeFrame++;
}

