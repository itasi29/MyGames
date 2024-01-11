#include <DxLib.h>
#include "Application.h"
#include "Input.h"
#include "StringUtility.h"

#include "StageBase.h"
#include "GameManager.h"
#include "StageManager.h"

#include "Player/Player.h"
#include "Enemy/EnemyBase.h"
#include "Boss/BossBase.h"

namespace
{
	// ���̓_�ŊԊu
	constexpr int kFlashInterval = 20;
}

StageBase::StageBase(GameManager& mgr, float fieldSize) :
	m_mgr(mgr),
	m_size(Application::GetInstance().GetWindowSize()),
	m_fieldSize(fieldSize),
	m_centerPos({m_size.w * 0.5f, m_size.h * 0.5f}),
	m_frame(0),
	m_waitFrame(kWaitChangeFrame),
	m_isUpdateTime(false)
{
	m_updateFunc = &StageBase::UpdateSelect;
	m_drawFunc = &StageBase::DrawSelect;
}

StageBase::~StageBase()
{
}

void StageBase::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void StageBase::Draw()
{
	(this->*m_drawFunc)();
}

void StageBase::GenericEnemy(const std::shared_ptr<EnemyBase>& enemy)
{
	m_enemy.push_back(enemy);
}

void StageBase::UpdateSelect(Input& input)
{
	// �G���������������J��Ԃ�
	for (const auto& enemy : m_enemy)
	{
		enemy->Update();
	}

	m_enemy.remove_if(
		[](const auto& enemy)
		{
			return !enemy->IsExsit();
		});
	if (m_boss)
	{
		m_boss->Update();
	}

	if (input.IsPress("OK"))
	{
		// �����o�֐��|�C���^�̍X�V
		m_updateFunc = &StageBase::UpdatePlaying;
		m_drawFunc = &StageBase::DrawPlaying;

		// �e�평��������
		Init();
	}

	m_mgr.GetStage()->ChangeAbility(kDash);

	// �t���[���̑���
	m_waitFrame++;
}

void StageBase::UpdatePlaying(Input& input)
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

	if (m_boss)
	{
		m_boss->Update();
		// �{�X�Ƀ_���[�W��^�����玞�Ԃ𑝂₷
		m_isUpdateTime = m_boss->OnAttack(playerIsDash, playerCol);

		// �{�X�̎��S����
		if (!m_boss->IsExsit())
		{
			// ����̂݃N���A�����Ɉړ�����
			BossDeath();
		}
		// �{�X���������Ă����
		// �v���C���[�Ƃ̔��菈��
		else if (!playerIsDash && playerCol.IsCollsion(m_boss->GetRect()))
		{
			// �v���C���[�̎��S����
			m_player->Death();

			// �E�������Ƃ�����G���̍X�V
			m_mgr.GetStage()->UpdateEnemyType(m_boss->GetName());
		}
	}

	UpdateTime();

	if (!m_player->IsExsit())
	{
		// �����o�֐��|�C���^��I���̕��ɖ߂�
		m_updateFunc = &StageBase::UpdateSelect;
		m_drawFunc = &StageBase::DrawSelect;

		// �t���[���̏�����
		m_waitFrame = 0;

		// �x�X�g�^�C���̍X�V
		m_mgr.GetStage()->UpdateBestTime(m_stageName, m_frame);

		// �N���A���Ă��邩�̊m�F
		CheckStageConditions();
	}
}

void StageBase::DrawSelect()
{
	DrawWall();

	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}
	if (m_boss)
	{
		m_boss->Draw();
	}

	auto name = StringUtility::StringToWString(m_stageName);
	// �X�e�[�W���̕`��
	DrawFormatString(128, 16, 0xffffff, L"%s", name.c_str());
	// �E���ꂽ���Ƃ�����G�̕`��
	DrawKilledEnemyType();
	// ���Ԃ̕`��
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatString(128, 32, 0xffffff, L"%02d:%02d.%03d", min, sec, minSec);
	// �X�e�[�W�����̕`��
	DrawStageConditions();

	// �x�X�g�^�C���̕`��
	int bestTime = m_mgr.GetStage()->GetBestTime(m_stageName);
	minSec = (bestTime * 1000 / 60) % 1000;
	sec = (bestTime / 60) % 60;
	min = bestTime / 3600;
	DrawExtendString(m_size.w - 256, 32, 1.5, 1.5, L"�x�X�g�^�C��", 0xffffff);
	DrawExtendFormatString(m_size.w - 256, 32 + 48, 2, 2, 0xffffff, L"%02d:%02d.%03d", min, sec, minSec);

	// ���̕`��
	DrawArrow();
}

