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
#include "Enemy/EnemyMoveWall.h"
#include "Enemy/EnemyNormal.h"
#include "Enemy/EnemyLarge.h"
#include "Enemy/EnemyDash.h"
#include "Enemy/EnemyCreate.h"
#include "Enemy/EnemyDivision.h"
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
	// ����ɋ���������̐F
	constexpr unsigned int kRedColor = 0xd2001a;
	// �o�b�N�t���[���̐F
	constexpr unsigned int kBackFrameColor = 0x161a30;

	// ���̓_�ŊԊu
	constexpr int kFlashInterval = 20;


	// �����̕`���ʒu
	constexpr int kConditionsPosX = 20;
	// �����B�����̕�����`��ʒu
	constexpr int kConditionsStrPosX = 12;

	// �E���ꂽ��ނ̊�`��ʒu
	constexpr int kKillTypePosX = 156;
	constexpr int kKillTypePosY = 200;
	// �f�t�H���g�g�嗦
	constexpr double kKillTypeDefExtRate = 0.372;
	// �傫�ߊg�嗦
	constexpr double kKillTypeLargeExtRate = 0.6;
	// ����ɑΉ����閼�O
	const std::vector<std::string> kLargeTypeName = {
		"Child",
		"Split",
		"SplitTwoBound"
	};
	// �����ߊg�嗦
	constexpr double kKillTypeSmallExtRate = 0.25;
	// ����ɑΉ����閼�O
	const std::vector<std::string> kSmallTypeName = {
		"Large",
		"BossArmored",
		"BossStrongArmored"
	};

	// �v���C���[���S���̉�ʂ̗h��t���[��
	constexpr int kShakeFrameDeath = 10;

	// �T�E���h�̃t�F�[�h�t���[��
	constexpr int kSoundFade = 30;

	// ���W�A���ł�90�x
	constexpr double kRad90 = DX_PI / 2;

	// �����B�����̕`�掞��("���̏����B���̕���")
	constexpr int kAchivedFrame = 120;

	// �X�^�[�g�����̃E�F�[�u�X�s�[�h
	constexpr float kWaveSpeed = DX_PI_F / 180 * 5;
	// �E�F�[�u�̊Ԋu
	constexpr float kWaveInterval = DX_PI_F / 15.0f;

	// �E�F�[�u������
	int kTitleWaveNum = 4;
	const wchar_t* const kTitleWave[] = {L"�X", L"�^", L"�[", L"�g"};
	int kDashWaveNum = 4;
	const wchar_t* const kDashWave[] = {L"�_", L"�b", L"�V", L"��"};

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
	m_waveAngle(DX_PI_F),
	m_isWaveDraw(true),
	m_isUpdateTime(false),
	m_isUpdateBestTime(false)
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
	m_arrowLock = file->LoadGraphic(L"UI/lock.png");
	m_arrowConditions = file->LoadGraphic(L"UI/arrowConditions.png");
	m_startFrame = file->LoadGraphic(L"UI/startFrame.png");
	m_bFrameImg = file->LoadGraphic(L"UI/backFrame.png");
	m_enemysImg["Normal"] = file->LoadGraphic(L"Enemy/Normal.png");
	m_enemysImg["MoveWall"] = file->LoadGraphic(L"Enemy/Wall.png");
	m_enemysImg["Large"] = file->LoadGraphic(L"Enemy/Large.png");
	m_enemysImg["Division"] = file->LoadGraphic(L"Enemy/Division.png");
	m_enemysImg["Dash"] = file->LoadGraphic(L"Enemy/Dash.png");
	m_enemysImg["Create"] = file->LoadGraphic(L"Enemy/Create.png");
	m_enemysImg["Child"] = file->LoadGraphic(L"Enemy/Child.png");
	m_enemysImg["Split"] = file->LoadGraphic(L"Enemy/Split.png");
	m_enemysImg["SplitTwoBound"] = file->LoadGraphic(L"Enemy/Split.png");
	m_enemysImg["BossArmored"] = file->LoadGraphic(L"Enemy/BossArmored.png");
	m_enemysImg["BossStrongArmored"] = file->LoadGraphic(L"Enemy/BossStrongArmored.png");
	m_check = file->LoadGraphic(L"UI/check.png");

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
	m_isWaveDraw = true;
	m_waveAngle -= kWaveSpeed;
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

	// �ҋ@�t���[���̑���
	m_waitFrame++;

	if (m_waitFrame < kWaitChangeFrame) return;


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

	if (input.IsTriggered("OK"))
	{
		// �ړ����ł����Ă������ړ�
		m_mgr.GetStage()->ImmediatelyChange();

		// �����o�֐��|�C���^�̍X�V
		m_updateFunc = &StageBase::UpdatePlaying;
		m_drawFunc = &StageBase::DrawPlaying;

		m_sound->Stop(m_selectBgm->GetHandle());
		m_isUpdateBestTime = false;
		m_soundFrame = 0;
		m_waveAngle = 0;

		// �e�평��������
		Init();
	}

	m_mgr.GetStage()->ChangeAbility(kDash);
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
		m_isUpdateTime = m_boss->OnAttack(playerIsDash, m_player->GetObjRect());

		// �{�X�̎��S����
		if (!m_boss->IsExsit())
		{
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

#ifdef _DEBUG
	// �f�o�b�O�p�Ƃ��č����Ŏ��Ԃ��i�ނ悤��
	if (CheckHitKey(KEY_INPUT_U))
	{
		for (int i = 0; i < 60; i++)
		{
			UpdateTime();
		}
	}
#endif
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
		m_waveAngle = 0.0;

		// �x�X�g�^�C���̍X�V
		if (m_mgr.GetStage()->UpdateBestTime(m_stageName, m_frame))
		{
			m_isUpdateBestTime = true;
		}

		// �N���A���Ă��邩�̊m�F
		CheckStageConditions();
	}
}

