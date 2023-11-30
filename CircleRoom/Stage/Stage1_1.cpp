#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_1.h"
#include "Stage1_2.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"
#include "Enemy/EnemyMoveWall.h"

namespace
{
	// �G�����Ԋu�t���[��
	constexpr int kCreateFrame = 60 * 5;
}

Stage1_1::Stage1_1(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = L"Stage1-1";

	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);
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

	// �Ǔ����G�̍쐬
	Vec2 vec;
	// �㑤
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_windowSize, m_fieldSize));
	vec.x = 0;
	vec.y = -1;
	m_enemy.back()->Init(vec);
	//// ����
	//m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_windowSize, m_fieldSize));
	//vec.y = 1;
	//m_enemy.back()->Init(vec);

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

	if (input.IsPress("left"))
	{
		// FIXME:�����ō�����z���ォ��֐�������

		// ���߂Ɏ��̃X�e�[�W���쐬����
		std::shared_ptr<Stage1_2> nextStage;
		nextStage = std::make_shared<Stage1_2>(m_mgr, m_windowSize, m_fieldSize);

		// FIXME:�����炭���R�[�h���������Œ����� /**/������Ƃ���܂�
		
		// ���݂̉�ʂ�ۑ�����悤
		int nowScreenHandle;
		nowScreenHandle = MakeScreen(m_windowSize.w, m_windowSize.h, true);
		SetDrawScreen(nowScreenHandle);
		// ���݂̉�ʂ�`��
		Draw();

		// ����p�̕`�����쐬����
		int sendScreenHandle;
		sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h, true);
		// �`����������X�N���[���ɂ���
		SetDrawScreen(sendScreenHandle);
		// ���̃X�e�[�W�̑I����ʂ�`��
		nextStage->Draw();
		// ���炵�ĕۑ��������݂̉�ʂ�`��
		DrawGraph(m_windowSize.w, 0, nowScreenHandle, true);
		// �`�������̏ꏊ�ɖ߂�
		SetDrawScreen(DX_SCREEN_BACK);

		// ���݂̉�ʂ�ۑ������n���h���͒��Â�ɂȂ�̂ł����ŏ���
		// ������͑������ق��ŏ������ߍl���Ȃ�
		DeleteGraph(nowScreenHandle);

		// ��ʂ𓮂������������s����
		m_mgr->StartMove(StageManager::kDirLeft, sendScreenHandle);

		/**/

		// ���̃X�e�[�W�ɕύX����
		m_mgr->ChangeStage(nextStage);
	}
}
