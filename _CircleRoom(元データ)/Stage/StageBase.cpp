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

	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// ����������̐F
	constexpr unsigned int kYellowColor = 0xffde00;
	// �o�b�N�t���[���̐F
	constexpr unsigned int kBackFrameColor = 0x161a30;

	// ���̓_�ŊԊu
	constexpr int kFlashInterval = 20;

	// �v���C���[���S���̉�ʂ̗h��t���[��
	constexpr int kShakeFrameDeath = 10;

	// �T�E���h�̃t�F�[�h�t���[��
	constexpr int kSoundFade = 30;

	// ���W�A���ł�90�x
	constexpr double kRad90 = DX_PI / 2;

	// �����B�����̕`�掞��("���̏����B���̕���")
	constexpr int kAchivedFrame = 120;
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
	m_field = file->LoadGraphic(L"field.png");
	m_arrow = file->LoadGraphic(L"UI/arrow.png");
	m_arrowFlash = file->LoadGraphic(L"UI/arrowFlash.png");
	m_arrowNo = file->LoadGraphic(L"UI/arrowNo.png");
	m_arrowConditions = file->LoadGraphic(L"UI/arrowConditions.png");
	m_startFrame = file->LoadGraphic(L"UI/startFrame.png");
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

	for (auto& achived : m_achived)
	{
		achived.frame++;
	}
	m_achived.remove_if(
		[](const auto& achived)
		{
			return achived.frame > kAchivedFrame;
		}
	);

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
	DrawBox(0, 28, 256, 104, kFrameColor, true);
	DrawFormatStringToHandle(64, 32, kYellowColor, m_mgr.GetFont()->GetHandle(64), L"%s", name.c_str());

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	// ���Ԃ̕`��
	DrawTime(20, 160, fontHandle);
	// �E���ꂽ���Ƃ�����G�̕`��
	DrawStringToHandle(136, 160, L"> Circle", kWhiteColor, m_mgr.GetFont()->GetHandle(24));
	DrawKilledEnemyType();
	SetDrawScreen(DX_SCREEN_BACK);
	// �㉺���]���ĕ`��t���[��
	DrawRotaGraph(155, 168, 1.0, 0.0, m_bFrameImg->GetHandle(), true, false, true);
	DrawBox(0, 198, 310, 198 + 30, kBackFrameColor, true);
	DrawGraph(0, 0, m_strHandle, true);

	// �X�e�[�W�����̕`��
	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	auto y = DrawStageConditions(256);
	SetDrawScreen(DX_SCREEN_BACK);
	// �������ɂ���t���[���w�i��`�悷��
	if (y >= 0)
	{
		DrawBox(0, 244, 310, 244 + y, kBackFrameColor, true);
		DrawGraph(0, 244 + y, m_bFrameImg->GetHandle(), true);
	}
	DrawGraph(0, 0, m_strHandle, true);

	// �t���[���`��
	DrawRotaGraph(m_size.w - 128, 128, 1.0, 0.0, m_bFrameImg->GetHandle(), true, true, true);
	DrawBox(m_size.w - 128 - 155, 158, m_size.w, 224, kBackFrameColor, true);
	// �x�X�g�^�C���̕`��
	int bestTime = m_mgr.GetStage()->GetBestTime(m_stageName);
	int minSec = (bestTime * 1000 / 60) % 1000;
	int sec = (bestTime / 60) % 60;
	int min = bestTime / 3600;
	DrawStringToHandle(m_size.w - 256, 112, L"> �x�X�g�^�C��", kWhiteColor, m_mgr.GetFont()->GetHandle(32));
	DrawFormatStringToHandle(m_size.w - 256, 112 + 48, kYellowColor, m_mgr.GetFont()->GetHandle(64), L"%02d:%02d.%03d", min, sec, minSec);

	// ���̕`��
	DrawArrow();

	// �X�^�[�g�̉摜�̕`��
	DrawImage();

	// �����B�����̂�`��
	DrawConditionsAchived();
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

	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	// ���Ԃ̕`��
	DrawTime(20, 144, m_mgr.GetFont()->GetHandle(64));
	SetDrawScreen(DX_SCREEN_BACK);
	DrawRotaGraph(155, 168, 1.0, 0.0, m_bFrameImg->GetHandle(), true, false, true);
	DrawBox(0, 198, 310, 198 + 36, kBackFrameColor, true);
	DrawGraph(0, 0, m_strHandle, true);

	// �����̕`��
	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	auto y = DrawStageConditions(244+16+20);
	SetDrawScreen(DX_SCREEN_BACK);
	// MEMO:�������ɂ���t���[���w�i��`�悷��
	if (y >= 0)
	{
		DrawBox(0, 268, 311, 268 + y, kBackFrameColor, true);
		DrawGraph(0, 268 + y, m_bFrameImg->GetHandle(), true);
	}
	DrawGraph(0, 0, m_strHandle, true);

}