void StageBase::UpdateBossDeath(Input& input)
{
	m_sound->PlayFadeBgm(m_playBgm->GetHandle(), 0.6f);

	if (m_soundFrame > kSoundFade)
	{
		m_sound->PlayBgm(m_playBgm->GetHandle());
	}
	else
	{
		m_soundFrame++;
		m_sound->PlayFadeBgm(m_playBgm->GetHandle(), m_soundFrame / static_cast<float>(kSoundFade));
	}

	m_boss->Update();

	if (m_boss->IsEndPerformance())
	{
		m_mgr.GetStage()->OnClear();
	}
}

void StageBase::DrawSelect()
{
	DrawWall();

	SetDrawMode(DX_DRAWMODE_BILINEAR);
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}
	if (m_boss)
	{
		m_boss->Draw();
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);
	m_player->Draw();
	auto name = StringUtility::StringToWString(m_stageName);
	// �X�e�[�W���̕`��
	DrawBox(0, 28, 288, 104, kFrameColor, true);
	DrawFormatStringToHandle(64, 32, kYellowColor, m_mgr.GetFont()->GetHandle(64), L"%s", name.c_str());

	int fontHandle = m_mgr.GetFont()->GetHandle(32);
	int drawScreenHandle = m_mgr.GetScene()->GetScreenHandle();

	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	// ���Ԃ̕`��
	DrawTime(20, 160, fontHandle);
	// �E���ꂽ���Ƃ�����G�̕`��
	DrawStringToHandle(136, 160, L"> Circle", kWhiteColor, m_mgr.GetFont()->GetHandle(24));
	DrawKilledEnemyType();
	SetDrawScreen(drawScreenHandle);
	// �㉺���]���ĕ`��t���[��
	DrawRotaGraph(155, 168, 1.0, 0.0, m_bFrameImg->GetHandle(), true, false, true);
	DrawBox(0, 198, 310, 198 + 30, kBackFrameColor, true);
	DrawGraph(0, 0, m_strHandle, true);

	// �X�e�[�W�����̕`��
	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	auto y = DrawStageConditions(256);
	SetDrawScreen(drawScreenHandle);
	// �������ɂ���t���[���w�i��`�悷��
	if (y >= 0)
	{
		DrawBox(0, 244, 310, 244 + y, kBackFrameColor, true);
		DrawGraph(0, 244 + y, m_bFrameImg->GetHandle(), true);
	}
	DrawGraph(0, 0, m_strHandle, true);

	DrawBestTime();

	// ���̕`��
	DrawArrow();

	// �X�^�[�g�̉摜�̕`��
	DrawWave("OK", kTitleWave, kTitleWaveNum);

	// �����B�����̂�`��
	DrawConditionsAchived(y);
}

