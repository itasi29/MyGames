#include <DxLib.h>
#include "Application.h"
#include "Input.h"
#include "StringUtility.h"

#include "StageBase.h"
#include "StageManager.h"

#include "Player/Player.h"
#include "Enemy/EnemyBase.h"

StageBase::StageBase(StageManager& mgr, const Size& windowSize, float fieldSize) :
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
	// �G�l�~�[�������������J��Ԃ�
	for (const auto& enemy : m_enemy)
	{
		enemy->Update();
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
	// �v���C���[�̏��𔲂����
	bool playerIsDash = m_player->IsDash();
	bool playerIsExsit = m_player->IsExsit();
	const Rect playerRect = m_player->GetRect();

	CreateEnemy();
	for (const auto& enemy : m_enemy)
	{
		enemy->Update();

		// �v���C���[�Ƃ̓����蔻�菈��
		// �_�b�V�����Ă����珈���͂��Ȃ�
		if (!playerIsDash && playerRect.IsCollsion(enemy->GetRect()))
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
			m_mgr.UpdateKilledEnemy(enemy->GetName());

			// �N���A���Ă��邩�̊m�F
			CheckStageConditions();
		}
	}

	// ���S�����G�͏���
	//m_enemy.remove_if([](const EnemyBase& enemy)
	//	{
	//		return enemy.IsExsit();
	//	});

	// �o�ߎ��Ԃ̍X�V
	m_frame++;
	m_player->Update(input);
}

void StageBase::DrawSelect()
{
	DrawWall();

	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
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

	// ���Ԃ̕`��
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawExtendFormatString(128, 32,	// �\���ʒu
		2, 2,	// �g�嗦
		0xffffff, // �F
		L"%02d:%02d.%03d", min, sec, minSec);	// ������
	// �����̕`��
	DrawStageConditions(true);
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
		0x00ff00);
	// �E
	DrawLine(static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0x00ff00);
	// ��
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		0x00ff00);
	// ��
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0x00ff00);
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
