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
	constexpr int kCreatDashNum = 2;
	constexpr int kCreatEneCreateNum = 4;
	constexpr int kCreateLargeNum = 3;

	// �����Ԋu
	constexpr int kCreateDashInterval = static_cast<int>(60 * 6.5);
	constexpr int kCreateEneCreateInterval = 60 * 12;
	constexpr int kCreateDivisionInterval = 60 * 8;
	constexpr int kCreateLargeInterval = 60 * 4;

	// �f�B���C�t���[��
	constexpr int kStartDeleyFrame = 40;
	constexpr int kDeleyFrame = 60 * 6;

	// ��N���A�����@��������
	constexpr int kUpExistTime = 5;

	// ��̕����̖��O
	const std::string kUpStName = "����";
}

Stage1_9::Stage1_9(GameManager& mgr, Input& input) :
	StageBase(mgr, input)
{
	m_stageName = "�v�x��";

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);
	Init();
}

Stage1_9::~Stage1_9()
{
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
