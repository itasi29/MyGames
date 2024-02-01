#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/FontSystem.h"
#include "StageManager.h"
#include "Scene/SceneManager.h"
#include "Scene/OneShotScene.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"

#include "StageTutorial.h"
#include "Stage1_1.h"

namespace
{
	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;

	const std::string kStageName = "Stage1-1";
	constexpr int kExsitTime = 2;

	// ��������
	constexpr int kCreateFrame = 300;


	// �v���C���[���S���̉�ʂ̗h��t���[��
	constexpr int kShakeFrameDeath = 10;
}

StageTutorial::StageTutorial(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_index(0),
	m_explanation(kOperation),
	m_createFrame(0)
{
	m_handle[0] = m_mgr.GetFile()->LoadGraphic(L"UI/keybordExplanation.png");
	m_handle[1] = m_mgr.GetFile()->LoadGraphic(L"UI/padExplanation.png");
	m_handle[2] = m_mgr.GetFile()->LoadGraphic(L"UI/explanation0.png");
	m_handle[3] = m_mgr.GetFile()->LoadGraphic(L"UI/explanation1.png");
	m_handle[4] = m_mgr.GetFile()->LoadGraphic(L"UI/explanation2.png");

	m_stageName = "Tutorial";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);
}

StageTutorial::~StageTutorial()
{
	// �I������ۂɃ`���[�g���A�������Ƃ������ɂ���
	m_mgr.GetStage()->SaveClear(m_stageName);
}

void StageTutorial::Init()
{
	m_frame = 0;
	m_createFrame = 0;
	m_isUpdateTime = true;

	m_player->Init();

	m_enemy.clear();
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
	m_enemy.back()->Init(m_centerPos);
}

void StageTutorial::ChangeStage(Input& input)
{
	if (m_player->IsExsit()) return;

	if (m_mgr.GetStage()->IsClearStage(kStageName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_1> nextStage = std::make_shared<Stage1_1>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
}

void StageTutorial::UpdateSelect(Input& input)
{
	m_waitFrame++;

	m_player->Update(input, kNone);

	for (auto& enemy : m_enemy)
	{
		enemy->Update();
	}

	switch (m_explanation)
	{
		// ����1��
	case kOperation:
		m_mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(m_mgr, m_handle[m_index]->GetHandle()));
		m_index++;
		if (m_index >= 2)
		{
			m_explanation = kClear;
		}
		break;
		// ����2��
	case kClear:
		m_mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(m_mgr, m_handle[m_index]->GetHandle()));
		m_index++;
		if (m_index >= 5)
		{
			m_explanation = kPlay;
		}
		break;
		// ���v���C
	case kPlay:
		if (input.IsTriggered("OK"))
		{
			ChangePlayingFunc();
			Init();
		}
		break;
	default:
		assert(false);
	case kEnd:
		break;;
	}
}

void StageTutorial::UpdatePlaying(Input& input)
{
	m_player->Update(input, m_mgr.GetStage()->GetAbility());

	// �v���C���[�̏��𔲂����
	bool playerIsDash = m_player->IsDash();
	bool playerIsExsit = m_player->IsExsit();
	const Collision& playerCol = m_player->GetRect();

	CreateEnemy();
	for (const auto& enemy : m_enemy)
	{
		enemy->Update();

		// �v���C���[�Ƃ̓����蔻�菈��
		// �_�b�V�����Ă����珈���͂��Ȃ�
		if (!playerIsDash && playerCol.IsCollsion(enemy->GetRect()))
		{
			// �v���C���[�̎��S����
			m_player->Death();
			m_mgr.GetScene()->ShakeScreen(kShakeFrameDeath);

			// �E�������Ƃ�����G���̍X�V
			m_mgr.GetStage()->UpdateEnemyType(enemy->GetName());

			break;
		}
	}

	// ���S�����G�͏���
	m_enemy.remove_if(
		[](const auto& enemy)
		{
			return !enemy->IsExsit();
		});

	UpdateTime();

	if (!m_player->IsExsit())
	{
		ChangeSelectFunc();

		// �t���[���̏�����
		m_waitFrame = 0;

		// �x�X�g�^�C���̍X�V
		m_mgr.GetStage()->UpdateBestTime(m_stageName, m_frame);

		// �N���A���Ă��邩�̊m�F
		CheckStageConditions();
	}
}

void StageTutorial::CheckStageConditions()
{
	CheckConditionsTime(kStageName, kExsitTime, L"��");
}

int StageTutorial::DrawStageConditions(int drawY)
{
	DrawTimeConditions(drawY, m_mgr.GetFont()->GetHandle(28), kExsitTime);

	return 0;
}

void StageTutorial::DrawArrow() const
{
	DrawUpArrow(false, kStageName);
}

void StageTutorial::CreateEnemy()
{
	m_createFrame++;

	if (m_createFrame > kCreateFrame)
	{
		m_createFrame = 0;

		m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
		m_enemy.back()->Init(m_centerPos);
	}
}

void StageTutorial::UpdateTime()
{
	m_frame++;
}
