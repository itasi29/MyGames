#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileBase.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_3.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"
#include "Enemy/EnemyMoveWall.h"

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
	constexpr int kLeftExsitTime = 10;
	// ��N���A�����@��������
	constexpr int kUpExsitTime = 15;

	// ���̕����̖��O
	const std::string kLeftStName = "Stage1-2";
	// ��̕����̖��O
	const std::string kUpStName = "Stage1-3";
}

Stage1_1::Stage1_1(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createFrame(0)
{
	m_stageName = "Stage1-1";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);
	// 1-1�Ɋւ��Ă͏��߂���N���A���Ă��邱�ƂƂ���
	//m_mgr.GetStage()->SaveClear(m_stageName);
	CheckStageConditions();

	StartCheck();
}

Stage1_1::~Stage1_1()
{
}

void Stage1_1::Init()
{
	// �o�ߎ��Ԃ̏�����
	m_frame = 0;
	// �o�߂��s������������
	m_isUpdateTime = true;

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
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_size, m_fieldSize));
	vec.x = 0;
	vec.y = -1;
	m_enemy.back()->Init(vec);
	// ����
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_size, m_fieldSize));
	vec.y = 1;
	m_enemy.back()->Init(vec);

	//// �G����̒ǉ�
	//m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
	//m_enemy.back()->Init(m_centerPos);
	//m_createNum++;
}

void Stage1_1::StartCheck()
{
	m_isLeftClear = m_mgr.GetStage()->IsClearStage(kLeftStName);
	m_isUpClear = m_mgr.GetStage()->IsClearStage(kUpStName);
}

void Stage1_1::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.GetStage()->IsClearStage(kLeftStName) && input.IsTriggered("left"))
	{
		std::shared_ptr<Stage1_2> nextStage;
		nextStage = std::make_shared<Stage1_2>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
	if (m_mgr.GetStage()->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
}

void Stage1_1::CheckStageConditions()
{
	// �����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.GetStage()->IsClearStage(kLeftStName))
	{
		// �����m�F
		if (m_mgr.GetStage()->GetBestTime(m_stageName) > kLeftExsitTime * 60)
		{
			m_mgr.GetStage()->SaveClear(kLeftStName);
			AddAchivedStr(L"��");
		}
	}
	// ����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.GetStage()->IsClearStage(kUpStName))
	{
		if (m_mgr.GetStage()->GetBestTime(m_stageName) > kUpExsitTime * 60)
		{
			m_mgr.GetStage()->SaveClear(kUpStName);
			AddAchivedStr(L"��");
		}
	}
}

int Stage1_1::DrawStageConditions(int drawY)
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

void Stage1_1::DrawArrow() const
{
	DrawLeftArrow(m_isLeftClear, kLeftStName);
	DrawUpArrow(m_isUpClear, kUpStName);
}

void Stage1_1::DrawKilledEnemyType() const
{
	DrawKilledEnemy("Normal", 0, 0xffffff);

	DrawKilledEnemy("MoveWall", 36, 0x888888);
}

void Stage1_1::CreateEnemy()
{
	// �������Ԃ̍X�V
	m_createFrame++;

	// ���߂͐����Ԋu������
	if (m_createNum < kStartCreatNum && m_createFrame > kStartCreateFrame)
	{
		CreateNormal(true);

		// ������������
		m_createNum++;

		return;
	}

	if (m_createFrame > kCreateFrame)
	{
		CreateNormal(false);
	}
}

void Stage1_1::CreateNormal(bool isStart)
{
	// �������Ԃ̏�����
	m_createFrame = 0;
	// �z��̍Ō�ɓG��ǉ�
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
	m_enemy.back()->Init(m_centerPos, isStart);
}

void Stage1_1::UpdateTime()
{
	m_frame++;
}

