#include <DxLib.h>
#include "Application.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/ImageFile.h"
#include "FileSystem/FontSystem.h"

#include "Enemy/EnemyBase.h"
#include "BossBase.h"

namespace
{
	constexpr unsigned int kWhiteColor = 0xf0ece5;

	// �^�C�g���ł̓����X�s�[�h
	constexpr float kTitleSpeed = 4.0f;
	// �^�C�g���ł̑傫��
	constexpr double kTitleSize = 2.0;

	// �ǂ���̖@���x�N�g��
	const Vec2 kNorVecLeft = Vec2{ 1.0f,  0.0f };
	const Vec2 kNorVecRight = Vec2{ -1.0f,  0.0f };
	const Vec2 kNorVecUp = Vec2{ 0.0f,  1.0f };
	const Vec2 kNorVecDown = Vec2{ 0.0f, -1.0f };

	// ���炷����
	const Vec2 kShiftSide = Vec2{ 0.0f, 0.2f };
	const Vec2 kShiftVert = Vec2{ 0.2f, 0.0f };

	// HP�o�[�̕`��ʒu
	constexpr int kDrawHpBarX = 960;
	constexpr int kDrawHpBarY = 512;
	// HP�o�[�̏c��
	constexpr int kHpBarHeight = 56;

	// �w�iHP�o�[�̑O�㕝
	constexpr int kBackHpBarWidth = 10;
	// �w�iHP�o�[�̑O�㍂��
	constexpr int kBackHpBarHeight = 10;

	// �摜�T�C�Y
	constexpr int kGraphSize = 162;
	// ���̃J�b�g��
	constexpr int kRectRow = 4;
	// �c�̃J�b�g��
	constexpr int kRectLine = 4;
	// �J�b�g��
	constexpr int kRectWidth = kGraphSize / kRectRow;
	constexpr int kRectHeight = kGraphSize / kRectLine;
	// �J�b�g���̃X�s�[�h�W
	constexpr float kRectSpeed[kRectRow] = { -1.0f, -0.5f, 0.5f, 1.0f };
	// �������������ɑ��x��������悤
	constexpr float kRectAngle = DX_PI_F / 180 * 4;



	// �G�t�F�N�g�̃X�s�[�h
	constexpr float kWallEffSpeed = 4.0f;
	// �G�t�F�N�g��
	constexpr int kWallEffNum = 5;
	// �ǂɓ��������G�t�F�N�g������t���[��
	constexpr int kWallEffFrame = 30;

	// HitStop�t���[��
	constexpr int kHitStopFrame = 1;

	/*���S���̉��o�ɂ���*/
	// ���o��
	constexpr int kPerformaceNum = 4;
	// ���o�̊Ԋu
	constexpr int kPerformanceInterval = 68;
	// �k�킹��t���[��
	constexpr int kShakeFrame = 60;
	// �G�t�F�N�g�C���^�[�o��
	constexpr int kPerEffInterval = 3;
	// �G�t�F�N�g�I���t���[��
	constexpr int kEndPerformanceFrame = 60;
	// �k�킹��ŏ��̑傫��
	constexpr int kShakeStartSize = 2;
	// �k�킹��ő�傫��
	constexpr int kShakeMaxSize = 50;
	// ���₵�Ă����傫��
	constexpr int kShakeAdd = static_cast<int>((kShakeMaxSize - kShakeStartSize) / kPerformaceNum);

	// �Ō�̉~�̐��̑傫���ƃX�s�[�h
	// MEMO:���̑傫���ƃX�s�[�h�͓����ق������h�����ǂ�
	constexpr int kRipple = 4;
}

