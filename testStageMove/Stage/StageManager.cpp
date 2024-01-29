#include <DxLib.h>
#include "StageBase.h"
#include "Stage1.h"
#include "StageManager.h"

namespace
{
	// �ړ��X�s�[�h
	constexpr float kSpeed = 100.0f;

	// �������ɂȂ鋗��
	constexpr float kSlowDis = 200.0f;

	// �������x
	constexpr float kLimitSlowSpeed = 10.0f;

	// �X�e�[�W�̏c����
	constexpr int kLine = 3;
	constexpr int kRow = 3;

	// �X�e�[�W�Ԃ̊Ԋu
	constexpr int kStageMargin = 80;

	// ���O�̏ꏊ
	const std::string kStName[kLine][kRow] =
	{
		{"1", "2", "3"},
		{"4", "5", "6"},
		{"7", "8", "9"}
	};

	// ���ɐL�т�傫��
	constexpr int kWidth = 200;
	// �c�ɐL�т�傫��
	constexpr int kHeight = 100;
	// �������t���[��
	constexpr int kDeathMoveFrame = 60;
}

StageManager::StageManager()
{
	m_drawScreen = MakeScreen(1280, 720, true);
	m_screen = MakeScreen(1280 * kRow + kStageMargin * kRow, 720 * kLine + kStageMargin * kLine, true);
	m_keepScreen = MakeScreen(1280 * kRow + kStageMargin * kRow, 720 * kLine + kStageMargin * kLine, true);

	m_updateFunc = &StageManager::NormalUpdate;
	m_drawFunc = &StageManager::NormalDraw;

	m_stage = std::make_shared<Stage1>(*this);
	m_nextStage = nullptr;

	m_pos = {};
	m_targetPos = {};
	m_vec = {};

	m_motoPos = {};
}

StageManager::~StageManager()
{
}

void StageManager::Update()
{
	(this->*m_updateFunc)();
}

void StageManager::Draw() const
{
	(this->*m_drawFunc)();

	/*�f�o�b�O�p�̕`��@�{�̂ɂ͎����čs���Ȃ�*/ 
	// ��g�`��
	DrawBox(0, 0, 128 * kRow + 8 + kRow, 72 * kLine + 8 * kLine, 0xffffff, false);

	// ���̈ʒu
	DrawBox(m_motoPos.x / 10, m_motoPos.y / 10, m_motoPos.x / 10+ 128, m_motoPos.y / 10 + 72, 0x00ff00, false);
	// �������ʒu
	DrawBox(m_targetPos.x / 10, m_targetPos.y / 10, m_targetPos.x / 10 + 128, m_targetPos.y / 10 + 72, 0xff0000, false);

	// ���݈ʒu
	DrawBox(m_pos.x / 10, m_pos.y / 10, m_pos.x / 10 + 128, m_pos.y / 10 + 72, 0xffff00, false);

	clsDx();
	printfDx("pos : %.2f, %.2f\n", m_pos.x, m_pos.y);
	printfDx("vec : %.2f, %.2f\n", m_deathVec.x, m_deathVec.y);
}

void StageManager::ChangeScene(const std::shared_ptr<StageBase> nextStage)
{
	Vec2 pos;

	if (m_vec.Length() < kSlowDis)
	{
		// �`��p�X�N���[���Ɍ��݂���X�e�[�W��`��
		SetDrawScreen(m_drawScreen);
		ClearDrawScreen();
		m_stage->Draw();

		// �L�[�v�p�̃X�N���[���ɏꏊ���w�肵�Ă������̂�`��
		SetDrawScreen(m_keepScreen);
		ClearDrawScreen();
		pos = GetPos(m_stage->GetName());
		m_motoPos = pos;

		DrawGraph(pos.x, pos.y, m_drawScreen, true);
	}

	// �`��p�X�N���[���Ɏ��̃X�e�[�W�̕`��
	SetDrawScreen(m_drawScreen);
	ClearDrawScreen();
	nextStage->Draw();

	// �ړ����`��X�N���[���ɏꏊ���w�肵�Ă������̂�`��
	SetDrawScreen(m_screen);
	ClearDrawScreen();
	pos = GetPos(nextStage->GetName());

	DrawGraph(pos.x, pos.y, m_drawScreen, true);

	// �L�[�v�ɂ�����̂�`��
	DrawGraph(0, 0, m_keepScreen, true);
		
	// �ړ��x�N�g���̌v�Z
	m_vec = (pos - m_pos).GetNormalized() * kSpeed;

	// ��������̏ꏊ�̕ۑ�
	m_targetPos = pos;

	// �����o�֐��|�C���^�̍X�V
	m_updateFunc = &StageManager::StageMoveUpdate;
	m_drawFunc = &StageManager::StageMoveDraw;

	// ���̃X�e�[�W�̕ۑ�
	m_nextStage = nextStage;

	// ���݂̕`���ւƖ߂�(�{����)
	SetDrawScreen(DX_SCREEN_BACK);
}

