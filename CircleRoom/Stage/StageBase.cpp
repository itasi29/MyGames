#include <DxLib.h>
#include "Application.h"
#include "Input.h"
#include "StringUtility.h"

#include "StageBase.h"
#include "StageManager.h"

#include "Player/Player.h"
#include "Enemy/EnemyBase.h"
#include "Boss/BossBase.h"

StageBase::StageBase(StageManager& mgr, const Size& windowSize, float fieldSize) :
	m_centerPos( { windowSize.w * 0.5f, windowSize.h * 0.5f }),
	m_mgr(mgr),
	m_windowSize(windowSize),
	m_fieldSize(fieldSize),
	m_frame(0),
	m_waitFrame(kWaitChangeFrame)
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

	// �t���[���̑���
	m_waitFrame++;
}

void StageBase::UpdatePlaying(Input& input)
{
	m_player->Update(input);

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

			// �����o�֐��|�C���^��I���̕��ɖ߂�
			m_updateFunc = &StageBase::UpdateSelect;
			m_drawFunc = &StageBase::DrawSelect;

			// �t���[���̏�����
			m_waitFrame = 0;

			// �x�X�g�^�C���̍X�V
			m_mgr.UpdateBestTime(m_stageName, m_frame);
			// �E�������Ƃ�����G���̍X�V
			m_mgr.UpdateEnemyType(enemy->GetName());

			// �N���A���Ă��邩�̊m�F
			CheckStageConditions();

			return;
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
		m_boss->OnAttack(playerIsDash, playerCol);

		// �{�X�̎��S����
		if (!m_boss->IsExsit())
		{
			// �{�X������
			m_boss.reset();
			// �G�S�ď���
			m_enemy.clear();

			// FIXME : ����v���C���[�̎��S�����Ɠ����ɂ��Ă��邯��ǌ�ŏ����̎d���ς��Ǝv��
			m_player->Death();

			// �����o�֐��|�C���^��I���̕��ɖ߂�
			m_updateFunc = &StageBase::UpdateSelect;
			m_drawFunc = &StageBase::DrawSelect;

			// �t���[���̏�����
			m_waitFrame = 0;

			// �x�X�g�^�C���̍X�V
			m_mgr.UpdateBestTime(m_stageName, m_frame);

			// �N���A���Ă��邩�̊m�F
			CheckStageConditions();

			return;
		}

		// �v���C���[�Ƃ̔��菈��
		if (!playerIsDash && playerCol.IsCollsion(m_boss->GetRect()))
		{
			// �v���C���[�̎��S����
			m_player->Death();

			// �����o�֐��|�C���^��I���̕��ɖ߂�
			m_updateFunc = &StageBase::UpdateSelect;
			m_drawFunc = &StageBase::DrawSelect;

			// �t���[���̏�����
			m_waitFrame = 0;

			// �x�X�g�^�C���̍X�V
			m_mgr.UpdateBestTime(m_stageName, m_frame);
			// �E�������Ƃ�����G���̍X�V
			m_mgr.UpdateEnemyType(m_boss->GetName());

			// �N���A���Ă��邩�̊m�F
			CheckStageConditions();

			return;
		}
	}

	// �o�ߎ��Ԃ̍X�V
	m_frame++;
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
	// ���Ԃ̕`��
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatString(128, 32, 0xffffff, L"%02d:%02d.%03d", min, sec, minSec);
	// �X�e�[�W�����̕`��
	DrawStageConditions();

	// �x�X�g�^�C���̕`��
	int bestTime = m_mgr.GetBestTime(m_stageName);
	minSec = (bestTime * 1000 / 60) % 1000;
	sec = (bestTime / 60) % 60;
	min = bestTime / 3600;
	DrawExtendString(m_windowSize.w - 256, 32, 1.5, 1.5, L"�x�X�g�^�C��", 0xffffff);
	DrawExtendFormatString(m_windowSize.w - 256, 32 + 48, 2, 2, 0xffffff, L"%02d:%02d.%03d", min, sec, minSec);

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

void StageBase::DrawLeftArrow() const
{
	unsigned int color = 0;
	// �N���A���Ă���ꍇ�͔Z���߂�
	if (m_mgr.IsClear(m_stageName, StageManager::kStageLeft))
	{
		color = 0xffffff;
	}
	// �N���A���Ă��Ȃ��ꍇ�͔��߂�
	else
	{
		color = 0x808080;
	}
	DrawTriangle(100, m_windowSize.h / 2,
		150, m_windowSize.h / 2 + 25,
		150, m_windowSize.h / 2 - 25,
		color, true);
}

void StageBase::DrawRightArrow() const
{
	unsigned int color = 0;
	if (m_mgr.IsClear(m_stageName, StageManager::kStageRight))
	{
		color = 0xffffff;
	}
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_windowSize.w - 100, m_windowSize.h / 2,
		m_windowSize.w - 150, m_windowSize.h / 2 + 25,
		m_windowSize.w - 150, m_windowSize.h / 2 - 25,
		color, true);
}

void StageBase::DrawUpArrow() const
{
	unsigned int color = 0;
	if (m_mgr.IsClear(m_stageName, StageManager::kStageUp))
	{
		color = 0xffffff;
	}
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_windowSize.w / 2, 100,
		m_windowSize.w / 2 + 25, 150,
		m_windowSize.w / 2 - 25, 150,
		color, true);
}

