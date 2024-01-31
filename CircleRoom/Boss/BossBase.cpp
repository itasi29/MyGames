#include <DxLib.h>
#include "Application.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/ImageFile.h"
#include "FileSystem/FontSystem.h"

#include "BossBase.h"

namespace
{
	constexpr unsigned int kWhiteColor = 0xf0ece5;

	// タイトルでの動くスピード
	constexpr float kTitleSpeed = 4.0f;
	// タイトルでの大きさ
	constexpr double kTitleSize = 2.0;

	// 壁からの法線ベクトル
	const Vec2 kNorVecLeft = Vec2{ 1.0f,  0.0f };
	const Vec2 kNorVecRight = Vec2{ -1.0f,  0.0f };
	const Vec2 kNorVecUp = Vec2{ 0.0f,  1.0f };
	const Vec2 kNorVecDown = Vec2{ 0.0f, -1.0f };

	// ずらす方向
	const Vec2 kShiftSide = Vec2{ 0.0f, 0.2f };
	const Vec2 kShiftVert = Vec2{ 0.2f, 0.0f };

	// HPバーの描画位置
	constexpr int kDrawHpBarX = 960;
//	constexpr int kDrawHpBarY = 288;
	constexpr int kDrawHpBarY = 512;
	// HPバーの縦幅
	constexpr int kHpBarHeight = 56;

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
	constexpr double kDamageSize = 3.0;
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

	/*死亡時の演出について*/
	// 演出数
	constexpr int kPerformaceNum = 6;
	// 演出の間隔
	constexpr int kPerformanceInterval = 45;
	// 震わせるフレーム
	constexpr int kShakeFrame = 30;
	// エフェクトインターバル
	constexpr int kPerEffInterval = 3;
}

BossBase::BossBase(const size& windowSize, float fieldSize, int maxHp) :
	m_angle(0.0),
	m_size(windowSize),
	m_fieldSize(fieldSize),
	m_maxHp(maxHp),
	m_isExsit(true),
	m_hp(maxHp),
	m_hitStopFrame(kHitStopFrame),
	m_lineType(0),
	m_isEndPerformance(false)
{
	m_rippleScreen = MakeScreen(m_size.w, m_size.h, true);

	m_updateFunc = &BossBase::StartUpdate;
	m_drawFunc = &BossBase::StartDraw;
	m_deathUpdateFunc = &BossBase::ExplotionUpdate;
	m_deathDrawFunc = &BossBase::ExplotionDraw;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_wallEffect = mgr->LoadGraphic(L"Enemy/wallEffect.png");
	m_damageEffect = mgr->LoadGraphic(L"Enemy/damageEffect.png");
	m_hpBar = mgr->LoadGraphic(L"UI/HpBar.png");
	m_hpBarBack = mgr->LoadGraphic(L"UI/HpBarBack.png");
	m_hpBarDown = mgr->LoadGraphic(L"UI/HpBarDown.png");
	m_hpBarFrame = mgr->LoadGraphic(L"UI/HpBarFrame.png");
	m_performance = mgr->LoadGraphic(L"Enemy/endPerformance.png");

	m_createSe = mgr->LoadSound(L"Se/create.mp3");
	m_damageSe = mgr->LoadSound(L"Se/bossDamage.mp3");
}

BossBase::~BossBase()
{
	DeleteGraph(m_rippleScreen);
}

void BossBase::TitleInit()
{
	m_radius *= kTitleSize;
	// 出現場所の作成
	int rand = GetRand(3);
	int width = static_cast<int>(m_size.w - 1);
	int height = static_cast<int>(m_size.h - 1);
	// 上から
	if (rand == 0)
	{
		m_pos = { static_cast<float>(GetRand(width)), -m_radius };
	}
	// 下から
	else if (rand == 1)
	{
		m_pos = { static_cast<float>(GetRand(width)), m_radius };
	}
	// 左から
	else if (rand == 2)
	{
		m_pos = { -m_radius, static_cast<float>(GetRand(height)) };
	}
	// 右から
	else
	{
		m_pos = { m_radius, static_cast<float>(GetRand(height)) };
	}

	// ベクトルの作成
	m_vec = Vec2{m_size.w * 0.5f, m_size.h * 0.5f} - m_pos;
	// 速度の調整
	m_vec = m_vec.GetNormalized() * kTitleSpeed;
}

