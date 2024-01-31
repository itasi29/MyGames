#include <DxLib.h>
#include <cmath>
#include "Application.h"

#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/SoundSystem.h"

#include "EnemyDash.h"

#include "Player/Player.h"

namespace
{
	// �����X�s�[�h
	constexpr float kSpeed = 4.0f;
	// ���a
	constexpr float kRadius = 43.0f;

	// �J���[
	constexpr int kColor = 0x0808ff;

	// ���O��
	constexpr int kDashLogNum = 8;
	// �_�b�V�����̃X�s�[�h
	constexpr float kDashSpeed = 15.0f;
	// �_�b�V���J�n�܂ł̃X�s�[�h
	constexpr float kStartWaitSpeed = 2.5f;

	// �_�b�V���ҋ@����
	constexpr int kWaitDashFrame = 60 * 5;
	// �_�b�V�����J�n����܂ł̃t���[��
	constexpr int kStartWaitDashFrame = 30;

	// �_�b�V���G�t�F�N�g�̊Ԋu
	constexpr int kDasshEffInterval = 3;
	// �s��
	constexpr int kDashRow = 10;
	// �_�b�V���G�t�F�N�g�t���[��
	constexpr int kDashEffFrame = kDasshEffInterval * kDashRow;
	// �摜�T�C�Y
	constexpr int kDashGraphSize = 64;
	// �g�嗦
	constexpr double kDashExtRate = 2.0;
	// Y���W�̐؂蔲���ʒu
	constexpr int kDashSrcY = kDashGraphSize * 8;

	// �Ō�̉~�̐��̑傫���ƃX�s�[�h
	// MEMO:���̑傫���ƃX�s�[�h�͓����ق������h�����ǂ�
	constexpr int kRipple = 6;
	// �g��̍ő�傫��
	// MEMO:�\�z�����ǂ���ȏア���Ȃ��Ǝv���l
	constexpr int kMaxRippleSize = 1400;
}

EnemyDash::EnemyDash(const size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize),
	m_player(nullptr)
{
	m_name = "Dash";
	m_color = kColor;
	m_radius = kRadius;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Dash.png");
}

EnemyDash::EnemyDash(const size& windowSize, float fieldSize, std::shared_ptr<Player>& player) :
	EnemyBase(windowSize, fieldSize),
	m_player(player),
	m_logFrame(kDashLogNum),
	m_isDash(false),
	m_waitDashFrame(kWaitDashFrame),
	m_startWaitDashFrame(kStartWaitDashFrame),
	m_dashEffRipper(kMaxRippleSize),
	m_isDashEff(false)
{
	m_name = "Dash";
	m_color = kColor;
	m_radius = kRadius;
	m_posLog.resize(kDashLogNum);
	m_dashEffScreen = MakeScreen(m_size.w, m_size.h, true);

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Dash.png");

	m_dashSe = mgr->LoadSound(L"Se/enemyDash.mp3");
}

EnemyDash::~EnemyDash()
{
	DeleteGraph(m_dashEffScreen);
}

void EnemyDash::Init(const Vec2& pos, bool isStart)
{
	// �����œn���ꂽ�ʒu�������ʒu��
	m_pos = pos;
	m_radius = kRadius;

	// ��]������
	m_angle = 0.0;

	// �t���[���̏�����
	m_frame = 0;

	// �������������_���Ō��߂�
	do
	{
		float moveX = (GetRand(16) - 8) * 0.125f;
		float moveY = (GetRand(16) - 8) * 0.125f;

		if (isStart)
		{
			double angle = atan2(moveX, -moveY);

			if (angle >= (DX_PI / 180 * 135)) continue;
			if (angle <= -(DX_PI / 180 * 135)) continue;
		}


		m_vec = Vec2{ moveX, moveY };
	} while (false);

	// �[���x�N�g���łȂ��Ȃ琳�K��
	if (m_vec.SqLength() > 0)
	{
		m_vec.Normalize();
	}
	// �[���x�N�g���Ȃ������^���ɂ���
	else
	{
		m_vec = Vec2{ 1.0f, 0.0f };
	}

	// �X�s�[�h�𒲐�
	m_vec *= kSpeed;
}