void StageBase::DrawPlaying()
{
	DrawWall();

	SetDrawMode(DX_DRAWMODE_BILINEAR);
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}
	if (m_boss)
	{
		m_boss->Draw();
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);
	m_player->Draw();

	int drawScreenHandle = m_mgr.GetScene()->GetScreenHandle();

	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	// ���Ԃ̕`��
	DrawTime(20, 144, m_mgr.GetFont()->GetHandle(64));
	SetDrawScreen(drawScreenHandle);
	DrawRotaGraph(155, 168, 1.0, 0.0, m_bFrameImg->GetHandle(), true, false, true);
	DrawBox(0, 198, 310, 198 + 36, kBackFrameColor, true);
	DrawGraph(0, 0, m_strHandle, true);

	// �����̕`��
	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	auto y = DrawStageConditions(244+16+20);
	SetDrawScreen(drawScreenHandle);
	// MEMO:�������ɂ���t���[���w�i��`�悷��
	if (y >= 0)
	{
		DrawBox(0, 268, 310, 268 + y, kBackFrameColor, true);
		DrawGraph(0, 268 + y, m_bFrameImg->GetHandle(), true);
	}
	DrawGraph(0, 0, m_strHandle, true);

	if (m_mgr.GetStage()->GetAbility() == kDash)
	{
		DrawWave("dash", kDashWave, kDashWaveNum);
	}

	DrawBestTime();
}

