#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "Stage1_1.h"
#include "Stage1_2.h"

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

	// �E����������
	constexpr int kRightExsitTime = 10;

	// �傫���G�����Ԋu�t���[��
	constexpr int kCreateLageFrame = 60 * 10;
	// �ʏ�I�����Ԋu�t���[��
	constexpr int kCreateNormalFrame = 60 * 5 + 10;

	const std::string kRightStName = "Stage1-1";
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
	m_createNormalFrame = 0;

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

	// �傫���G����
	CreateLage();
	// �ʏ�G����
	CreateNormal();
	CreateNormal();
}

void Stage1_2::StartCheck()
{
	m_isRightClear = m_mgr.GetStage()->IsClearStage(kRightStName);
}

void Stage1_2::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.GetStage()->IsClearStage(kRightStName) && input.IsTriggered("right"))
	{
		// ���߂Ɏ��̃X�e�[�W���쐬����
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
}

void Stage1_2::CheckStageConditions()
{
	// �E���܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.GetStage()->IsClearStage(kRightStName))
	{
		// �����m�F
		if (m_mgr.GetStage()->GetBestTime(m_stageName) > kRightExsitTime * 60)
		{
			m_mgr.GetStage()->SaveClear(kRightStName);
			AddAchivedStr(L"�E");
		}
	}
}

int Stage1_2::DrawStageConditions(int drawY)
{
	int startY = drawY;
	if (!m_isRightClear)
	{
		int fontHandle = m_mgr.GetFont()->GetHandle(28);

		DrawArrowConditions(kRightStName, drawY, kRad90);
		DrawTimeConditions(drawY, fontHandle, kRightExsitTime);

		drawY += 68;
	}

	return drawY - startY - 68;
}

void Stage1_2::DrawArrow() const
{
	DrawRightArrow(m_isRightClear, kRightStName);
}

void Stage1_2::DrawKilledEnemyType() const
{
	DrawKilledEnemy("Normal", 0, 0xffffff);

	DrawKilledEnemy("MoveWall", 36, 0x888888);

	DrawKilledEnemy("Large", 76, 0xffffff, 20);
}

void Stage1_2::CreateEnemy()
{
	m_createNormalFrame++;
	m_createLageFrame++;

	if (m_createNormalFrame > kCreateNormalFrame)
	{
		CreateNormal();
	}

	if (m_createLageFrame > kCreateLageFrame)
	{
		CreateLage();
	}
}

void Stage1_2::CreateNormal()
{
	// �������Ԃ̏�����
	m_createNormalFrame = 0;
	// �z��̍Ō�ɓG��ǉ�
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
	m_enemy.back()->Init(m_centerPos);
}

void Stage1_2::CreateLage()
{
	m_createLageFrame = 0;
	m_enemy.push_back(std::make_shared<EnemyLarge>(m_size, m_fieldSize));
	m_enemy.back()->Init(m_centerPos);
}

void Stage1_2::UpdateTime()
{
	m_frame++;
}
