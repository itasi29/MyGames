#include <DxLib.h>
#include "Application.h"
#include "EnemyBase.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/ImageFile.h"

namespace
{
	// �ǂ���̖@���x�N�g��
	const Vec2 kNorVecLeft = Vec2{ 1.0f,  0.0f };
	const Vec2 kNorVecRight = Vec2{ -1.0f,  0.0f };
	const Vec2 kNorVecUp = Vec2{ 0.0f,  1.0f };
	const Vec2 kNorVecDown = Vec2{ 0.0f, -1.0f };

	// ���炷����
	const Vec2 kShiftSide = Vec2{ 0.0f, 0.2f };
	const Vec2 kShiftVert = Vec2{ 0.2f, 0.0f };

	// 1�G�t�F�N�g���t���[����
	constexpr int kWallEffectInterval = 3;
	// �ǂɓ��������G�t�F�N�g������t���[��
	constexpr int kWallHitFrame = 8 * kWallEffectInterval;
	// �摜�T�C�Y
	constexpr int kWallEffectGraphSize = 64;
	// �g�嗦
	constexpr double kExtRate = 0.75;
	// �s��
	constexpr int kRow = 8;
	// �񐔂̎�ސ�
	constexpr int kEffectTypeNum = 4;
	// �o���G�t�F�N�g�̗�̏ꏊ
	constexpr int kLine[kEffectTypeNum] = {
		0, 4, 7, 8
	};


	// ���炷��
	constexpr int kWallEffectSlide = 32;
}

EnemyBase::EnemyBase(const size& windowSize, float fieldSize) :
	m_size(windowSize),
	m_fieldSize(fieldSize),
	m_color(0),
	m_radius(0),
	m_isExsit(true),
	m_frame(0),
	m_lineType(0)
{
	m_updateFunc = &EnemyBase::StartUpdate;
	m_drawFunc = &EnemyBase::StartDraw;

	auto& file = GameManager::GetInstance().GetFile();
	m_wallEffect = file->LoadGraphic(L"Enemy/wallEffect.png");
	m_createSe = file->LoadSound(L"Se/create.mp3");
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Update()
{
	(this->*m_updateFunc)();
}

void EnemyBase::Draw()
{
	(this->*m_drawFunc)();
}

bool EnemyBase::Reflection(float scale, bool isShift)
{
	m_wallHitFrame--;

	float centerX = m_size.w * 0.5f;
	float centerY = m_size.h * 0.5f;

	// ��
	if (m_pos.x - m_radius * scale < centerX - m_fieldSize)
	{
		// �ʒu�̏C��
		m_pos.x = centerX - m_fieldSize + m_radius * scale;

		if (isShift)
		{
			// �G�t�F�N�g�t���[��������
			m_wallHitFrame = kWallHitFrame;

			// �G�t�F�N�g�`��ʒu
			m_drawWallHitX = static_cast<int>(m_pos.x - kWallEffectSlide - m_radius * scale);
			m_drawWallHitY = static_cast<int>(m_pos.y);

			ReflectionCal(kNorVecLeft);
			ShiftReflection(kShiftSide);
		}
		else
		{
			m_vec = { m_vec.y, -m_vec.x};
		}

		return true;
	}
	// �E
	if (m_pos.x + m_radius * scale > centerX + m_fieldSize)
	{
		m_pos.x = centerX + m_fieldSize - m_radius * scale;

		if (isShift)
		{
			m_wallHitFrame = kWallHitFrame;

			m_drawWallHitX = static_cast<int>(m_pos.x + kWallEffectSlide + m_radius * scale);
			m_drawWallHitY = static_cast<int>(m_pos.y);

			ReflectionCal(kNorVecRight);
			ShiftReflection(kShiftSide);
		}
		else
		{
			m_vec = { m_vec.y, -m_vec.x };
		}

		return true;
	}
	// ��
	if (m_pos.y - m_radius * scale < centerY - m_fieldSize)
	{
		m_pos.y = centerY - m_fieldSize + m_radius * scale;

		if (isShift)
		{
			m_wallHitFrame = kWallHitFrame;

			m_drawWallHitX = static_cast<int>(m_pos.x);
			m_drawWallHitY = static_cast<int>(m_pos.y - kWallEffectSlide - m_radius * scale);

			ReflectionCal(kNorVecUp);
			ShiftReflection(kShiftVert);
		}
		else
		{
			m_vec = { m_vec.y, -m_vec.x };
		}

		return true;
	}
	// ��
	if (m_pos.y + m_radius * scale > centerY + m_fieldSize)
	{
		m_pos.y = centerY + m_fieldSize - m_radius * scale;

		if (isShift)
		{
			m_wallHitFrame = kWallHitFrame;

			m_drawWallHitX = static_cast<int>(m_pos.x);
			m_drawWallHitY = static_cast<int>(m_pos.y + kWallEffectSlide + m_radius * scale);

			ReflectionCal(kNorVecDown);
			ShiftReflection(kShiftVert);
		}
		else
		{
			m_vec = { m_vec.y, -m_vec.x };
		}

		return true;
	}

	return false;
}

void EnemyBase::ReflectionCal(const Vec2& norVec)
{
	m_lineType = GetRand(kEffectTypeNum);

	// �@���x�N�g����2�{���猻�݂̃x�N�g��������
	m_vec = m_vec + norVec * norVec.Dot(-m_vec) * 2.0f;
}

void EnemyBase::ShiftReflection(const Vec2& shift)
{
	// FIXME:����͂���ł������ǁA�ł�����Q�l���ɂł���悤��

	Vec2 temp = m_vec;
	
	// �i��ł��������shift���i�܂���
	if (temp.x > 0)
	{
		temp.x += shift.x;
	}
	else
	{
		temp.x -= shift.x;
	}

	if (temp.y > 0)
	{
		temp.y += shift.y;
	}
	else
	{
		temp.y -= shift.y;
	}

	m_vec = temp.GetNormalized() * m_vec.Length();
}

void EnemyBase::ChangeNormalFunc()
{
	m_updateFunc = &EnemyBase::NormalUpdate;
	m_drawFunc = &EnemyBase::NormalDraw;
}

void EnemyBase::StartDraw()
{
	float rate = static_cast<float>(m_frame) / static_cast<float>(kApeearFrame);
	int alpha = static_cast<int>(255 * rate);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, 0.0,
		m_charImg->GetHandle(), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EnemyBase::NormalDraw()
{
	DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, 0.0,
		m_charImg->GetHandle(), true);

	DrawHitWallEffect();

#ifdef _DEBUG
	// �����蔻��̕`��
	m_col.Draw(0xff0000, false);
#endif
}

void EnemyBase::DrawHitWallEffect()
{
	// �ǂɓ��������G�t�F�N�g�̕`��
	if (m_wallHitFrame > 0)
	{
		int x = m_drawWallHitX - static_cast<int>(kWallEffectGraphSize * 0.5f - kWallEffectGraphSize * kExtRate * 0.5f);
		int y = m_drawWallHitY - static_cast<int>(kWallEffectGraphSize * 0.5f - kWallEffectGraphSize * kExtRate * 0.5f);

		int index = (kWallHitFrame - m_wallHitFrame) / kWallEffectInterval;
		int srcX = kWallEffectGraphSize * (index % kRow);
		int srcY = kWallEffectGraphSize * kLine[m_lineType];

		DrawRectRotaGraph(x, y, srcX, srcY, static_cast<int>(kWallEffectGraphSize * kExtRate), static_cast<int>(kWallEffectGraphSize * kExtRate), kExtRate, 0.0, m_wallEffect->GetHandle(), true);
	}
}