void StageBase::DrawBossDeath()
{
	DrawWall();

//	m_player->Draw();
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	m_boss->Draw();
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

void StageBase::CheckConditionsTime(const std::string& stageName, int exsitTime, const std::wstring& dir)
{
	auto& stage = m_mgr.GetStage();

	// ���ɃN���A���Ă�����m�F���Ȃ�
	if (stage->IsClearStage(stageName)) return;

	// �x�X�g�^�C�����������Ԃ𒴂��Ă��邩
	if (stage->GetBestTime(m_stageName) > exsitTime * 60)
	{
		stage->SaveClear(stageName);
		AddAchivedStr(dir);
	}
}

void StageBase::CheckConditionsKilled(const std::string& stageName, int killedNum, const std::wstring& dir)
{
	auto& stage = m_mgr.GetStage();

	// ���ɃN���A���Ă�����m�F���Ȃ�
	if (stage->IsClearStage(stageName)) return;

	if (stage->GetEnemyTypeCount() >= killedNum)
	{
		stage->SaveClear(stageName);
		AddAchivedStr(dir);
	}
}

void StageBase::CheckConditionsSumTime(const std::string& stageName, const std::vector<std::string>& names, int exsitTime, const std::wstring& dir)
{
	auto& stage = m_mgr.GetStage();
	int sumTime = 0;

	// ���ɃN���A���Ă�����m�F���Ȃ�
	if (stage->IsClearStage(stageName)) return;

	// �m�F����X�e�[�W�̂��ׂẴ^�C�������Z����
	for (const auto& name : names)
	{
		sumTime += stage->GetBestTime(name);
	}

	// ���Z�������Ԃ������Ă��邩
	if (sumTime >= exsitTime * 60)
	{
		stage->SaveClear(stageName);
		AddAchivedStr(dir);
	}
}

void StageBase::DrawArrowConditions(const std::string& nextStName, int y, double angle, bool isReverseX, bool isReverxeY)
{
	if (m_mgr.GetStage()->IsClearStage(nextStName) && (m_waitFrame / kFlashInterval) % 2 != 0)
	{
		DrawBox(kConditionsPosX, y, kConditionsPosX + 28, y + 28, 0xffde00, true);
	}
	DrawRotaGraph(kConditionsPosX + 14, y + 14, 1.0, angle, m_arrowConditions->GetHandle(), true, isReverseX, isReverxeY);
}

void StageBase::DrawTimeConditions(int y, int handle, int existTime)
{
	DrawStringToHandle(kConditionsPosX, y, L"�@�@   �b�Ԑ����c��\n�@�@(           )", kWhiteColor, handle);
	DrawFormatStringToHandle(kConditionsPosX, y, kYellowColor, handle, L"�@�@%2d\n�@�@  %2d / %2d",
		existTime, m_mgr.GetStage()->GetBestTime(m_stageName) / 60, existTime);
}

void StageBase::DrawKilledConditions(int y, int handle, int killedNum)
{
	DrawStringToHandle(kConditionsPosX, y, L"�@�@   ��ނ̓G�ɎE�����\n�@�@(          )", kWhiteColor, handle);
	DrawFormatStringToHandle(kConditionsPosX, y, kYellowColor, handle, L"�@�@%2d\n�@�@  %2d / %2d",
		killedNum, m_mgr.GetStage()->GetEnemyTypeCount(), killedNum);
}

void StageBase::DrawSumTimeConditions(const std::vector<std::string>& names, int y, int handle, int existTime)
{
	auto& stage = m_mgr.GetStage();
	int sumTime = 0;

	// �m�F����X�e�[�W�̂��ׂẴ^�C�������Z����
	for (const auto& name : names)
	{
		sumTime += stage->GetBestTime(name);
	}

	// �b�ɖ߂�
	sumTime /= 60;

	DrawStringToHandle(kConditionsPosX, y, L"�@�@�S�X�e�[�W���v\n�@�@    �b�Ԑ����c��\n�@�@(           )", kWhiteColor, handle);
	DrawFormatStringToHandle(kConditionsPosX, y, kYellowColor, handle, L"\n�@�@%02d\n�@�@  %2d / %2d",
		existTime, sumTime, existTime);
}

void StageBase::DrawLeftArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage, bool isClear) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	int x = static_cast<int>(m_size.w * 0.5f - 150);
	int y = static_cast<int>(m_size.h * 0.5f);
	DrawRotaGraph(x, y, 1.0, -kRad90, handle, true);
	DrawArrowLock(x, y, isBossStage, isClear);
}

void StageBase::DrawRightArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage, bool isClear) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	int x = static_cast<int>(m_size.w * 0.5f + 150);
	int y = static_cast<int>(m_size.h * 0.5f);
	// MEMO:���̂�ReverX��true�ɂ���Ƃ��т�Ȃ����炵�Ă����Ă�
	DrawRotaGraph(x, y, 1.0, kRad90, handle, true, true);
	DrawArrowLock(x, y, isBossStage, isClear);
}

void StageBase::DrawUpArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage, bool isClear) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	int x = static_cast<int>(m_size.w * 0.5f);
	int y = static_cast<int>(m_size.h * 0.5f - 150);
	DrawRotaGraph(x, y, 1.0, 0.0, handle, true);
	DrawArrowLock(x, y, isBossStage, isClear);
}

void StageBase::DrawDownArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage, bool isClear) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	int x = static_cast<int>(m_size.w * 0.5f);
	int y = static_cast<int>(m_size.h * 0.5f + 150);
	DrawRotaGraph(x, y, 1.0, 0.0, handle, true, false, true);
	DrawArrowLock(x, y, isBossStage, isClear);
}

