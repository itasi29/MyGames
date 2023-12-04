#include <DxLib.h>
#include "Input.h"

#include "Stage1_3.h"
#include "Stage1_1.h"

#include "Player/Player.h"
#include "Enemy/EnemyMoveWall.h"

Stage1_3::Stage1_3(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = L"Stage1-3";

	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);
}

Stage1_3::~Stage1_3()
{
}

void Stage1_3::CheckStageConditions()
{
}

void Stage1_3::DrawStageConditions(bool isPlaying)
{
	DrawFormatString(128, 48, 0xffffff, L"����");
}

void Stage1_3::Init()
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
}

void Stage1_3::CreateEnemy()
{
	// �Ƃ肠�����Ȃɂ��������Ȃ�
}

void Stage1_3::ChangeStage(Input& input)
{
	// �v���C���[���������Ă���Ԃ͕ς��Ȃ��悤�ɂ���
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;

	if (input.IsPress("down"))
	{
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, m_windowSize, m_fieldSize);

		SlideDown(nextStage);
	}
}