BossBase::BossBase(const size& windowSize, float fieldSize, int maxHp) :
	m_angle({}),
	m_size(windowSize),
	m_fieldSize(fieldSize),
	m_maxHp(maxHp),
	m_isExsit(true),
	m_hp(maxHp),
	m_hitStopFrame(kHitStopFrame),
	m_isEndPerformance(false)
{
	m_rippleScreen = MakeScreen(m_size.w, m_size.h, true);

	m_updateFunc = &BossBase::StartUpdate;
	m_drawFunc = &BossBase::StartDraw;
	m_deathUpdateFunc = &BossBase::ExplotionUpdate;
	m_deathDrawFunc = &BossBase::ExplotionDraw;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_shadow = mgr->LoadGraphic(L"Enemy/ShadowBoss.png");
	m_wallEffect = mgr->LoadGraphic(L"Enemy/wallEff.png");
	m_hpBar = mgr->LoadGraphic(L"UI/HpBar.png");
	m_hpBarBack = mgr->LoadGraphic(L"UI/HpBarBack.png");
	m_hpBarDown = mgr->LoadGraphic(L"UI/HpBarDown.png");
	m_hpBarFrame = mgr->LoadGraphic(L"UI/HpBarFrame.png");
	m_particle = mgr->LoadGraphic(L"Enemy/missileEff.png");

	m_createSe = mgr->LoadSound(L"Se/create.mp3");
	m_damageSe = mgr->LoadSound(L"Se/bossDamage.mp3");
	m_explosionSe = mgr->LoadSound(L"Se/bossExplosion.mp3");
	m_explosionLastSe = mgr->LoadSound(L"Se/bossExplosionLast.mp3");
}

BossBase::~BossBase()
{
	DeleteGraph(m_rippleScreen);
}

void BossBase::TitleInit()
{
	m_radius *= kTitleSize;
	// �o���ꏊ�̍쐬
	int rand = GetRand(3);
	int width = static_cast<int>(m_size.w - 1);
	int height = static_cast<int>(m_size.h - 1);
	// �ォ��
	if (rand == 0)
	{
		m_pos = { static_cast<float>(GetRand(width)), -m_radius };
	}
	// ������
	else if (rand == 1)
	{
		m_pos = { static_cast<float>(GetRand(width)), m_radius };
	}
	// ������
	else if (rand == 2)
	{
		m_pos = { -m_radius, static_cast<float>(GetRand(height)) };
	}
	// �E����
	else
	{
		m_pos = { m_radius, static_cast<float>(GetRand(height)) };
	}

	// �x�N�g���̍쐬
	m_vec = Vec2{m_size.w * 0.5f, m_size.h * 0.5f} - m_pos;
	// ���x�̒���
	m_vec = m_vec.GetNormalized() * kTitleSpeed;
}

void BossBase::TitleUpdate()
{
	m_pos += m_vec;
	m_angle[0] += -kRad * 2;
	m_angle[1] += kRad;
	m_angle[2] += -kRad;
	
	// ��ʊO����
	// FIXME:���Ԃ�����Ί֐���
	// ���ɓ����Ă���Ƃ�
	if (m_vec.x < 0)
	{
		// ���ɓ����Ă���Ƃ�
		if (m_vec.y > 0)
		{
			if (m_pos.x + m_radius < 0 || m_pos.y - m_radius > m_size.h)
			{
				m_isExsit = false;
				return;
			}
		}
		// ��ɓ����Ă���Ƃ�
		else
		{
			if (m_pos.x + m_radius < 0 || m_pos.y + m_radius < 0)
			{
				m_isExsit = false;
				return;
			}
		}
	}
	// �E�ɓ����Ă���Ƃ�
	else
	{
		// ���ɓ����Ă���Ƃ�
		if (m_vec.y > 0)
		{
			if (m_pos.x - m_radius > m_size.w || m_pos.y - m_radius > m_size.h)
			{
				m_isExsit = false;
				return;
			}
		}
		// ��ɓ����Ă���Ƃ�
		else
		{
			if (m_pos.x - m_radius > m_size.w || m_pos.y + m_radius < 0)
			{
				m_isExsit = false;
				return;
			}
		}
	}
}

void BossBase::TitleDraw() const
{
	// �e�̕`��
	DrawRotaGraph(static_cast<int>(m_pos.x + 10), static_cast<int>(m_pos.y + 10), 1.0, m_angle[0],
		m_shadow->GetHandle(), true);

	for (int i = 0; i < kGraphNum; i++)
	{
		DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, m_angle[i],
			m_char[i]->GetHandle(), true);
	}
}

