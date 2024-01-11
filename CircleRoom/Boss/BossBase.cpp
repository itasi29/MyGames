#include <DxLib.h>
#include "Application.h"
#include "GameManager.h"
#include "FileSystem/ImageFile.h"

#include "BossBase.h"

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

	// HP�o�[�̕�
	constexpr int kHpBarWidth = 180;
	// HP�o�[�̍���
	constexpr int kHpBarHeight = 50;

	// �w�iHP�o�[�̑O�㕝
	constexpr int kBackHpBarWidth = 10;
	// �w�iHP�o�[�̑O�㍂��
	constexpr int kBackHpBarHeight = 10;

	// �ǂɓ��������G�t�F�N�g������t���[��
	constexpr int kWallHitFrame = 10;

	// ���炷��
	constexpr int kWallEffectSlide = 32;
}

BossBase::BossBase(const size& windowSize, float fieldSize, int maxHp) :
	m_size(windowSize),
	m_fieldSize(fieldSize),
	m_maxHp(maxHp),
	m_isExsit(true),
	m_hp(maxHp)
{
	m_updateFunc = &BossBase::StartUpdate;
	m_drawFunc = &BossBase::StartDraw;

	m_wallEffect = GameManager::GetInstance().GetFile().LoadGraphic(L"Data/Image/Enemy/wallEffect.png");
	m_damageEffect = GameManager::GetInstance().GetFile().LoadGraphic(L"Data/Image/Enemy/damageEffect.png");
}

BossBase::~BossBase()
{
}

void BossBase::Update()
{
	(this->*m_updateFunc)();
}

void BossBase::Draw() const
{
	(this->*m_drawFunc)();
}

bool BossBase::OnAttack(bool isDash, const Collision& rect)
{
	if (isDash) return false;

	m_hp--;

	// HP���[���ɂȂ����玀�S�Ƃ���
	if (m_hp <= 0)
	{
		// �o�[�̕`�����HP��0�ɂ��Ă���
		m_hp = 0;
		m_isExsit = false;
	}

	return true;
}

bool BossBase::Reflection()
{
	m_wallHitFrame--;

	float centerX = m_size.w * 0.5f;
	float centerY = m_size.h * 0.5f;

	// ��
	if (m_pos.x - m_radius < centerX - m_fieldSize)
	{
		m_pos.x = centerX - m_fieldSize + m_radius;
		ReflectionCal(kNorVecLeft);
		ShiftReflection(kShiftSide);

		m_wallHitFrame = kWallHitFrame;

		m_drawWallHitX = static_cast<int>(m_pos.x - kWallEffectSlide - m_radius);
		m_drawWallHitY = static_cast<int>(m_pos.y);

		return true;
	}
	// �E
	if (m_pos.x + m_radius > centerX + m_fieldSize)
	{
		m_pos.x = centerX + m_fieldSize - m_radius;
		ReflectionCal(kNorVecRight);
		ShiftReflection(kShiftSide);

		m_wallHitFrame = kWallHitFrame;

		m_drawWallHitX = static_cast<int>(m_pos.x + kWallEffectSlide + m_radius);
		m_drawWallHitY = static_cast<int>(m_pos.y);

		return true;
	}
	// ��
	if (m_pos.y - m_radius < centerY - m_fieldSize)
	{
		m_pos.y = centerY - m_fieldSize + m_radius;
		ReflectionCal(kNorVecUp);
		ShiftReflection(kShiftVert);

		m_wallHitFrame = kWallHitFrame;

		m_drawWallHitX = static_cast<int>(m_pos.x);
		m_drawWallHitY = static_cast<int>(m_pos.y - kWallEffectSlide - m_radius);

		return true;
	}
	// ��
	if (m_pos.y + m_radius > centerY + m_fieldSize)
	{
		m_pos.y = centerY + m_fieldSize - m_radius;
		ReflectionCal(kNorVecDown);
		ShiftReflection(kShiftVert);

		m_wallHitFrame = kWallHitFrame;

		m_drawWallHitX = static_cast<int>(m_pos.x);
		m_drawWallHitY = static_cast<int>(m_pos.y + kWallEffectSlide + m_radius);

		return true;
	}

	return false;
}

void BossBase::ReflectionCal(const Vec2& norVec)
{
	// �@���x�N�g����2�{���猻�݂̃x�N�g��������
	m_vec = m_vec + norVec * norVec.Dot(-m_vec) * 2.0f;
}

void BossBase::ShiftReflection(const Vec2& shift)
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

void BossBase::ChangeNormalFunc()
{
	m_updateFunc = &BossBase::NormalUpdate;
	m_drawFunc = &BossBase::NormalDraw;
}

void BossBase::StartDraw() const
{
	float rate = static_cast<float>(m_frame) / static_cast<float>(kApeearFrame);
	int alpha = static_cast<int>(255 * rate);
	SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), m_color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void BossBase::NormalDraw() const
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), m_color, true);

	// �ǂɓ��������G�t�F�N�g�̕`��
	if (m_wallHitFrame > 0)
	{
		// MEMO:���݂͉�
		// ���W�𒆐S�Ƃ���
		DrawGraph(m_drawWallHitX - 16, m_drawWallHitY - 16, m_wallEffect->GetHandle(), true);
	}

#ifdef _DEBUG
	// �����蔻��̕`��
	m_col.Draw(0xff0000, false);
#endif
}

void BossBase::DrawHpBar() const
{
	Vec2 base = {m_size.w - kHpBarWidth * 1.5f, 160.0f};

	// �w�i
	DrawBox(static_cast<int>(base.x - kBackHpBarWidth), static_cast<int>(base.y - kBackHpBarHeight),
		static_cast<int>(base.x + kHpBarWidth + kBackHpBarWidth), static_cast<int>(base.y + kHpBarHeight + kBackHpBarHeight),
		0xf0f0f0, true);

	// �ʏ��HP�o�[
	DrawBox(static_cast<int>(base.x), static_cast<int>(base.y),
		static_cast<int>(base.x + kHpBarWidth * (m_hp / static_cast<float>(m_maxHp))), static_cast<int>(base.y + kHpBarHeight),
		0x08ff08, true);

	DrawFormatString(static_cast<int>(base.x), static_cast<int>(base.y - 32),
		0xffffff, L"%02d / %02d", m_hp, m_maxHp);
}
