#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_3.h"

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
	// �傫���G�����Ԋu�t���[��
	constexpr int kCreateLageFrame = 60 * 10;
	// �ʏ�I�����Ԋu�t���[��
	constexpr int kCreateNormalFrame = 60 * 10;
	// �����f�B���C�t���[��1
	constexpr int kDeleyFrame1 = 60 * -3;
	// �����f�B���C�t���[��2
	constexpr int kDeleyFrame2 = 60 * -5;

	// �ʏ평�񐶐���(Normal)
	constexpr int kCreateNum = 2;


	constexpr int kRightExsitTime = 5;
	constexpr int kUpExsitTime = 10;

	const std::string kRightStName = "�T�[�N��";
	const std::string kUpStName = "�ߐڑ���";
}

Stage1_2::Stage1_2(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createLageFrame(0)
{
	m_stageName = "����";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);

	Init();
}

Stage1_2::~Stage1_2()
{
}

void Stage1_2::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kRightStName) && input.IsTriggered("right"))
	{
		// ���߂Ɏ��̃X�e�[�W���쐬����
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		// ���߂Ɏ��̃X�e�[�W���쐬����
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}

void Stage1_2::UpdateTime()
{
	m_timeFrame++;
}
