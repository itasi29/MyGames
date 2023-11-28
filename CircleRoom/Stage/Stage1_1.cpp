#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_1.h"
#include "Stage1_2.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"

namespace
{
	// �G�����Ԋu�t���[��
	constexpr int kCreateFrame = 60 * 5;
}

Stage1_1::Stage1_1(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);
	
	Init();
}

Stage1_1::~Stage1_1()
{
}

void Stage1_1::Init()
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
		float centerX = m_windowSize.w * 0.5f;
		float centerY = m_windowSize.h * 0.5f;
		m_enemy.back()->Init(Vec2{ centerX, centerY });
	}
}

void Stage1_1::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	if (input.IsPress("left"))
	{
		m_mgr->ChangeStage(std::make_shared<Stage1_2>(m_mgr, m_windowSize, m_fieldSize));
	}
}
