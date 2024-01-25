#include <DxLib.h>
#include <cmath>
#include "Application.h"

#include "GameManager.h"
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
	constexpr float kRadius = 24.0f;

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
}

EnemyDash::EnemyDash(const size& windowSize, float fieldSize, std::shared_ptr<Player>& player) :
	EnemyBase(windowSize, fieldSize),
	m_player(player),
	m_logFrame(kDashLogNum),
	m_isDash(false),
	m_waitDashFrame(kWaitDashFrame),
	m_startWaitDashFrame(kStartWaitDashFrame)
{
	m_name = "Dash";
	m_color = kColor;
	m_posLog.resize(kDashLogNum);

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Dash.png");

	m_dashSe = mgr->LoadSound(L"Se/enemyDash.mp3");
}

EnemyDash::~EnemyDash()
{
}

void EnemyDash::Init(const Vec2& pos)
{
	// �����œn���ꂽ�ʒu�������ʒu��
	m_pos = pos;
	m_radius = kRadius;

	// �t���[���̏�����
	m_frame = 0;

	// �������������_���Ō��߂�
	do
	{
		float moveX = (GetRand(16) - 8) * 0.125f;
		float moveY = (GetRand(16) - 8) * 0.125f;

		double angle = atan2(moveX, -moveY);

		if (angle >= (DX_PI / 180 * 135)) continue;
		if (angle <= -(DX_PI / 180 * 135)) continue;


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

	// ���˂������A�_�b�V����ԂȂ�ʏ��Ԃɖ߂�
	if (Reflection() && m_isDash)
	{
		// �_�b�V���I��
		m_isDash = false;

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
			DrawRotaGraph(static_cast<int>(m_posLog[i].x), static_cast<int>(m_posLog[i].y), 1.0, 0.0,
				m_charImg->GetHandle(), true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, 0.0,
		m_charImg->GetHandle(), true);

	DrawHitWallEffect();

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
		for (int i = kDashLogNum - 1; i > 0; i--)
		{
			m_posLog[i] = m_posLog[i - 1];
		}
		m_posLog[0] = m_pos;
		m_logFrame = 0;

		return;
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
