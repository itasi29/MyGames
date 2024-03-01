#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileBase.h"

#include "Player/Player.h"
#include "Stage1_6.h"
#include "Stage1_4.h"
#include "Stage1_7.h"
#include "Stage1_9.h"

namespace
{
	// ���W�A���ł�90�x
	constexpr double kRad90 = DX_PI / 2;

	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// �����Ԋu�t���[��
	constexpr int kCreateEneCreateFrame = 60 * 13;
	constexpr int kCreateNormalFrame = 60 * 16;

	// �f�B���C�t���[��
	constexpr int kDeleyFrame1 = 60 * -3;
	constexpr int kDeleyFrame2 = 60 * -6;

	// ������
	constexpr int kCreateNum = 2;


	// �N���A����
	constexpr int kLeftExsitTime = 5;
	constexpr int kUpExsitTime = 10;
	constexpr int kDownKilledNum = 11;

	const std::string kLeftStName = "�ؒf";
	const std::string kUpStName = "����";
	const std::string kDownStName = "�v�x��";
}

Stage1_6::Stage1_6(GameManager& mgr, Input& input) :
	StageBase(mgr, input)
{
	m_stageName = "����";

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);

	Init();
}

Stage1_6::~Stage1_6()
{
}

void Stage1_6::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kLeftStName) && input.IsTriggered("left"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_7> nextStage;
		nextStage = std::make_shared<Stage1_7>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kDownStName) && input.IsTriggered("down"))
	{
		std::shared_ptr<Stage1_9> nextStage;
		nextStage = std::make_shared<Stage1_9>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
}
