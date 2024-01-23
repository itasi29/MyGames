#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "Input.h"
#include "StringUtility.h"

#include "StageBase.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "StageManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/BottansFile.h"
#include "FileSystem/KeyFile.h"

#include "Player/Player.h"
#include "Enemy/EnemyBase.h"
#include "Boss/BossBase.h"

namespace
{
	// �t���[���̐F
	constexpr unsigned int kFrameColor = 0xd80032;

	// �t�B�[���h�T�C�Y�̔{��
	// �t�B�[���h��windowsize�̏c���ɔ{�������������̂Ƃ���
	constexpr float kSizeScale = 0.4f;

	// ������̐F
	constexpr unsigned int kStrColor = 0xf0ece5;

	// ���̓_�ŊԊu
	constexpr int kFlashInterval = 20;

	// �v���C���[���S���̉�ʂ̗h��t���[��
	constexpr int kShakeFrameDeath = 10;

	// �T�E���h�̃t�F�[�h�t���[��
	constexpr int kSoundFade = 30;
}

StageBase::StageBase(GameManager& mgr, Input& input) :
	m_mgr(mgr),
	m_input(input),
	m_size(Application::GetInstance().GetWindowSize()),
	m_fieldSize(m_size.h* kSizeScale),
	m_centerPos({m_size.w * 0.5f, m_size.h * 0.5f}),
	m_soundFrame(kSoundFade),
	m_frame(0),
	m_waitFrame(kWaitChangeFrame),
	m_isUpdateTime(false)
{
	m_updateFunc = &StageBase::UpdateSelect;
	m_drawFunc = &StageBase::DrawSelect;

	// ��O������true�ɂ��Ă����Ȃ��ƍ������ʂ����߂��Ȃ�
	m_strHandle = MakeScreen(m_size.w, m_size.h, true);

	m_sound = m_mgr.GetSound();

	auto& file = m_mgr.GetFile();
	m_bFrameImg = file->LoadGraphic(L"UI/backFrame.png");
	m_selectBgm = file->LoadSound(L"Bgm/provisionalBgm.mp3");
	m_playBgm = file->LoadSound(L"Bgm/fieldFight.mp3");

	m_bt = std::make_shared<BottansFile>(file);
	m_key = std::make_shared<KeyFile>(file);
}

StageBase::~StageBase()
{
	DeleteGraph(m_strHandle);
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
	if (m_soundFrame > kSoundFade)
	{
		m_sound->PlayBgm(m_selectBgm->GetHandle());
	}
	else
	{
		m_soundFrame++;
		m_sound->PlayFadeBgm(m_selectBgm->GetHandle(), m_soundFrame / static_cast<float>(kSoundFade));
	}

	m_player->Update(input, kNone);

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

		m_sound->Stop(m_selectBgm->GetHandle());
		m_soundFrame = 0;

		// �e�평��������
		Init();
	}

	m_mgr.GetStage()->ChangeAbility(kDash);

	// �t���[���̑���
	m_waitFrame++;
}

