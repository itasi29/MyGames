#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "Stage1_4.h"
#include "Stage1_3.h"
#include "Stage1_6.h"
#include "Stage1_8.h"
#include "Stage1_1.h"

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
	// �����Ԋu
	constexpr int kCreateNormalFrame = 60 * 10;
	constexpr int kCreateDashFrame = 60 * 15;
	constexpr int kCreateLargeFrame = 60 * 10;

	// �f�B���C�t���[��
	constexpr int kDeleyFrame = 60 * -5;


	// �N���A����
	constexpr int kLeftExsitTime = 10;
	constexpr int kRightExsitTime = 5;
	constexpr int kUpExistTime = 60;
	constexpr int kDownKilledNum = 4;

	const std::string kLeftStName = "�ߐڑ���";
	const std::string kRightStName = "����";
	const std::string kUpStName = "Reaper";
	const std::string kDownStName = "�T�[�N��";

	const std::vector<std::string> kNames =
	{
		"�T�[�N��",
		"����",
		"�ߐڑ���",
		"�ؒf",
		"�����i�[",
		"����",
		"����"
	};
}

Stage1_4::Stage1_4(GameManager& mgr, Input& input) :
	StageBase(mgr, input)
{
	m_stageName = "�ؒf";

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);

	Init();
}

Stage1_4::~Stage1_4()
{
}

void Stage1_4::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kLeftStName) && input.IsTriggered("left"))
	{
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kRightStName) && input.IsTriggered("right"))
	{
		std::shared_ptr<Stage1_6> nextStage;
		nextStage = std::make_shared<Stage1_6>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_8> nextStage;
		nextStage = std::make_shared<Stage1_8>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kDownStName) && input.IsTriggered("down"))
	{
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}
