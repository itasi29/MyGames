#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/SoundSystem.h"
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

	const std::string kStageName = "�T�[�N��";
	constexpr int kExsitTime = 2;

	// ��������
	constexpr int kCreateFrame = 300;

	// �����B�����̕`�掞��("���̏����B���̕���")
	constexpr int kAchivedFrame = 120;

	// �T�E���h�̃t�F�[�h�t���[��
	constexpr int kSoundFade = 30;

	// �v���C���[���S���̉�ʂ̗h��t���[��
	constexpr int kPlayerDeathShakeFrame = 10;

	// �v���C���[�����`��t���[��
	constexpr int kEmphasisFrame = 60 * 5;
	// �����J�n�t���[��
	constexpr int kAlphaEmphasisFrame = 60 * 4;
	// �F�ύX�Ԋu
	constexpr int kEmlhasisInterval = 20;

	// �h��p�x
	float kShakeAngle = DX_PI_F / 180 * 8;
}

StageTutorial::StageTutorial(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_index(0),
	m_explanation(Explanation::kOperation),
	m_createFrame(0),
	m_isStart(true),
	m_emphasisFrame(0)
{
	m_stageName = "���K";

	// �f�[�^�̐���
	m_mgr.GetStage()->CreateData(m_stageName);

	Init();
}

StageTutorial::~StageTutorial()
{
	// �I������ۂɃ`���[�g���A�������Ƃ������ɂ���
	m_mgr.GetStage()->SaveClear(m_stageName);
}


void StageTutorial::ChangeStage(Input& input)
{
	if (m_player->IsExsit()) return;

	// ���S����͕ς��Ȃ��悤�ɂ���
	if (m_waitFrame < kWaitChangeFrame) return;


	if (m_mgr.GetStage()->IsClearStage(kStageName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_1> nextStage = std::make_shared<Stage1_1>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
}
