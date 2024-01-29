#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileBase.h"
#include "Stage1_9.h"
#include "Stage1_6.h"

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

	// ��N���A�����@��������
	constexpr int kUpExistTime = 5;

	// ��̕����̖��O
	const std::string kUpStName = "Stage1-6";
}

Stage1_9::Stage1_9(GameManager& mgr, Input& input) :
	StageBase(mgr, input)
{
	m_stageName = "Stage1-9";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);
	// 1-1�Ɋւ��Ă͏��߂���N���A���Ă��邱�ƂƂ���
	//m_mgr.GetStage()->SaveClear(m_stageName);
	CheckStageConditions();

	StartCheck();
}

Stage1_9::~Stage1_9()
{
}

void Stage1_9::Init()
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
	CreateMoveWall();
}

void Stage1_9::StartCheck()
{
	auto& stage = m_mgr.GetStage();
	m_isUpClear = stage->IsClearStage(kUpStName);
}

void Stage1_9::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_6> nextStage;
		nextStage = std::make_shared<Stage1_6>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}

void Stage1_9::CheckStageConditions()
{
	CheckConditionsTime(kUpStName, kUpExistTime, L"��");
}

int Stage1_9::DrawStageConditions(int drawY)
{
	int startY = drawY;
	int fontHandle = m_mgr.GetFont()->GetHandle(28);
	if (!m_isUpClear)
	{
		DrawArrowConditions(kUpStName, drawY, -kRad90);
		DrawTimeConditions(drawY, fontHandle, kUpExistTime);

		drawY += 68;
	}

	return drawY - startY - 68;
}

void Stage1_9::DrawArrow() const
{
	DrawUpArrow(m_isUpClear, kUpStName);
}

void Stage1_9::DrawKilledEnemyType() const
{
}

void Stage1_9::CreateEnemy()
{
}

void Stage1_9::UpdateTime()
{
	m_frame++;
}
