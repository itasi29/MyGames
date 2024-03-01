#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileBase.h"

#include "Player/Player.h"
#include "Stage1_7.h"
#include "Stage1_8.h"
#include "Stage1_6.h"

namespace
{
	// ���W�A���ł�90�x
	constexpr double kRad90 = DX_PI / 2;

	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// �����Ԋu�t���[��
	constexpr int kCreateFrame = 60 * 10;

	// �f�B���C�t���[��
	constexpr int kDeleyFrame = 60 * -4;

	// ������
	constexpr int kCreateNum = 3;


	// �N���A����
	constexpr int kDownExsitTime = 10;
	constexpr int kLeftExsitTime = 60;

	const std::string kLeftStName = "Reaper";
	const std::string kDownStName = "����";

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

Stage1_7::Stage1_7(GameManager& mgr, Input& input) :
	StageBase(mgr, input)
{
	m_stageName = "����";

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);
	Init();
}

Stage1_7::~Stage1_7()
{
}

void Stage1_7::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kLeftStName) && input.IsTriggered("left"))
	{
		std::shared_ptr<Stage1_8> nextStage;
		nextStage = std::make_shared<Stage1_8>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kDownStName) && input.IsTriggered("down"))
	{
		std::shared_ptr<Stage1_6> nextStage;
		nextStage = std::make_shared<Stage1_6>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}
