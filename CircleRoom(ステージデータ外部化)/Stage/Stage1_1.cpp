#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileBase.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_4.h"

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

	// ���N���A�����@��������
	constexpr int kLeftExsitTime = 5;
	// ��N���A�����@��������
	constexpr int kUpKilledNum = 4;

	// ���̕����̖��O
	const std::string kLeftStName = "����";
	// ��̕����̖��O
	const std::string kUpStName = "�ؒf";
}

Stage1_1::Stage1_1(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createFrame(0)
{
	m_stageName = "�T�[�N��";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);

	Init();
}

Stage1_1::~Stage1_1()
{
}

void Stage1_1::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kLeftStName) && input.IsTriggered("left"))
	{
		std::shared_ptr<Stage1_2> nextStage;
		nextStage = std::make_shared<Stage1_2>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}
