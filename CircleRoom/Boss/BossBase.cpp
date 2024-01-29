#include <DxLib.h>
#include "Application.h"
#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/ImageFile.h"

#include "BossBase.h"

namespace
{
	// 壁からの法線ベクトル
	const Vec2 kNorVecLeft = Vec2{ 1.0f,  0.0f };
	const Vec2 kNorVecRight = Vec2{ -1.0f,  0.0f };
	const Vec2 kNorVecUp = Vec2{ 0.0f,  1.0f };
	const Vec2 kNorVecDown = Vec2{ 0.0f, -1.0f };

	// ずらす方向
	const Vec2 kShiftSide = Vec2{ 0.0f, 0.2f };
	const Vec2 kShiftVert = Vec2{ 0.2f, 0.0f };

	// HPバーの幅
	constexpr int kHpBarWidth = 180;
	// HPバーの高さ
	constexpr int kHpBarHeight = 50;

	// 背景HPバーの前後幅
	constexpr int kBackHpBarWidth = 10;
	// 背景HPバーの前後高さ
	constexpr int kBackHpBarHeight = 10;

	// 1エフェクト何フレームか
	constexpr int kWallEffectInterval = 3;
	// 壁に当たったエフェクトをするフレーム
	constexpr int kWallHitFrame = 19 * kWallEffectInterval;
	// 画像サイズ
	constexpr int kWallEffectGraphSize = 64;
	// 拡大率
	constexpr double kExtRate = 0.75;
	// 行数
	constexpr int kRow = 8;
	// 列数の種類数
	constexpr int kEffectTypeNum = 4;
	// 出すエフェクトの列の場所
	constexpr int kLine[kEffectTypeNum] = {
		0, 4, 7, 8
	};

	// ダメージエフェクトの画像サイズ
	constexpr int kDamageGraphSize = 64;
	// ダメージエフェクトサイズ
	constexpr double kDamageSize = 2.0;
	// ダメージエフェクトの画像の縦切り取り位置
	constexpr int kSrcY = 8 * kDamageGraphSize;
	// 横の数
	constexpr int kDamageRow = 11;
	// 1エフェクト何フレーム
	constexpr int kDamageEffInterval = 3;

	// ずらす量
	constexpr int kWallEffectSlide = 32;

	// HitStopフレーム
	constexpr int kHitStopFrame = 3;
}

BossBase::BossBase(const size& windowSize, float fieldSize, int maxHp) :
	m_size(windowSize),
	m_fieldSize(fieldSize),
	m_maxHp(maxHp),
	m_isExsit(true),
	m_hp(maxHp),
	m_hitStopFrame(kHitStopFrame),
	m_lineType(0)
{
	m_updateFunc = &BossBase::StartUpdate;
	m_drawFunc = &BossBase::StartDraw;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_wallEffect = mgr->LoadGraphic(L"Enemy/wallEffect.png");
	m_damageEffect = mgr->LoadGraphic(L"Enemy/damageEffect.png");

	m_createSe = mgr->LoadSound(L"Se/create.mp3");
	m_damageSe = mgr->LoadSound(L"Se/bossDamage.mp3");
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

	// HPがゼロになったら死亡とする
	if (m_hp <= 0)
	{
		// バーの描画問題でHPを0にしておく
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

	// 左
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
	// 右
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
	// 上
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
	// 下
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
	m_lineType = GetRand(kEffectTypeNum);

	// 法線ベクトルの2倍から現在のベクトルを引く
	m_vec = m_vec + norVec * norVec.Dot(-m_vec) * 2.0f;
}

void BossBase::ShiftReflection(const Vec2& shift)
{
	// FIXME:現状はこれでいいけど、できたら参考元にできるように

	Vec2 temp = m_vec;

	// 進んでいる方向にshift分進ませる
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

void BossBase::HitStopUpdate()
{
	if (m_hitStopFrame > kHitStopFrame)
	{
		m_updateFunc = &BossBase::NormalUpdate;
	}
	m_hitStopFrame++;
}

void BossBase::StartDraw() const
{
	float rate = static_cast<float>(m_frame) / static_cast<float>(kApeearFrame);
	int alpha = static_cast<int>(255 * rate);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, 0.0,
		m_charImg->GetHandle(), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void BossBase::NormalDraw() const
{
	DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, m_angle,
		m_charImg->GetHandle(), true);

	DrawHitWallEffect();

	DrawDamageEffect();

#ifdef _DEBUG
	// 当たり判定の描画
	m_col.Draw(0xff0000, false);
#endif
}

void BossBase::DrawHpBar() const
{
	Vec2 base = {m_size.w - kHpBarWidth * 1.5f, 160.0f};

	// 背景
	DrawBox(static_cast<int>(base.x - kBackHpBarWidth), static_cast<int>(base.y - kBackHpBarHeight),
		static_cast<int>(base.x + kHpBarWidth + kBackHpBarWidth), static_cast<int>(base.y + kHpBarHeight + kBackHpBarHeight),
		0xf0f0f0, true);

	// 通常のHPバー
	DrawBox(static_cast<int>(base.x), static_cast<int>(base.y),
		static_cast<int>(base.x + kHpBarWidth * (m_hp / static_cast<float>(m_maxHp))), static_cast<int>(base.y + kHpBarHeight),
		0x08ff08, true);

	DrawFormatString(static_cast<int>(base.x), static_cast<int>(base.y - 32),
		0xffffff, L"%02d / %02d", m_hp, m_maxHp);
}

void BossBase::DrawHitWallEffect() const
{
	// 壁に当たったエフェクトの描画
	if (m_wallHitFrame > 0)
	{
		int x = m_drawWallHitX - static_cast<int>(kWallEffectGraphSize * 0.5f - kWallEffectGraphSize * kExtRate * 0.5f);
		int y = m_drawWallHitY - static_cast<int>(kWallEffectGraphSize * 0.5f - kWallEffectGraphSize * kExtRate * 0.5f);

		int index = (kWallHitFrame - m_wallHitFrame) / kWallEffectInterval;
		int srcX = kWallEffectGraphSize * (index % kRow);
		int srcY = kWallEffectGraphSize * kLine[m_lineType];

		DrawRectRotaGraph(x, y, srcX, srcY, kWallEffectGraphSize, kWallEffectGraphSize, kExtRate, 0.0, m_wallEffect->GetHandle(), true);
	}
}

void BossBase::DrawDamageEffect() const
{
	if (m_onDamagetFrame > 0)
	{
		int x = m_drawOnDamagetX - static_cast<int>(kDamageGraphSize * 0.5f - kDamageGraphSize * kDamageSize * 0.5f);
		int y = m_drawOnDamagetY - static_cast<int>(kDamageGraphSize * 0.5f - kDamageGraphSize * kDamageSize * 0.5f);

		int index = (kOnDamageFrame - m_onDamagetFrame) / kDamageEffInterval;
		int srcX = kDamageGraphSize * (index % kDamageRow);

		DrawRectRotaGraph(x, y, srcX, kSrcY, kDamageGraphSize, kDamageGraphSize,
			kDamageSize, 0.0, m_damageEffect->GetHandle(), true);
	}
}