void EnemyDash::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// �ς��Ƃ��ɓ����蔻��������
		m_col.SetCenter(m_pos, m_radius); 
		auto& sound = GameManager::GetInstance().GetSound();
		sound->PlaySe(m_createSe->GetHandle());

		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyDash::NormalUpdate()
{
	Dash();
	m_pos += m_vec;

	if (m_isDashEff)
	{
		m_dashEffRipper += kRipple;

		m_isDashEff = (m_dashEffRipper < kMaxRippleSize);
	}

	// ���˂������A�_�b�V����ԂȂ�ʏ��Ԃɖ߂�
	if (Reflection() && m_isDash)
	{
		// �_�b�V���I��
		m_isDash = false;

		m_dashEffPos = m_pos;
		m_dashEffRipper = kRipple;
		m_isDashEff = true;

		// �_�b�V����Ԃ���ʏ푬�x�ɖ߂�
		m_vec = m_vec.GetNormalized() * kSpeed;

		// �ҋ@���Ԃ�߂�
		m_waitDashFrame = kWaitDashFrame;
		// �X�^�[�g�ҋ@���Ԃ�߂�
		m_startWaitDashFrame = kStartWaitDashFrame;
	}

	m_col.SetCenter(m_pos, m_radius);
}

void EnemyDash::NormalDraw()
{
	if (m_logFrame < kDashLogNum)
	{
		for (int i = kDashLogNum - 1; i > -1; i--)
		{
			//auto alpha = (255 - (m_logFrame + i) * (255 / kDashLogNum));
			auto alpha = 255 / (i + m_logFrame + 1);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawRotaGraph(static_cast<int>(m_posLog[i].x), static_cast<int>(m_posLog[i].y), 1.0, m_angle,
				m_charImg->GetHandle(), true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, m_angle,
		m_charImg->GetHandle(), true);

	DrawDashEff();

	DrawHitWallEffect();

	DrawDashEff();

#ifdef _DEBUG
	// �����蔻��̕`��
	m_col.Draw(0xff0000, false);
#endif
}

void EnemyDash::Dash()
{
	m_logFrame++;
	// �_�b�V�����Ȃ烍�O�̂ݍX�V
	if (m_isDash)
	{
		m_angle -= kRad * 0.25;

		for (int i = kDashLogNum - 1; i > 0; i--)
		{
			m_posLog[i] = m_posLog[i - 1];
		}
		m_posLog[0] = m_pos;
		m_logFrame = 0;

		return;
	}
	else
	{
		m_angle -= kRad;
	}

	// �ҋ@���ԑ҂�����
	if (m_waitDashFrame > 0)
	{
		m_waitDashFrame--;

		// �ҋ@���Ԃ��I�������
		if (m_waitDashFrame <= 0)
		{
			// ���K��
			m_vec.Normalize();
			// ���x�̕ύX
			m_vec *= kStartWaitSpeed;
		}
		return;
	}
	
	// �X�^�[�g�܂ł̃t���[�������炷
	m_startWaitDashFrame--;

	// �X�^�[�g�ҋ@���I�������
	if (m_startWaitDashFrame <= 0)
	{
		auto& sound = GameManager::GetInstance().GetSound();
		sound->PlaySe(m_dashSe->GetHandle());
		m_isDash = true;

		// ���݂̈ʒu����v���C���[�܂ł̃x�N�g���̐���
		m_vec = m_player->GetPos() - m_pos;
		// ���K��
		m_vec.Normalize();
		// �X�s�[�h�ɕϊ�
		m_vec *= kDashSpeed;

		// ���O�̍X�V
		for (auto& log : m_posLog)
		{
			log = m_pos;
		}
		m_logFrame = 0;
	}
}

void EnemyDash::DrawDashEff() const
{
	if (!m_isDashEff) return;

	SetDrawScreen(m_dashEffScreen);
	SetDrawBlendMode(DX_BLENDMODE_MULA, 16);
	DrawBox(0, 0, m_size.w, m_size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
	DrawCircle(static_cast<int>(m_dashEffPos.x), static_cast<int>(m_dashEffPos.y), m_dashEffRipper, 0x0b60b0, false, kRipple);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	int handle = GameManager::GetInstance().GetScene()->GetScreenHandle();
	SetDrawScreen(handle);
	DrawGraph(0, 0, m_dashEffScreen, true);
}