void StageBase::DrawArrowConditions(const std::string& nextStName, int x, int y, double angle, bool isReverseX, bool isReverxeY)
{
	if (m_mgr.GetStage()->IsClearStage(nextStName) && (m_waitFrame / kFlashInterval) % 2 != 0)
	{
		DrawBox(x, y, x + 28, y + 28, 0xffde00, true);
	}
	DrawRotaGraph(x + 14, y + 14, 1.0, angle, m_arrowConditions->GetHandle(), true, isReverseX, isReverxeY);
}

void StageBase::DrawTimeConditions(int x, int y, int handle, int existTime)
{
	DrawStringToHandle(x, y, L"�@�@   �b�Ԑ����c��\n�@�@(         )", kWhiteColor, handle);
	DrawFormatStringToHandle(x, y, kYellowColor, handle, L"�@�@%2d\n�@�@ %2d / %2d",
		existTime, m_mgr.GetStage()->GetBestTime(m_stageName) / 60, existTime);
}

void StageBase::DrawKilledConditions(int x, int y, int handle, int killedNum)
{
	DrawStringToHandle(x, y, L"�@�@   ��ނ̓G�ɎE�����\n�@�@(         )", kWhiteColor, handle);
	DrawFormatStringToHandle(x, y, kYellowColor, handle, L"�@�@%2d\n�@�@ %2d / %2d", 
		killedNum, m_mgr.GetStage()->GetEnemyTypeCount(), killedNum);
}

void StageBase::DrawLeftArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	int handle;
	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
		{
			handle = m_arrow->GetHandle();
		}
		else
		{
			handle = m_arrowFlash->GetHandle();
		}
	}
	else
	{
		handle = m_arrowNo->GetHandle();
	}

	DrawRotaGraph(static_cast<int>(m_size.w * 0.5f - 150), static_cast<int>(m_size.h * 0.5f), 1.0, -kRad90, handle, true);
}

void StageBase::DrawRightArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	int handle;
	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
		{
			handle = m_arrow->GetHandle();
		}
		else
		{
			handle = m_arrowFlash->GetHandle();
		}
	}
	else
	{
		handle = m_arrowNo->GetHandle();
	}

	// MEMO:���̂�ReverX��true�ɂ���Ƃ��т�Ȃ����炵�Ă����Ă�
	DrawRotaGraph(static_cast<int>(m_size.w * 0.5f + 150), static_cast<int>(m_size.h * 0.5f), 1.0, kRad90, handle, true, true);
}

void StageBase::DrawUpArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	int handle;
	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
		{
			handle = m_arrow->GetHandle();
		}
		else
		{
			handle = m_arrowFlash->GetHandle();
		}
	}
	else
	{
		handle = m_arrowNo->GetHandle();
	}

	DrawRotaGraph(static_cast<int>(m_size.w * 0.5f), static_cast<int>(m_size.h * 0.5f - 150), 1.0, 0.0, handle, true);
}

void StageBase::DrawDownArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	int handle;
	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
		{
			handle = m_arrow->GetHandle();
		}
		else
		{
			handle = m_arrowFlash->GetHandle();
		}
	}
	else
	{
		handle = m_arrowNo->GetHandle();
	}

	DrawRotaGraph(static_cast<int>(m_size.w * 0.5f), static_cast<int>(m_size.h * 0.5f + 150), 1.0, 0.0, handle, true, false, true);
}

void StageBase::AddAchivedStr(const std::wstring& dir)
{
	m_achived.push_back({ dir + L"�̏����B���I", 0 });
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

void StageBase::DrawTime(int x, int y, int handle)
{
	DrawStringToHandle(x, y, L"> �^�C��", kWhiteColor, m_mgr.GetFont()->GetHandle(24));
	y += 24;
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatStringToHandle(x, y, kYellowColor, handle, L"%01d:%02d.%03d", min, sec, minSec);
}

void StageBase::DrawConditionsAchived()
{
	int y = 180;
	for (const auto& achived : m_achived)
	{
		if (achived.frame < static_cast<int>(kAchivedFrame * 0.5f))
		{
			DrawStringToHandle(540, y, achived.str.c_str(), kYellowColor, m_mgr.GetFont()->GetHandle(64));
		}
		else
		{
			float rate = (kAchivedFrame - achived.frame) / (kAchivedFrame * 0.5f);
			int alpha = 255 * rate;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawStringToHandle(540, y, achived.str.c_str(), kYellowColor, m_mgr.GetFont()->GetHandle(64));
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		y += 64;
	}
}

void StageBase::DrawWall()
{
	int centerX = static_cast<int>(m_size.w * 0.5f);
	int centerY = static_cast<int>(m_size.h * 0.5f);

	// �摜���S�����ɂ����`������邽�߂�
	// Rota�ɂ��Ă���
	DrawRotaGraph(centerX, centerY, 1.0, 0.0, m_field->GetHandle(), true);
}

void StageBase::DrawImage()
{

	DrawGraph(980, 595, m_startFrame->GetHandle(), true);
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
	DrawStringToHandle(1064, 600, L"�X�^�[�g", kWhiteColor, m_mgr.GetFont()->GetHandle(32));
}