void StageBase::DrawPlaying()
{
	DrawWall();

	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}
	if (m_boss)
	{
		m_boss->Draw();
	}

	// ���Ԃ̕`��
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawExtendFormatString(128, 32,	// �\���ʒu
		2, 2,	// �g�嗦
		0xffffff, // �F
		L"%02d:%02d.%03d", min, sec, minSec);	// ������
	// �����̕`��
	DrawStageConditions(64);
}

void StageBase::DrawLeftArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	unsigned int color = 0;
	// �N���A���Ă���ꍇ�͔Z���߂�
	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
		{
			color = 0xffffff;
		}
		else
		{
			color = 0xffff08;
		}
	}
	// �N���A���Ă��Ȃ��ꍇ�͔��߂�
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_size.w / 2 - 150, m_size.h / 2,
		m_size.w / 2 - 100, m_size.h / 2 + 25,
		m_size.w / 2 - 100, m_size.h / 2 - 25,
		color, true);
}

void StageBase::DrawRightArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	unsigned int color = 0;
	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
		{
			color = 0xffffff;
		}
		else
		{
			color = 0xffff08;
		}
	}
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_size.w / 2 + 150, m_size.h / 2,
		m_size.w / 2 + 100, m_size.h / 2 + 25,
		m_size.w / 2 + 100, m_size.h / 2 - 25,
		color, true);
}

void StageBase::DrawUpArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	unsigned int color = 0;
	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
		{
			color = 0xffffff;
		}
		else
		{
			color = 0xffff08;
		}
	}
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_size.w / 2, m_size.h / 2 - 150,
		m_size.w / 2 + 25, m_size.h / 2 - 100,
		m_size.w / 2 - 25, m_size.h / 2 - 100,
		color, true);
}

void StageBase::DrawDownArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	unsigned int color;
	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
		{
			color = 0xffffff;
		}
		else
		{
			color = 0xffff08;
		}
	}
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_size.w / 2, m_size.h / 2 + 150,
		m_size.w / 2 + 25, m_size.h / 2 + 100,
		m_size.w / 2 - 25, m_size.h / 2 + 100,
		color, true);
}

void StageBase::SlideLeft(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:�����炭���R�[�h���������Œ�����
	// �N���A���̍X�V
	ChangeClearData(StageManager::kStageRight, nextStage);

	// ��ʂ�ۑ�����悤
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	// ����p�̕`�����쐬����
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_size.w * 2, m_size.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	// ���̃X�e�[�W�����ɓ����Ă����炸�炷
	DrawGraph(0, m_mgr.GetStage()->GetSlideVolumeY(StageManager::kStageDown), nextScreenHandle, true);
	// ���݂̉摜����ɓ����Ă����炸�炷
	DrawGraph(m_size.w, m_mgr.GetStage()->GetSlideVolumeY(StageManager::kStageUp), nowScreenHandle, true);

	// �`�������̏ꏊ�ɖ߂�
	SetDrawScreen(DX_SCREEN_BACK);

	// ���݁A���̉�ʂ�ۑ������n���h���͒��Â�ɂȂ�̂ł����ŏ���
	// ������͑������ق��ŏ������ߍl���Ȃ�
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// ��ʂ𓮂������������s����
	m_mgr.GetStage()->StartMove(StageManager::kStageLeft, sendScreenHandle);

	// ���̃X�e�[�W�ɕύX����
	m_mgr.GetStage()->ChangeStage(nextStage);
}

void StageBase::SlideRight(std::shared_ptr<StageBase> nextStage)
{
	// FIXEME: �N�\�R�[�h���������Œ�����
	// �N���A���̍X�V
	ChangeClearData(StageManager::kStageLeft, nextStage);

	// ��ʂ�ۑ�����悤
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_size.w * 2, m_size.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(0, m_mgr.GetStage()->GetSlideVolumeY(StageManager::kStageUp), nowScreenHandle, true);
	DrawGraph(m_size.w, m_mgr.GetStage()->GetSlideVolumeY(StageManager::kStageDown), nextScreenHandle, true);

	// �`�������̏ꏊ�ɖ߂�
	SetDrawScreen(DX_SCREEN_BACK);

	// ���݁A���̉�ʂ�ۑ������n���h���͒��Â�ɂȂ�̂ł����ŏ���
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);
	
	// ��ʂ𓮂������������s����
	m_mgr.GetStage()->StartMove(StageManager::kStageRight, sendScreenHandle);

	// ���̃X�e�[�W�ɕύX����
	m_mgr.GetStage()->ChangeStage(nextStage);
}