void BossBase::Update()
{
	for (auto& walls : m_wallEff)
	{
		for (auto& eff : walls.effs)
		{
			eff.pos += eff.vec;
		}
		walls.frame++;

		if (walls.frame > kWallEffFrame)
		{
			walls.isUse = false;
		}
	}
	m_wallEff.remove_if(
		[](const auto& walls)
		{
			return !walls.isUse;
		}

	);

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
	float centerX = m_size.w * 0.5f;
	float centerY = m_size.h * 0.5f;

	// ��
	if (m_pos.x - m_radius < centerX - m_fieldSize)
	{
		m_pos.x = centerX - m_fieldSize + m_radius;

		AddWallEff({ centerX - m_fieldSize, m_pos.y }, 4, 6, 16, 8);
		ReflectionCal(kNorVecLeft);
		ShiftReflection(kShiftSide);

		return true;
	}
	// �E
	if (m_pos.x + m_radius > centerX + m_fieldSize)
	{
		m_pos.x = centerX + m_fieldSize - m_radius;

		AddWallEff({ centerX + m_fieldSize, m_pos.y }, 4, -2, 16, 8);
		ReflectionCal(kNorVecRight);
		ShiftReflection(kShiftSide);

		return true;
	}
	// ��
	if (m_pos.y - m_radius < centerY - m_fieldSize)
	{
		m_pos.y = centerY - m_fieldSize + m_radius;

		AddWallEff({ m_pos.x, centerY - m_fieldSize }, 16, 8, 4, 6);
		ReflectionCal(kNorVecUp);
		ShiftReflection(kShiftVert);

		return true;
	}
	// ��
	if (m_pos.y + m_radius > centerY + m_fieldSize)
	{
		m_pos.y = centerY + m_fieldSize - m_radius;

		AddWallEff({ m_pos.x, centerY + m_fieldSize }, 16, 8, 4, -2);
		ReflectionCal(kNorVecDown);
		ShiftReflection(kShiftVert);

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

void BossBase::HitStop()
{
	m_hitStopFrame = 0;
	m_updateFunc = &BossBase::HitStopUpdate;
}

void BossBase::OnDeath()
{
	// ���S�Ɍ������悤�Ƀx�N�g���𒲐�
	m_vec = Vec2{ m_size.w * 0.5f, m_size.h * 0.5f } - m_pos;
	// ���傤�ǔ���(�H)�G�t�F�N�g�I�����ɒ��S�ӂ�ɗ���悤�ɒ���
	m_vec = m_vec / (kPerformaceNum * kPerformanceInterval);

	// ���ꂼ��̏�����
	m_endPerformanceFrame = 0;
	m_performanceNum = 0;
	m_isShake = false;
	m_isScatter = false;

	m_ripple1 = 64;
	m_ripple2 = 32;
	m_ripple3 = 16;

	m_updateFunc = &BossBase::DeathUpdate;
	m_drawFunc = &BossBase::DeathDraw;

	m_particles.clear();
	m_shakeSize = kShakeStartSize;
}

void BossBase::HitStopUpdate()
{
	if (m_hitStopFrame > kHitStopFrame)
	{
		m_updateFunc = &BossBase::NormalUpdate;
	}
	m_hitStopFrame++;
}

void BossBase::DeathUpdate()
{
	(this->*m_deathUpdateFunc)();

	m_angle[0] += -kRad * 0.5;
	m_angle[1] += kRad * 0.25;
	m_angle[2] += -kRad * 0.25;

	for (auto& eff : m_particles)
	{
		eff.frame++;
		eff.pos += eff.vec;

		if (eff.frame > 30)
		{
			eff.isEnd = true;
		}
	}
	m_particles.remove_if(
		[](const auto& eff)
		{
			return eff.isEnd;
		}
	);
}

void BossBase::ExplotionUpdate()
{
	m_pos += m_vec;
	m_endPerformanceFrame++;

	AddParticleEff();

	if (m_endPerformanceFrame > 10)
	{
		m_isShake = false;
	}
	m_isShake = true;

	if (m_endPerformanceFrame > kPerformanceInterval)
	{
		m_performanceNum++;
		m_endPerformanceFrame = 0;
		// �k�킹��傫������
		m_isShake = true;
		m_shakeSize += kShakeAdd;
		// �������Ȃ炷
		auto& sound = GameManager::GetInstance().GetSound();
		sound->PlaySe(m_explosionSe->GetHandle());

		StartJoypadVibration(DX_INPUT_PAD1, 200 * m_performanceNum, 200, 1);

		// �Ō�̔����G�t�F�N�g�������ꍇ
		if (m_performanceNum >= kPerformaceNum)
		{
			m_deathUpdateFunc = &BossBase::ShakeUpdate;
			m_deathDrawFunc = &BossBase::ShakeDraw;
		}
	}
}

void BossBase::ShakeUpdate()
{
	m_endPerformanceFrame++;
	AddParticleEff();

	if (m_endPerformanceFrame > kShakeFrame)
	{
		m_endPerformanceFrame = 0;

		auto& mgr = GameManager::GetInstance();

		// �Ō�̔������炷
		auto& sound = mgr.GetSound();
		sound->PlaySe(m_explosionLastSe->GetHandle());

		StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, 1);

		// �V�F�[�_�[�̊J�n
		mgr.GetScene()->OnShader();

		m_deathUpdateFunc = &BossBase::LastUpdate;
		m_deathDrawFunc = &BossBase::LastDraw;
	}
}

void BossBase::LastUpdate()
{
	m_endPerformanceFrame++;

	m_ripple1 += kRipple;
	m_ripple2 += kRipple;
	m_ripple3 += kRipple;

	if (m_endPerformanceFrame > kEndPerformanceFrame)
	{
		m_isEndPerformance = true;
	}

}

void BossBase::StartDraw() const
{
	float rate = static_cast<float>(m_frame) / static_cast<float>(kApeearFrame);
	int alpha = static_cast<int>(255 * rate);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	// �e�̕`��
	DrawRotaGraph(static_cast<int>(m_pos.x + 10), static_cast<int>(m_pos.y + 10), 1.0, m_angle[0],
		m_shadow->GetHandle(), true);

	for (int i = 0; i < kGraphNum; i++)
	{
		DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, m_angle[i],
			m_char[i]->GetHandle(), true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void BossBase::NormalDraw() const
{// �e�̕`��
	DrawRotaGraph(static_cast<int>(m_pos.x + 10), static_cast<int>(m_pos.y + 10), 1.0, m_angle[0],
		m_shadow->GetHandle(), true);

	for (int i = 0; i < kGraphNum; i++)
	{
		DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, m_angle[i],
			m_char[i]->GetHandle(), true);
	}

	DrawHitWallEffect();

#ifdef _DEBUG
	// �����蔻��̕`��
	m_col.Draw(0xff0000, false);
#endif
}

void BossBase::DeathDraw() const
{
	(this->*m_deathDrawFunc)();

	for (const auto& eff : m_particles)
	{
		int alpha = static_cast<int>(255 * (1.0f - eff.frame / 30.0f));

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraph(static_cast<int>(eff.pos.x), static_cast<int>(eff.pos.y), eff.size, 0.0, m_particle->GetHandle(), true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void BossBase::ExplotionDraw() const
{
	int x = static_cast<int>(m_pos.x);
	int y = static_cast<int>(m_pos.y);

	x += GetRand(m_shakeSize) - static_cast<int>(m_shakeSize * 0.5);
	y += GetRand(m_shakeSize) - static_cast<int>(m_shakeSize * 0.5);

	// �e�̕`��
	DrawRotaGraph(x + 10, y + 10, 1.0, m_angle[0],
		m_shadow->GetHandle(), true);
	for (int i = 0; i < kGraphNum; i++)
	{
		DrawRotaGraph(x, y, 1.0, m_angle[i],
			m_char[i]->GetHandle(), true);
	}
}

void BossBase::ShakeDraw() const
{
	int x = static_cast<int>(m_pos.x);
	int y = static_cast<int>(m_pos.y);

	x += GetRand(kShakeMaxSize) - static_cast<int>(kShakeMaxSize * 0.5);
	y += GetRand(kShakeMaxSize) - static_cast<int>(kShakeMaxSize * 0.5);

	// �e�̕`��
	DrawRotaGraph(x + 10, y + 10, 1.0, m_angle[0],
		m_shadow->GetHandle(), true);
	for (int i = 0; i < kGraphNum; i++)
	{
		DrawRotaGraph(x, y, 1.0, m_angle[i],
			m_char[i]->GetHandle(), true);
	}
}

void BossBase::LastDraw() const
{
	int x = static_cast<int>(m_pos.x - kGraphSize * 0.5f);
	int y;

	float angle = m_endPerformanceFrame * kRectAngle;

	int alpha = static_cast<int>(255 * (1.0f - m_endPerformanceFrame / static_cast<float>(kEndPerformanceFrame)));

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	for (int rectX = 0; rectX < kRectRow; rectX++)
	{
		y = static_cast<int>(m_pos.y - kGraphSize * 0.5f);

		for (int rectY = 0; rectY < kRectLine; rectY++)
		{
			int drawX = x + static_cast<int>(kRectSpeed[rectX] * (m_endPerformanceFrame + abs(sinf(angle))));
			int drawY = y + static_cast<int>(kRectSpeed[rectY] * (m_endPerformanceFrame + abs(sinf(angle))));

			DrawRectRotaGraph(drawX, drawY, rectX * kRectWidth, rectY * kRectHeight, kRectWidth, kRectHeight,
				1.0, angle * (rectX * kRectRow + rectY), m_charAll->GetHandle(), true);
#if false
			DrawRotaGraph(drawX, drawY, 1.0, 0.0,
				m_charAll->GetHandle(), true);
#endif

			y += kRectHeight;
		}
		
		x += kRectWidth;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �g��̕`��
	x = static_cast<int>(m_pos.x);
	y = static_cast<int>(m_pos.y);

	SetDrawScreen(m_rippleScreen);
	SetDrawBlendMode(DX_BLENDMODE_MULA, 16);
	DrawBox(0, 0, m_size.w, m_size.h, 0x5f6976, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 96);
	DrawCircle(x, y, m_ripple3, 0x789461, false, kRipple);
	DrawCircle(x, y, m_ripple2, 0x50623a, false, kRipple);
	DrawCircle(x, y, m_ripple1, 0x294b29, false, kRipple);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int screenHandle = GameManager::GetInstance().GetScene()->GetScreenHandle();
	SetDrawScreen(screenHandle);

	DrawGraph(0, 0, m_rippleScreen, true);
}

void BossBase::DrawHpBar() const
{
	// �t���[�����o�[�̕������������߈ʒu����
	int barX = kDrawHpBarX + 2;
	int barY = kDrawHpBarY + 4;

	// �w�i��HP�o�[
	DrawGraph(barX, barY, m_hpBarBack->GetHandle(), true);
	// �Ԃ�HP�o�[�̕`��
	DrawExtendGraph(barX, barY, 
		barX + m_hpDownWidth, barY + kHpBarHeight, 
		m_hpBarDown->GetHandle(), true);
	// �ʏ��HP�o�[
	DrawExtendGraph(barX, barY, 
		barX + m_hpWidth, barY + kHpBarHeight,
		m_hpBar->GetHandle(), true);
	// �t���[���̕`��
	DrawGraph(kDrawHpBarX, kDrawHpBarY, m_hpBarFrame->GetHandle(), true);

	auto& font = GameManager::GetInstance().GetFont();
	DrawFormatStringToHandle(kDrawHpBarX, kDrawHpBarY -48, kWhiteColor, font->GetHandle(32),
		L"BOSS : %02d / %02d", m_hp, m_maxHp);
}

void BossBase::DrawHitWallEffect() const
{
	// �ǂɓ��������G�t�F�N�g�̕`��
	for (const auto& walls : m_wallEff)
	{
		int alpha = static_cast<int>(255 * (1.0f - walls.frame / static_cast<float>(kWallEffFrame)));

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		for (const auto& eff : walls.effs)
		{
			DrawRotaGraph(static_cast<int>(eff.pos.x), static_cast<int>(eff.pos.y), eff.size, 0.0, m_wallEffect->GetHandle(), true);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}


void BossBase::AddWallEff(const Vec2& pos, int sizeX, float shiftX, int sizeY, float shiftY)
{
	std::vector<WallEff> effs;
	effs.resize(kWallEffNum);

	float x, y;

	shiftX *= 0.125f;
	shiftY *= 0.125f;

	for (auto& eff : effs)
	{
		eff.size = 0.6 + GetRand(10) * 0.1 - 0.5;

		eff.pos = pos;

		x = GetRand(sizeX) * 0.125f - shiftX;
		y = GetRand(sizeY) * 0.125f - shiftY;

		eff.vec = { x, y };
		eff.vec.Normalize();

		eff.vec = eff.vec * static_cast<float>(eff.size) * kWallEffSpeed;
	}

	m_wallEff.push_back({ effs, true, 0 });
}

void BossBase::AddParticleEff()
{
	ParticleEff eff;

	// MEMO:����Ɠ��ڂ��O�X�^�[�g���ƕς��Ȃ�����
	int num = -1;

	do
	{
		eff.size = GetRand(10) * 0.1 + 0.2;

		float x = GetRand(16) * 0.125f - 1.0f;
		float y = GetRand(16) * 0.125f - 1.0f;

		eff.vec = { x, y };

		eff.vec.Normalize();

		eff.pos = m_pos + eff.vec * m_radius;

		eff.vec = eff.vec * static_cast<float>(eff.size) * 8.0f;

		m_particles.push_back(eff);

		num++;
	} while (num < m_performanceNum);
}