void StageManager::DeathMoveScreen(const Vec2& vec)
{
	m_deathVec = vec;
	// ���K��
	m_deathVec.Normalize();

	// �ǂ��炪�傫����
	m_isBaseX = (abs(m_deathVec.x) > abs(m_deathVec.y));

	m_deathMoveFrame = 0;
	m_deathMoveAngle = 0.0;

	// �����o�֐��|�C���^�̍X�V
	m_updateFunc = &StageManager::DeathMoveUpdate;
	m_drawFunc = &StageManager::DeathMoveDraw;
}

void StageManager::NormalUpdate()
{
	m_stage->Update();
}

void StageManager::StageMoveUpdate()
{
	//m_nextStage->Update();

	// �ꏊ�̍X�V
	m_pos += m_vec;

	// �����̌v�Z
	Vec2 vel = (m_targetPos - m_pos);
	float dis = vel.Length();

	// ���������鋗���ɂȂ�����
	if (dis < kSlowDis)
	{
		// ���x��0�ȉ��ɂȂ�����ړ��I��
		if (dis < kLimitSlowSpeed)
		{
			m_updateFunc = &StageManager::NormalUpdate;
			m_drawFunc = &StageManager::NormalDraw;

			m_pos = m_targetPos;

			m_stage = m_nextStage;

			return;
		}

		// �X�s�[�h�̌v�Z
		float speed = (dis / kSlowDis) * kSpeed;
		// �X�s�[�h�̒���
		m_vec = vel.GetNormalized() * speed;
	}
}

void StageManager::DeathMoveUpdate()
{
	m_deathMoveFrame++;
	m_deathMoveAngle += (DX_PI / 15) * 0.5;

	//if (m_deathMoveFrame >= kDeathMoveFrame)
	if (m_deathMoveAngle >= DX_PI)
	{
		m_updateFunc = &StageManager::NormalUpdate;
		m_drawFunc = &StageManager::NormalDraw;
	}
}

void StageManager::NormalDraw() const
{
	m_stage->Draw();
}

void StageManager::StageMoveDraw() const
{
	// MEMO:�Ȃ����}�C�i�X�ɂ���ƕ`�悪���ʂɂȂ���
	DrawGraph(-m_pos.x, -m_pos.y, m_screen, true);
}

void StageManager::DeathMoveDraw() const
{
	int x, y;

	int frame = (static_cast<int>(m_deathMoveFrame % kDeathMoveFrame) - (kDeathMoveFrame * 0.5f));
	float rate = (1.0f - abs(frame) / static_cast<float>(kDeathMoveFrame * 0.5f));

#if false
	if (m_isBaseX)
	{
		x = m_deathVec.x * rate * kWidth;
		y = m_deathVec.y * rate * kHeight;
	}
	else
	{
		y = m_deathVec.x * rate * kWidth;
		x = m_deathVec.y * rate * kHeight;
	}
#else 
	rate = sinf(m_deathMoveAngle);
	if (m_isBaseX)
	{
		x = m_deathVec.x * rate * kWidth;
		y = m_deathVec.y * rate * kHeight;
	}
	else
	{
		y = m_deathVec.x * rate * kWidth;
		x = m_deathVec.y * rate * kHeight;
	}
#endif

	DrawGraph(x, y, m_drawScreen, true);
}

Vec2 StageManager::GetPos(const std::string& stage)
{
	Vec2 pos;

	for (int x = 0; x < kRow; x++)
	{
		for (int y = 0; y < kLine; y++)
		{
			// �X�e�[�W������v������
			if (kStName[y][x] == stage)
			{
				// �ꏊ�̕ۑ�
				pos.x = 1280 * x + kStageMargin * x;
				pos.y = 720 * y + kStageMargin * y;

				// �T���̏I��
				break;
			}
		}
	}

	return pos;
}