void StageBase::SlideUp(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:�����炭���R�[�h���������Œ�����
	// �N���A���̍X�V
	ChangeClearData(StageManager::kStageDown, nextStage);

	// ��ʂ�ۑ�����悤
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);


	// ����p�̕`�����쐬����
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_size.w * 2, m_size.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(m_mgr.GetStage()->GetSlideVolumeX(StageManager::kStageRight), 0, nextScreenHandle, true);
	DrawGraph(m_mgr.GetStage()->GetSlideVolumeX(StageManager::kStageLeft), m_size.h, nowScreenHandle, true);
	
	// �`�������̏ꏊ�ɖ߂�
	SetDrawScreen(DX_SCREEN_BACK);

	// ���݁A���̉�ʂ�ۑ������n���h���͒��Â�ɂȂ�̂ł����ŏ���
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// ��ʂ𓮂������������s����
	m_mgr.GetStage()->StartMove(StageManager::kStageUp, sendScreenHandle);

	// ���̃X�e�[�W�ɕύX����
	m_mgr.GetStage()->ChangeStage(nextStage);
}

void StageBase::SlideDown(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:�����炭���R�[�h���������Œ�����
	// �N���A���̍X�V
	ChangeClearData(StageManager::kStageUp, nextStage);

	// ��ʂ�ۑ�����悤
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	// ����p�̕`�����쐬����
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_size.w * 2, m_size.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(m_mgr.GetStage()->GetSlideVolumeX(StageManager::kStageLeft), 0, nowScreenHandle, true);
	DrawGraph(m_mgr.GetStage()->GetSlideVolumeX(StageManager::kStageRight), m_size.h, nextScreenHandle, true);

	// �`�������̏ꏊ�ɖ߂�
	SetDrawScreen(DX_SCREEN_BACK);

	// ���݁A���̉�ʂ�ۑ������n���h���͒��Â�ɂȂ�̂ł����ŏ���
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// ��ʂ𓮂������������s����
	m_mgr.GetStage()->StartMove(StageManager::kStageDown, sendScreenHandle);

	// ���̃X�e�[�W�ɕύX����
	m_mgr.GetStage()->ChangeStage(nextStage);
}

void StageBase::BossDeath()
{
	// ���łɃN���A����Ă����ꍇ�͋����{�X���o��
	if (m_mgr.GetStage()->IsClearBoss(m_boss->GetName()))
	{
		CreateStrongBoss();
		return;
	}


	m_mgr.GetStage()->m_clear = true;

	// ����E���̎��͓|������I���Ƃ���
	// FIXME : ����v���C���[�̎��S�����Ɠ����ɂ��Ă��邯��ǌ�ŏ����̎d���ς��Ǝv��
	m_player->Death();

	// �����o�֐��|�C���^��I���̕��ɖ߂�
	m_updateFunc = &StageBase::UpdateSelect;
	m_drawFunc = &StageBase::DrawSelect;

	// �|�������̒ǉ�
	m_mgr.GetStage()->UpdateClearBoss(m_boss->GetName());

	// �{�X������
	m_boss.reset();
	// �G�S�ď���
	m_enemy.clear();
}

void StageBase::DrawWall()
{
	float centerX = m_size.w * 0.5f;
	float centerY = m_size.h * 0.5f;

	// �F�͉�
	DrawBox(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0xffffff, false);
}

void StageBase::SlideStart(int& now, int& next, const std::shared_ptr<StageBase>& nextStage)
{
	// ���݂̉�ʂ�ۑ�����悤
	now = MakeScreen(m_size.w, m_size.h, true);
	// �`���ύX
	SetDrawScreen(now);
	// ���݂̉�ʂ�`��
	Draw();
	// ���̉�ʂ�ێ�����悤
	next = MakeScreen(m_size.w, m_size.h, true);
	SetDrawScreen(next);
	nextStage->Draw();
}

void StageBase::ChangeClearData(int dir, const std::shared_ptr<StageBase>& nextStage) const
{
	m_mgr.GetStage()->SaveClear(nextStage->GetStageName());
	nextStage->StartCheck();
}