void StageBase::DrawDownArrow() const
{
	unsigned int color;
	if (m_mgr.IsClear(m_stageName, StageManager::kStageDown))
	{
		color = 0xffffff;
	}
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_windowSize.w / 2, m_windowSize.h - 100,
		m_windowSize.w / 2 + 25, m_windowSize.h - 150,
		m_windowSize.w / 2 - 25, m_windowSize.h - 150,
		color, true);
}

void StageBase::SlideLeft(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:�����炭���R�[�h���������Œ�����
	// �N���A���̍X�V
	ChangeClearData(StageManager::kStageRight, nextStage->GetStageName());

	// ��ʂ�ۑ�����悤
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	// ����p�̕`�����쐬����
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	// ���̃X�e�[�W�����ɓ����Ă����炸�炷
	DrawGraph(0, m_mgr.GetSlideVolumeY(StageManager::kStageDown), nextScreenHandle, true);
	// ���݂̉摜����ɓ����Ă����炸�炷
	DrawGraph(m_windowSize.w, m_mgr.GetSlideVolumeY(StageManager::kStageUp), nowScreenHandle, true);

	// �`�������̏ꏊ�ɖ߂�
	SetDrawScreen(DX_SCREEN_BACK);

	// ���݁A���̉�ʂ�ۑ������n���h���͒��Â�ɂȂ�̂ł����ŏ���
	// ������͑������ق��ŏ������ߍl���Ȃ�
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// ��ʂ𓮂������������s����
	m_mgr.StartMove(StageManager::kStageLeft, sendScreenHandle);

	// ���̃X�e�[�W�ɕύX����
	m_mgr.ChangeStage(nextStage);
}

void StageBase::SlideRight(std::shared_ptr<StageBase> nextStage)
{
	// FIXEME: �N�\�R�[�h���������Œ�����
	// �N���A���̍X�V
	ChangeClearData(StageManager::kStageLeft, nextStage->GetStageName());

	// ��ʂ�ۑ�����悤
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(0, m_mgr.GetSlideVolumeY(StageManager::kStageUp), nowScreenHandle, true);
	DrawGraph(m_windowSize.w, m_mgr.GetSlideVolumeY(StageManager::kStageDown), nextScreenHandle, true);

	// �`�������̏ꏊ�ɖ߂�
	SetDrawScreen(DX_SCREEN_BACK);

	// ���݁A���̉�ʂ�ۑ������n���h���͒��Â�ɂȂ�̂ł����ŏ���
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);
	
	// ��ʂ𓮂������������s����
	m_mgr.StartMove(StageManager::kStageRight, sendScreenHandle);

	// ���̃X�e�[�W�ɕύX����
	m_mgr.ChangeStage(nextStage);
}

void StageBase::SlideUp(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:�����炭���R�[�h���������Œ�����
	// �N���A���̍X�V
	ChangeClearData(StageManager::kStageDown, nextStage->GetStageName());

	// ��ʂ�ۑ�����悤
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);


	// ����p�̕`�����쐬����
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(m_mgr.GetSlideVolumeX(StageManager::kStageRight), 0, nextScreenHandle, true);
	DrawGraph(m_mgr.GetSlideVolumeX(StageManager::kStageLeft), m_windowSize.h, nowScreenHandle, true);
	
	// �`�������̏ꏊ�ɖ߂�
	SetDrawScreen(DX_SCREEN_BACK);

	// ���݁A���̉�ʂ�ۑ������n���h���͒��Â�ɂȂ�̂ł����ŏ���
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// ��ʂ𓮂������������s����
	m_mgr.StartMove(StageManager::kStageUp, sendScreenHandle);

	// ���̃X�e�[�W�ɕύX����
	m_mgr.ChangeStage(nextStage);
}

void StageBase::SlideDown(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:�����炭���R�[�h���������Œ�����
	// �N���A���̍X�V
	ChangeClearData(StageManager::kStageUp, nextStage->GetStageName());

	// ��ʂ�ۑ�����悤
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	// ����p�̕`�����쐬����
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(m_mgr.GetSlideVolumeX(StageManager::kStageLeft), 0, nowScreenHandle, true);
	DrawGraph(m_mgr.GetSlideVolumeX(StageManager::kStageRight), m_windowSize.h, nextScreenHandle, true);

	// �`�������̏ꏊ�ɖ߂�
	SetDrawScreen(DX_SCREEN_BACK);

	// ���݁A���̉�ʂ�ۑ������n���h���͒��Â�ɂȂ�̂ł����ŏ���
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// ��ʂ𓮂������������s����
	m_mgr.StartMove(StageManager::kStageDown, sendScreenHandle);

	// ���̃X�e�[�W�ɕύX����
	m_mgr.ChangeStage(nextStage);
}

void StageBase::DrawWall()
{
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;

	// �F�͉�
	// ��
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0xffffff);
	// �E
	DrawLine(static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0xffffff);
	// ��
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		0xffffff);
	// ��
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0xffffff);
}

void StageBase::SlideStart(int& now, int& next, const std::shared_ptr<StageBase>& nextStage)
{
	// ���݂̉�ʂ�ۑ�����悤
	now = MakeScreen(m_windowSize.w, m_windowSize.h, true);
	// �`���ύX
	SetDrawScreen(now);
	// ���݂̉�ʂ�`��
	Draw();
	// ���̉�ʂ�ێ�����悤
	next = MakeScreen(m_windowSize.w, m_windowSize.h, true);
	SetDrawScreen(next);
	nextStage->Draw();
}

void StageBase::ChangeClearData(int dir, const std::string& name) const
{
	m_mgr.SaveClear(name, dir);
}