void BossBase::TitleUpdate()
{
	m_pos += m_vec;
	m_angle -= kRad;
	
	// 画面外判定
	// FIXME:時間があれば関数化
	// 左に動いているとき
	if (m_vec.x < 0)
	{
		// 下に動いているとき
		if (m_vec.y > 0)
		{
			if (m_pos.x + m_radius < 0 || m_pos.y - m_radius > m_size.h)
			{
				m_isExsit = false;
				return;
			}
		}
		// 上に動いているとき
		else
		{
			if (m_pos.x + m_radius < 0 || m_pos.y + m_radius < 0)
			{
				m_isExsit = false;
				return;
			}
		}
	}
	// 右に動いているとき
	else
	{
		// 下に動いているとき
		if (m_vec.y > 0)
		{
			if (m_pos.x - m_radius > m_size.w || m_pos.y - m_radius > m_size.h)
			{
				m_isExsit = false;
				return;
			}
		}
		// 上に動いているとき
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

void BossBase::TitleDraw()
{
	DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 2.0, m_angle,
		m_charImg->GetHandle(), true);
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

void BossBase::OnDeath()
{
	// 中心に向かうようにベクトルを調整
	m_vec = Vec2{ m_size.w * 0.5f, m_size.h * 0.5f } - m_pos;
	// ちょうど爆発(？)エフェクト終了時に中心辺りに来るように調整
	m_vec = m_vec / (kPerformaceNum * kPerformanceInterval);

	// それぞれの初期化
	m_endPerformanceFrame = 0;
	m_performanceNum = 0;
	m_isShake = false;
	m_isScatter = false;

	m_ripple1 = 64;
	m_ripple2 = 32;
	m_ripple3 = 16;

	m_updateFunc = &BossBase::DeathUpdate;
	m_drawFunc = &BossBase::DeathDraw;
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

	for (auto& eff : m_performanceEff)
	{
		eff.frame++;
		if (eff.frame > 9 * kPerEffInterval)
		{
			eff.isEnd = true;
		}
	}
	m_performanceEff.remove_if(
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

	if (m_endPerformanceFrame > 10)
	{
		m_isShake = false;
	}

	if (m_endPerformanceFrame > kPerformanceInterval)
	{
		m_performanceNum++;
		m_endPerformanceFrame = 0;
		Vec2 pos = m_pos;
		pos.x += (GetRand(30) - 15) * 10;
		pos.y += (GetRand(30) - 15) * 10;
		m_performanceEff.push_back({ pos, 0, false });
		// 震わせる
		m_isShake = true;

		// 最後の爆発エフェクト立った場合
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

	if (m_endPerformanceFrame > kShakeFrame)
	{
		m_endPerformanceFrame = 0;

		m_deathUpdateFunc = &BossBase::LastUpdate;
		m_deathDrawFunc = &BossBase::LastDraw;
	}
}

void BossBase::LastUpdate()
{
	m_endPerformanceFrame++;
	int kRippleSpeed = 14;

	m_ripple1 += kRippleSpeed;
	m_ripple2 += kRippleSpeed;
	m_ripple3 += kRippleSpeed;

	if (m_endPerformanceFrame > 30)
	{
		m_isEndPerformance = true;
	}

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

void BossBase::DeathDraw() const
{
	(this->*m_deathDrawFunc)();

	for (const auto& eff : m_performanceEff)
	{
		int srcX = 64 * (eff.frame / kPerEffInterval);
		int srcY = 64 * 5;

		DrawRectRotaGraph(static_cast<int>(eff.pos.x), static_cast<int>(eff.pos.y), srcX, srcY, 64, 64,
			3.0, 0.0, m_performance->GetHandle(), true);
	}
}

void BossBase::ExplotionDraw() const
{
	int x = static_cast<int>(m_pos.x);
	int y = static_cast<int>(m_pos.y);

	if (m_isShake)
	{
		x += GetRand(30) - 15;
		y += GetRand(30) - 15;
	}

	DrawRotaGraph(x, y, 1.0, m_angle,
		m_charImg->GetHandle(), true);
}

void BossBase::ShakeDraw() const
{
	int x = static_cast<int>(m_pos.x);
	int y = static_cast<int>(m_pos.y);

	x += GetRand(30) - 15;
	y += GetRand(30) - 15;

	DrawRotaGraph(x, y, 1.0, m_angle,
		m_charImg->GetHandle(), true);
}

void BossBase::LastDraw() const
{
	int x = static_cast<int>(m_pos.x);
	int y = static_cast<int>(m_pos.y);

	// TODO:波紋っぽくするところから
	int kLineThickness = 3;

	SetDrawScreen(m_rippleScreen);
	SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
	DrawBox(0, 0, m_size.w, m_size.h, 0x010101, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawCircle(x, y, m_ripple3, 0x789461, false, kLineThickness);
	DrawCircle(x, y, m_ripple2, 0x50623a, false, kLineThickness);
	DrawCircle(x, y, m_ripple1, 0x294b29, false, kLineThickness);

	int screenHandle = GameManager::GetInstance().GetScene()->GetScreenHandle();
	SetDrawScreen(screenHandle);

	DrawGraph(0, 0, m_rippleScreen, true);
}

void BossBase::DrawHpBar() const
{
	// フレームよりバーの方が小さいため位置調整
	int barX = kDrawHpBarX + 2;
	int barY = kDrawHpBarY + 4;

	// 背景のHPバー
	DrawGraph(barX, barY, m_hpBarBack->GetHandle(), true);
	// 赤いHPバーの描画
	DrawExtendGraph(barX, barY, 
		barX + m_hpDownWidth, barY + kHpBarHeight, 
		m_hpBarDown->GetHandle(), true);
	// 通常のHPバー
	DrawExtendGraph(barX, barY, 
		barX + m_hpWidth, barY + kHpBarHeight,
		m_hpBar->GetHandle(), true);
	// フレームの描画
	DrawGraph(kDrawHpBarX, kDrawHpBarY, m_hpBarFrame->GetHandle(), true);

	auto& font = GameManager::GetInstance().GetFont();
	DrawFormatStringToHandle(kDrawHpBarX, kDrawHpBarY -48, kWhiteColor, font->GetHandle(32),
		L"BOSS : %02d / %02d", m_hp, m_maxHp);
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