void StageBase::DrawKilledEnemy(const std::string& enemyName, int addX, unsigned int color, int radius) const
{
	double enemyExtRate = kKillTypeDefExtRate;
	for (const auto& name : kSmallTypeName)
	{
		if (name == enemyName)
		{
			enemyExtRate = kKillTypeSmallExtRate;
			break;
		}
	}
	for (const auto& name : kLargeTypeName)
	{
		if (name == enemyName)
		{
			enemyExtRate = kKillTypeLargeExtRate;
			break;
		}
	}

	auto& file = m_enemysImg.at(enemyName);
	if (m_mgr.GetStage()->IsKilledEnemy(enemyName))
	{
		DrawRotaGraph(kKillTypePosX + addX, kKillTypePosY, enemyExtRate, 0.0, file->GetHandle(), true);
		DrawRotaGraph(kKillTypePosX + addX, kKillTypePosY, 0.5, 0.0, m_check->GetHandle(), true);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
		DrawRotaGraph(kKillTypePosX + addX, kKillTypePosY, enemyExtRate, 0.0, file->GetHandle(), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

//		DrawCircle(kKillTypePosX + addX, kKillTypePosY, radius, color, false);
	}
}

void StageBase::AddAchivedStr(const std::wstring& dir)
{
	m_achived.push_back({ dir + L"�̏����B���I", 0 });
}

void StageBase::CreateMoveWall()
{
	// �㑤
	auto enemy = std::make_shared<EnemyMoveWall>(m_size, m_fieldSize);
	enemy->Init({ 0, -1 });
	m_enemy.push_back(enemy);
	enemy = std::make_shared<EnemyMoveWall>(m_size, m_fieldSize);
	enemy->Init({ 0, 1 });
	m_enemy.push_back(enemy);
}

void StageBase::CreateNormal(int& frame, bool isStart)
{
	frame = 0;
	auto enemy = std::make_shared<EnemyNormal>(m_size, m_fieldSize);
	enemy->Init(m_centerPos, isStart);
	m_enemy.push_back(enemy);
}

void StageBase::CreateLarge(int& frame, bool isStart)
{
	frame = 0;
	auto enemy = std::make_shared<EnemyLarge>(m_size, m_fieldSize);
	enemy->Init(m_centerPos, isStart);
	m_enemy.push_back(enemy);
}

void StageBase::CreateDash(int& frame, bool isStart)
{
	frame = 0;
	auto enemy = std::make_shared<EnemyDash>(m_size, m_fieldSize, m_player);
	enemy->Init(m_centerPos, isStart);
	m_enemy.push_back(enemy);
}

void StageBase::CreateEneCreate(int& frame, bool isStart)
{
	frame = 0;
	auto enemy = std::make_shared<EnemyCreate>(m_size, m_fieldSize, this);
	enemy->Init(m_centerPos, isStart);
	m_enemy.push_back(enemy);
}

void StageBase::CreateDivision(int& frame, bool isStart)
{
	frame = 0;
	auto enemy = std::make_shared<EnemyDivision>(m_size, m_fieldSize, this);
	enemy->Init(m_centerPos, isStart);
	m_enemy.push_back(enemy);
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

//	m_mgr.GetStage()->m_clear = true;
	// �x�X�g�^�C���̍X�V
	if (m_mgr.GetStage()->UpdateBestTime(m_stageName, m_frame))
	{
		m_isUpdateBestTime = true;
	}

	// �N���A���Ă��邩�̊m�F
	CheckStageConditions();

	// �|�������̒ǉ�
	m_mgr.GetStage()->UpdateClearBoss(m_boss->GetName());

	// �G�S�ď���
	m_enemy.clear();

	m_updateFunc = &StageBase::UpdateBossDeath;
	m_drawFunc = &StageBase::DrawBossDeath;
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

void StageBase::DrawBestTime()
{
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

	if (!m_isUpdateBestTime) return;


	if (((m_waitFrame / 30) % 2) == 1)
	{
		DrawFormatStringToHandle(m_size.w - 256, 112 + 48, kRedColor, m_mgr.GetFont()->GetHandle(64), L"%02d:%02d.%03d", min, sec, minSec);
	}
	
	if (m_waitFrame > kAchivedFrame) return;

	if (m_waitFrame > static_cast<int>(kAchivedFrame * 0.5f))
	{
		float rate = 1.0f - (m_waitFrame - (kAchivedFrame * 0.5f)) / (kAchivedFrame * 0.5f);
		int alpha = static_cast<int>(255 * rate);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(m_size.w - 344, 248, m_size.w, 312, kBackFrameColor, true);
		DrawStringToHandle(m_size.w - 336, 256, L"�x�X�g�^�C���X�V�I", kYellowColor, m_mgr.GetFont()->GetHandle(48));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		return;
	}

	DrawBox(m_size.w - 344, 248, m_size.w, 312, kBackFrameColor, true);
	DrawStringToHandle(m_size.w - 336, 256, L"�x�X�g�^�C���X�V�I", kYellowColor, m_mgr.GetFont()->GetHandle(48));
}

void StageBase::DrawConditionsAchived(int y)
{
	y += 244 + 100;
	int backFrameY = y - 4;

	for (const auto& achived : m_achived)
	{
		if (achived.frame < static_cast<int>(kAchivedFrame * 0.5f))
		{
			DrawBox(0, backFrameY, 352, backFrameY + 68, kBackFrameColor, true);
			DrawStringToHandle(kConditionsStrPosX, y, achived.str.c_str(), kYellowColor, m_mgr.GetFont()->GetHandle(64));
		}
		else
		{
			float rate = (kAchivedFrame - achived.frame) / (kAchivedFrame * 0.5f);
			int alpha = static_cast<int>(255 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawBox(0, backFrameY, 352, backFrameY + 68, kBackFrameColor, true);
			DrawStringToHandle(kConditionsStrPosX, y, achived.str.c_str(), kYellowColor, m_mgr.GetFont()->GetHandle(64));
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		y += 64;
		backFrameY += 68;
	}
}

void StageBase::DrawArrowLock(int x, int y, bool isBossStage, bool isClear) const
{
	if (!isBossStage || isClear) return;

	DrawRotaGraph(x, y, 1.0, 0.0, m_arrowLock->GetHandle(), true);
}

void StageBase::DrawWall()
{
	int centerX = static_cast<int>(m_size.w * 0.5f);
	int centerY = static_cast<int>(m_size.h * 0.5f);

	// �摜���S�����ɂ����`������邽�߂�
	// Rota�ɂ��Ă���
	DrawRotaGraph(centerX, centerY, 1.0, 0.0, m_field->GetHandle(), true);
}

void StageBase::DrawWave(const char* const cmd, const wchar_t* const str[], int num)
{
	if (!m_isWaveDraw) return;
	m_isWaveDraw = false;

	DrawGraph(980, 595, m_startFrame->GetHandle(), true);

	switch (m_input.GetType())
	{
	case InputType::keybd:
		m_key->DrawKey(m_input.GetHardDataName(cmd, InputType::keybd), 1016, 600, 2.0);
		break;
	default:
		assert(false);
	case InputType::pad:
		m_bt->DrawBottan(m_input.GetHardDataName(cmd, InputType::pad), 1016, 600, 2.0);
		break;
	}

	int handle = m_mgr.GetFont()->GetHandle(32);

	int x = 1064;

	for (int i = 0; i < num; i++)
	{
		int add = static_cast<int>(sinf(m_waveAngle + kWaveInterval * i) * -10);

		if (add > 0)
		{
			add = 0;
		}

		int y = 600 + add;


		DrawStringToHandle(x, y, str[i], kWhiteColor, handle);
		x += 24;
	}
}

int StageBase::GetArrowHandle(bool isAlreadyClear, const std::string& nextStName) const
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

	return handle;
}
