#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileManager.h"
#include "Stage1_5.h"
#include "Stage1_4.h"

#include "Player/Player.h"
#include "Enemy/EnemyMoveWall.h"
#include "Boss/BossArmored.h"
#include "Boss/BossStrongArmored.h"

namespace
{
	// ���W�A���ł�90�x
	constexpr double kRad90 = DX_PI / 2;

	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;

	// �����̕`���ʒu
	constexpr int kConditionsPosX = 20;

	constexpr int kDownKilledNum = 5;

	const std::string kDownStName = "Stage1-4";
}

Stage1_5::Stage1_5(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createFrame(0)
{
	m_stageName = "Stage1-5";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// 1-5�̓{�X�̂���BGM��ύX����
	m_playBgm = m_mgr.GetFile()->LoadSound(L"Bgm/boss.mp3");

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);

	StartCheck();
}

Stage1_5::~Stage1_5()
{
}

void Stage1_5::Init()
{
	// �o�ߎ��Ԃ̏�����
	m_frame = 0;
	// �o�߂��s������������
	m_isUpdateTime = false;

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

	// �X�^�[�g�ʒu�̐ݒ�
	m_boss = std::make_shared<BossArmored>(m_size, m_fieldSize, this);
	m_boss->Init(m_centerPos);
}

void Stage1_5::StartCheck()
{
	m_isDownClear = m_mgr.GetStage()->IsClearStage(kDownStName);
}

void Stage1_5::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.GetStage()->IsClearStage(kDownStName) && input.IsTriggered("down"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
}

void Stage1_5::CheckStageConditions()
{
	// �����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_mgr.GetStage()->IsClearStage(kDownStName))
	{
		if (m_mgr.GetStage()->GetEnemyTypeCount() >= kDownKilledNum)
		{
			m_mgr.GetStage()->SaveClear(kDownStName);
			AddAchivedStr(L"��");
		}
	}
}

int Stage1_5::DrawStageConditions(int drawY)
{
	int startY = drawY;
	int fontHandle = m_mgr.GetFont()->GetHandle(28);

	if (!m_isDownClear)
	{
		DrawArrowConditions(kDownStName, drawY, DX_PI);
		DrawKilledConditions(drawY, fontHandle, kDownKilledNum);

		drawY += 68;
	}

	// FIXME: �����ɒǉ��ŏ����Ă��邯��ǂ��Ƃŕʂ̂Ƃ���ɏ�����ύX����
	if (m_mgr.GetStage()->IsClearBoss("BossArmored"))
	{
		DrawStringToHandle(kConditionsPosX, drawY + 14, L"clear", kWhiteColor, fontHandle);

		drawY += 68;
	}
	else
	{
		DrawStringToHandle(kConditionsPosX, drawY + 14, L"�{�X��|���I", kWhiteColor, fontHandle);

		drawY += 68;
	}

	return drawY - startY - 68;
}

void Stage1_5::DrawArrow() const
{
	DrawDownArrow(m_isDownClear, kDownStName);
}

void Stage1_5::DrawKilledEnemyType() const
{
	DrawKilledEnemy("MoveWall", 0, 0x888888);

	DrawKilledEnemy("BossArmored", 36, 0x08ff08);

	DrawKilledEnemy("BossStrongArmored", 72, 0xaaffaa);

	DrawKilledEnemy("SplitTwoBound", 108, 0xffffff, 14);
}

void Stage1_5::CreateEnemy()
{
}

void Stage1_5::UpdateTime()
{
	if (m_isUpdateTime)
	{
		// ��b�ǉ�
		m_frame += 60;
		m_isUpdateTime = false;
	}
}

void Stage1_5::CreateStrongBoss()
{
	std::shared_ptr<BossStrongArmored> strong;
	strong = std::make_shared<BossStrongArmored>(m_size, m_fieldSize, this);
	strong->Init(m_boss->GetPos());

	m_boss = strong;
}
