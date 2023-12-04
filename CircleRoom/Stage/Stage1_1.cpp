#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_3.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"
#include "Enemy/EnemyMoveWall.h"

namespace
{
	// �G�����Ԋu�t���[��
	constexpr int kCreateFrame = 60 * 5;

	// ���N���A�����@��������
	constexpr int kLeftExsitTime = 10;
	// ��N���A�����@��������
	constexpr int kUpExsitTime = 15;
}

Stage1_1::Stage1_1(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = L"Stage1-1";

	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);

	m_mgr->GetClearInf("1-1", m_isClear);
}

Stage1_1::~Stage1_1()
{
	m_mgr->SaveClearInf("1-1", m_isClear);
}

void Stage1_1::CheckStageConditions()
{
	// �����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_isClear[0])
	{
		// �����m�F
		if (m_frame > kLeftExsitTime * 60)
		{
			m_isClear[0] = true;
		}
	}
	// ����܂��N���A���Ă��Ȃ��ꍇ
	if (!m_isClear[1])
	{
		if (m_frame > kUpExsitTime * 60)
		{
			m_isClear[1] = true;
		}
	}
}

void Stage1_1::DrawStageConditions(bool isPlaying)
{
	if (isPlaying)
	{
		DrawFormatString(128, 64, 0xffffff, L"���@%d�b�Ԑ����c��\n(%d / %d)", kLeftExsitTime, m_bestTime / 60, kLeftExsitTime);
		DrawFormatString(128, 96, 0xffffff, L"��@%d�b�Ԑ����c��\n(%d / %d)", kUpExsitTime, m_bestTime / 60, kUpExsitTime);
	}
	else
	{
		DrawFormatString(128, 48, 0xffffff, L"���@%d�b�Ԑ����c��\n(%d / %d)", kLeftExsitTime, m_bestTime / 60, kLeftExsitTime);
		DrawFormatString(128, 80, 0xffffff, L"��@%d�b�Ԑ����c��\n(%d / %d)", kUpExsitTime, m_bestTime / 60, kUpExsitTime);
	}
}

void Stage1_1::Init()
{
	// �o�ߎ��Ԃ̏�����
	m_frame = 0;

	// �����t���[���̏�����
	m_createFrame = 0;

	// �v���C���[�̏�����
	m_player->Init();

	// �G�̔z���������
	m_enemy.clear();

	// �Ǔ����G�̍쐬
	Vec2 vec;
	// �㑤
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_windowSize, m_fieldSize));
	vec.x = 0;
	vec.y = -1;
	m_enemy.back()->Init(vec);
	// ����
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_windowSize, m_fieldSize));
	vec.y = 1;
	m_enemy.back()->Init(vec);

	// �G����̒ǉ�
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_windowSize, m_fieldSize));

	// �X�^�[�g�ʒu�̐ݒ�
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;
	vec = { centerX, centerY };

	m_enemy.back()->Init(vec);
}

void Stage1_1::CreateEnemy()
{
	// �������Ԃ̍X�V
	m_createFrame++;

	if (m_createFrame > kCreateFrame)
	{
		// �������Ԃ̏�����
		m_createFrame = 0;
		// �z��̍Ō�ɓG��ǉ�
		m_enemy.push_back(std::make_shared<EnemyNormal>(m_windowSize, m_fieldSize));
		// �����ɏ���������
		Vec2 center(m_windowSize.w * 0.5f, m_windowSize.h * 0.5f);
		m_enemy.back()->Init(center);
	}
}

void Stage1_1::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_isClear[0] && input.IsPress("left"))
	{
		// ���߂Ɏ��̃X�e�[�W���쐬����
		std::shared_ptr<Stage1_2> nextStage;
		nextStage = std::make_shared<Stage1_2>(m_mgr, m_windowSize, m_fieldSize);

		SlideLeft(nextStage);
		
		return;
	}
	if (m_isClear[1] && input.IsPress("up"))
	{
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr, m_windowSize, m_fieldSize);

		SlideUp(nextStage);

		return;
	}
}