void StageBase::UpdatePlaying(Input& input)
{
	if (m_soundFrame > kSoundFade)
	{
		m_sound->PlayBgm(m_playBgm->GetHandle());
	}
	else
	{
		m_soundFrame++;
		m_sound->PlayFadeBgm(m_playBgm->GetHandle(), m_soundFrame / static_cast<float>(kSoundFade));
	}

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
			m_mgr.UpdateDeathCcount();

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
			m_mgr.GetScene()->ShakeScreen(kShakeFrameDeath);
			m_mgr.UpdateDeathCcount();

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

		// ���֌W�̐ݒ�
		m_soundFrame = 0;
		m_sound->Stop(m_playBgm->GetHandle());

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

	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}
	if (m_boss)
	{
		m_boss->Draw();
	}
	m_player->Draw();
	auto name = StringUtility::StringToWString(m_stageName);
	// �X�e�[�W���̕`��
	DrawFormatStringToHandle(128, 16, kStrColor, m_mgr.GetFont()->GetHandle(64), L"%s", name.c_str());

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	// ���Ԃ̕`��
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatStringToHandle(128, 96, kStrColor, fontHandle, L"%02d:%02d.%03d", min, sec, minSec);
	// �E���ꂽ���Ƃ�����G�̕`��
	DrawKilledEnemyType();
	SetDrawScreen(DX_SCREEN_BACK);
	// �㉺���]���ĕ`��t���[��
	DrawRotaGraph(155, 120, 1.0, 0.0, m_bFrameImg->GetHandle(), true, false, true);
	DrawBox(0, 120 + 30, 211 + 100, 120 + 30 + 30, 0xB6BBC4, true);
	DrawGraph(0, 0, m_strHandle, true);

	// �X�e�[�W�����̕`��
	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	auto y = DrawStageConditions(196);
	SetDrawScreen(DX_SCREEN_BACK);
	// �������ɂ���t���[���w�i��`�悷��
	if (y >= 0)
	{
		DrawBox(0, 196, 311, 196 + y, 0xb6bbc4, true);
		DrawGraph(0, 196 + y, m_bFrameImg->GetHandle(), true);
	}
	DrawGraph(0, 0, m_strHandle, true);

	// �t���[���`��
	DrawRotaGraph(m_size.w - 128, 48, 1.0, 0.0, m_bFrameImg->GetHandle(), true, true, true);
	DrawBox(m_size.w - 128 - 156, 78, m_size.w, 144, 0xb6bbc4, true);
	// �x�X�g�^�C���̕`��
	int bestTime = m_mgr.GetStage()->GetBestTime(m_stageName);
	minSec = (bestTime * 1000 / 60) % 1000;
	sec = (bestTime / 60) % 60;
	min = bestTime / 3600;
	DrawExtendStringToHandle(m_size.w - 256, 32, 1.5, 1.5, L"�x�X�g�^�C��", kStrColor, fontHandle);
	DrawExtendFormatStringToHandle(m_size.w - 256, 32 + 48, 2, 2, kStrColor, fontHandle, L"%02d:%02d.%03d", min, sec, minSec);

	// ���̕`��
	DrawArrow();

	DrawImage();
}

void StageBase::DrawPlaying()
{
	DrawWall();

	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}
	if (m_boss)
	{
		m_boss->Draw();
	}
	m_player->Draw();

	// ���Ԃ̕`��
	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawExtendFormatString(128, 32,	// �\���ʒu
		2, 2,	// �g�嗦
		kStrColor, // �F
		L"%02d:%02d.%03d", min, sec, minSec);	// ������
	SetDrawScreen(DX_SCREEN_BACK);
	DrawRotaGraph(155, 48, 1.0, 0.0, m_bFrameImg->GetHandle(), true, false, true);
	DrawGraph(0, 0, m_strHandle, true);

	// �����̕`��
	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	auto y = DrawStageConditions(196+24);
	SetDrawScreen(DX_SCREEN_BACK);
	// MEMO:�������ɂ���t���[���w�i��`�悷��
	if (y >= 0)
	{
		DrawBox(0, 196 + 24, 311, 196 + 24 + y, 0xb6bbc4, true);
		DrawGraph(0, 196 + 24 + y, m_bFrameImg->GetHandle(), true);
	}
	DrawGraph(0, 0, m_strHandle, true);

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
	ChangeClearData(nextStage);

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
	ChangeClearData(nextStage);

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
	ChangeClearData(nextStage);

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
	ChangeClearData(nextStage);

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

void StageBase::ChangeSelectFunc()
{
	m_updateFunc = &StageBase::UpdateSelect;
	m_drawFunc = &StageBase::DrawSelect;
}

void StageBase::ChangePlayingFunc()
{
	m_updateFunc = &StageBase::UpdatePlaying;
	m_drawFunc = &StageBase::DrawPlaying;
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

void StageBase::DrawImage()
{
	DrawBox(1000, 600, 1280, 632, kFrameColor, true);
	switch (m_input.GetType())
	{
	case InputType::keybd:
		m_key->DrawKey(m_input.GetHardDataName("OK", InputType::keybd), 1016, 600, 2.0);
		break;
	default:
		assert(false);
	case InputType::pad:
		m_bt->DrawBottan(m_input.GetHardDataName("OK", InputType::pad), 1016, 600, 2.0);
		break;
	}
	DrawStringToHandle(1064, 600, L"�I��", kStrColor, m_mgr.GetFont()->GetHandle(32));
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

void StageBase::ChangeClearData(const std::shared_ptr<StageBase>& nextStage) const
{
	m_mgr.GetStage()->SaveClear(nextStage->GetStageName());
	nextStage->StartCheck();
}
