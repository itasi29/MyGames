#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_2.h"
#include "Stage1_1.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"

Stage1_2::Stage1_2(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = L"Stage1-2";

	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);
}

Stage1_2::~Stage1_2()
{
}

void Stage1_2::Init()
{
	// �����t���[���̏�����
	m_createFrame = 0;

	// �v���C���[�̏�����
	m_player->Init();

	// �G�̔z���������
	m_enemy.clear();
	// �G����̒ǉ�
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_windowSize, m_fieldSize));

	// �X�^�[�g�ʒu�̐ݒ�
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;
	Vec2 center{ centerX, centerY };

	m_enemy.back()->Init(center);
}

void Stage1_2::CreateEnemy()
{
}

void Stage1_2::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	if (input.IsPress("right"))
	{
		m_mgr->ChangeStage(std::make_shared<Stage1_1>(m_mgr, m_windowSize, m_fieldSize));
	}
}
