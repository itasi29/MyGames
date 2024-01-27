#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "Stage1_3.h"
#include "Stage1_1.h"
#include "Stage1_4.h"

#include "Player/Player.h"
#include "Enemy/EnemyMoveWall.h"
#include "Enemy/EnemyDash.h"

namespace
{
	// ���W�A���ł�90�x
	constexpr double kRad90 = DX_PI / 2;

	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;

	// �N���A����
	constexpr int kDownExsitTime = 15;
	constexpr int kLeftKilledNum = 3;

	// �����Ԋu�t���[��
	constexpr int kCreateFrame = 60 * 6;

	const std::string kDownStName = "Stage1-1";
	const std::string kLeftStName = "Stage1-4";
}

Stage1_3::Stage1_3(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createFrame(0)
{
	m_stageName = "Stage1-3";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);
	CheckStageConditions();

	StartCheck();
}

Stage1_3::~Stage1_3()
{
}

void Stage1_3::Init()
{
	// �o�ߎ��Ԃ̏�����
	m_frame = 0;
	// �o�߂��s������������
	m_isUpdateTime = true;

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

	m_enemy.push_back(std::make_shared<EnemyDash>(m_size, m_fieldSize, m_player));
	m_enemy.back()->Init(m_centerPos);
	m_enemy.push_back(std::make_shared<EnemyDash>(m_size, m_fieldSize, m_player));
	m_enemy.back()->Init(m_centerPos);
}

void Stage1_3::StartCheck()
{
	m_isDownClear = m_mgr.GetStage()->IsClearStage(kDownStName);
	m_isLeftClear = m_mgr.GetStage()->IsClearStage(kLeftStName);
}

void Stage1_3::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.GetStage()->IsClearStage(kLeftStName) && input.IsTriggered("left"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
	if (m_mgr.GetStage()->IsClearStage(kDownStName) && input.IsTriggered("down"))
	{
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
}

void Stage1_3::CheckStageConditions()
{
	// �����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.GetStage()->IsClearStage(kLeftStName))
	{
		if (m_mgr.GetStage()->GetEnemyTypeCount() >= kLeftKilledNum)
		{
			m_mgr.GetStage()->SaveClear(kLeftStName);
			AddAchivedStr(L"��");
		}
	}
	// �����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.GetStage()->IsClearStage(kDownStName))
	{
		if (m_mgr.GetStage()->GetBestTime(m_stageName) > kDownExsitTime * 60)
		{
			m_mgr.GetStage()->SaveClear(kDownStName);
			AddAchivedStr(L"��");
		}
	}
}

int Stage1_3::DrawStageConditions(int drawY)
{
	int startY = drawY;
	int fontHandle = m_mgr.GetFont()->GetHandle(28);

	if (!m_isLeftClear)
	{
		DrawArrowConditions(kLeftStName, drawY, -kRad90);
		DrawKilledConditions(drawY, fontHandle, kLeftKilledNum);

		drawY += 68;
	}
	if (!m_isDownClear)
	{
		DrawArrowConditions(kDownStName, drawY, DX_PI);
		DrawTimeConditions(drawY, fontHandle, kDownExsitTime);

		drawY += 68;
	}

	return drawY - startY - 68;
}

void Stage1_3::DrawArrow() const
{
	DrawLeftArrow(m_isLeftClear, kLeftStName);
	DrawDownArrow(m_isDownClear, kDownStName);
}

void Stage1_3::DrawKilledEnemyType() const
{
	DrawKilledEnemy("Dash", 0, 0x0808ff);

	DrawKilledEnemy("MoveWall", 36, 0x888888);
}

void Stage1_3::CreateEnemy()
{
	m_createFrame++;

	if (m_createFrame > kCreateFrame)
	{
		m_createFrame = 0;
		m_enemy.push_back(std::make_shared<EnemyDash>(m_size, m_fieldSize, m_player));
		m_enemy.back()->Init(m_centerPos);
	}
}

void Stage1_3::UpdateTime()
{
	m_frame++;
}
