#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "Stage1_4.h"
#include "Stage1_3.h"
#include "Stage1_5.h"

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

	// �N���A����
	constexpr int kRightKilledNum = 3;
	constexpr int kUpKilledNum = 5;

	// �����Ԋu
	constexpr int kCreateNormalFrame = 60 * 4;
	constexpr int kCreateCreateFrame = 60 * 7;

	const std::string kRightStName = "Stage1-3";
	const std::string kUpStName = "Stage1-5";
}

Stage1_4::Stage1_4(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createNormalFrame(0)
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
	m_createCreateFrame = 0;

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

	m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
	m_enemy.back()->Init(m_centerPos);

	m_enemy.push_back(std::make_shared<EnemyCreate>(m_size, m_fieldSize, this));
	m_enemy.back()->Init(m_centerPos);
}

void Stage1_4::StartCheck()
{
	m_isRightClear = m_mgr.GetStage()->IsClearStage(kRightStName);
	m_isUpClear = m_mgr.GetStage()->IsClearStage(kUpStName);
}

void Stage1_4::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.GetStage()->IsClearStage(kRightStName) && input.IsTriggered("right"))
	{
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
	if (m_mgr.GetStage()->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_5> nextStage;
		nextStage = std::make_shared<Stage1_5>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
}

void Stage1_4::CheckStageConditions()
{
	// �E���܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.GetStage()->IsClearStage(kRightStName))
	{
		if (m_mgr.GetStage()->GetEnemyTypeCount() >= kRightKilledNum)
		{
			m_mgr.GetStage()->SaveClear(kRightStName);
			AddAchivedStr(L"�E");
		}
	}
	if (!m_mgr.GetStage()->IsClearStage(kUpStName))
	{
		if (m_mgr.GetStage()->GetEnemyTypeCount() >= kUpKilledNum)
		{
			m_mgr.GetStage()->SaveClear(kUpStName);
			AddAchivedStr(L"��");
		}
	}
}

int Stage1_4::DrawStageConditions(int drawY)
{
	int startY = drawY;
	int fontHandle = m_mgr.GetFont()->GetHandle(28);

	if (!m_isRightClear)
	{
		DrawArrowConditions(kRightStName, drawY, kRad90);
		DrawKilledConditions(drawY, fontHandle, kRightKilledNum);

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

void Stage1_4::DrawArrow() const
{
	DrawRightArrow(m_isRightClear, kRightStName);
	DrawUpArrow(m_isUpClear, kUpStName);
}

void Stage1_4::DrawKilledEnemyType() const
{
	DrawKilledEnemy("Normal", 0, 0xffffff);

	DrawKilledEnemy("MoveWall", 36, 0x888888);

	DrawKilledEnemy("Create", 72, 0xffff08);

	DrawKilledEnemy("Child", 108, 0xf0f008, 12);
}

void Stage1_4::CreateEnemy()
{
	m_createNormalFrame++;
	m_createCreateFrame++;

	if (m_createNormalFrame > kCreateNormalFrame)
	{
		m_createNormalFrame = 0;
		m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
		m_enemy.back()->Init(m_centerPos);
	}

	if (m_createCreateFrame > kCreateCreateFrame)
	{
		m_createCreateFrame = 0;
		m_enemy.push_back(std::make_shared<EnemyCreate>(m_size, m_fieldSize, this));
		m_enemy.back()->Init(m_centerPos);
	}
}

void Stage1_4::UpdateTime()
{
	m_frame++;
}
