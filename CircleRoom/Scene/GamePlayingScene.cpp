#include <DxLib.h>
#include <cassert>
#include "Input.h"
#include "Application.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "Stage/StageManager.h"
#include "FileSystem/SoundSystem.h"

#include "GamePlayingScene.h"
#include "GameClearScene.h"
#include "OptionScene.h"

#include "FileSystem/FileManager.h"
#include "FileSystem/ImageFile.h"

#include "Stage/Stage1_1.h"
#include "Stage/Stage1_2.h"
#include "Stage/Stage1_3.h"
#include "Stage/Stage1_4.h"
#include "Stage/Stage1_5.h"
#include "Stage/Stage1_6.h"
#include "Stage/Stage1_7.h"
#include "Stage/Stage1_8.h"
#include "Stage/Stage1_9.h"
#include "Stage/StageTutorial.h"


namespace
{
	enum PathType
	{
		kGraph,
		kSound
	};

	struct PathData
	{
		std::wstring path;
		PathType type;
	};

	// �t�F�[�h�̃t���[������
	constexpr int kFadeFrame = 60;

	// ���O�ɓǂݍ���ł����f�[�^��path
	const std::vector<PathData> kPath =
	{
		//{L"Enemy/wallEffect.png", kGraph},
		{L"Player/blood.png", kGraph},
		{L"UI/backFrame.png", kGraph},
		{L"Bgm/provisionalBgm.mp3", kSound},
		{L"Bgm/fieldFight.mp3", kSound},
		{L"Bgm/boss.mp3", kSound}
	};

}

GamePlayingScene::GamePlayingScene(GameManager& mgr, Input& input) :
	Scene(mgr),
	m_screenHandle(0),
	m_frame(kFadeFrame)
{
	// �����o�֐��|�C���^�̐ݒ�
	m_updateFunc = &GamePlayingScene::UpdateFadeIn;
	m_drawFunc = &GamePlayingScene::DrawFade;

	// �X�e�[�W�̐ݒ�
	// �`���[�g���A���X�e�[�W��ʏ�X�e�[�W��
	if (m_mgr.GetStage()->IsClearStage("Tutorial"))
	{
		StartStage(input);
	}
	// ���Ă��Ȃ���΃`���[�g���A���X�e�[�W��
	else
	{
		m_mgr.GetStage()->ChangeStage(std::make_shared<StageTutorial>(m_mgr, input));
	}

	m_mgr.GetStage()->ResetClear();

	m_bgm = m_mgr.GetFile()->LoadSound(L"Bgm/provisionalBgm.mp3");

	// ���O�ɃX�e�[�W���ő����g�����̂͂����œǂݍ���ł���
	auto size = kPath.size();
	m_stgData.resize(size);
	for (int i = 0; i < size; i++)
	{
		if (kPath[i].type == kGraph)
		{
			m_stgData[i] = m_mgr.GetFile()->LoadGraphic(kPath[i].path);
		}
		else if (kPath[i].type == kSound)
		{
			m_stgData[i] = m_mgr.GetFile()->LoadSound(kPath[i].path);
		}
	}
}

GamePlayingScene::~GamePlayingScene()
{
}

void GamePlayingScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void GamePlayingScene::Draw() const
{
	(this->*m_drawFunc)();
}

void GamePlayingScene::UpdateFadeIn(Input& input)
{
	m_sound->PlayFadeBgm(m_bgm->GetHandle(), 1.0f - m_frame / static_cast<float>(kFadeFrame));
	m_frame--;
	if (m_frame <= 0)
	{
		m_updateFunc = &GamePlayingScene::UpdateNormal;
		m_drawFunc = &GamePlayingScene::DrawNormal;
		m_frame = 0;
	}
}

void GamePlayingScene::UpdateFadeOut(Input& input)
{
	m_mgr.GetStage()->Update(input);
	m_frame++;
	if (m_frame > kFadeFrame)
	{
		m_sound->Stop();
		m_mgr.GetScene()->OnNormal();

		m_mgr.GetScene()->ChangeScene(std::make_shared<GameClearScene>(m_mgr));
	}
}

void GamePlayingScene::UpdateNormal(Input& input)
{
	// pause�{�^���������ꂽ��|�[�Y��ʂ��J��
	if (input.IsTriggered("pause"))
	{
		m_mgr.GetScene()->PushScene(std::make_shared<OptionScene>(m_mgr, input));
		m_mgr.GetStage()->OffWaveDraw();
		return;
	}

	m_mgr.GetStage()->Update(input);
	// �v���C���Ԃ̃A�b�v�f�[�g
	m_mgr.UpdatePlaytime();

	// �N���A������
	if (m_mgr.GetStage()->IsClear())
	{
		m_updateFunc = &GamePlayingScene::UpdateFadeOut;
		m_drawFunc = &GamePlayingScene::DrawFade;
		m_frame = 0;
	}
#if _DEBUG
	// ���U�I���i�N���A��ʂւ̈ڍs�j
	if (CheckHitKey(KEY_INPUT_RSHIFT))
	{
		m_updateFunc = &GamePlayingScene::UpdateFadeOut;
		m_drawFunc = &GamePlayingScene::DrawFade;
		m_frame = 0;
	}
#endif
}

void GamePlayingScene::DrawFade() const
{
	m_mgr.GetStage()->Draw();

	const auto& m_size = Application::GetInstance().GetWindowSize();
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, m_size.w, m_size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GamePlayingScene::DrawNormal() const
{
	m_mgr.GetStage()->Draw();
}

void GamePlayingScene::StartStage(Input& input)
{
	const auto& nowStage = m_mgr.GetNowStage();
	
	if (nowStage == "Stage1-2")
	{
		m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_2>(m_mgr, input));
		return;
	}
	if (nowStage == "Stage1-3")
	{
		m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_3>(m_mgr, input));
		return;
	}
	if (nowStage == "Stage1-4")
	{
		m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_4>(m_mgr, input));
		return;
	}
	if (nowStage == "Stage1-5")
	{
		m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_5>(m_mgr, input));
		return;
	}
	if (nowStage == "Stage1-6")
	{
		m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_6>(m_mgr, input));
		return;
	}
	if (nowStage == "Stage1-7")
	{
		m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_7>(m_mgr, input));
		return;
	}
	if (nowStage == "StageBoss")
	{
		m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_8>(m_mgr, input));
		return;
	}
	if (nowStage == "Master")
	{
		m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_9>(m_mgr, input));
		return;
	}

	// �ǂ��ɂ�����Ȃ�������1-1�ɓ���悤�ɂ���
	m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_1>(m_mgr, input));
	return;
}

