#include <DxLib.h>
#include "Application.h"
#include "EnemyBase.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/ImageFile.h"

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

	// 1エフェクト何フレームか
	constexpr int kWallEffectInterval = 3;
	// 壁に当たったエフェクトをするフレーム
	constexpr int kWallHitFrame = 8 * kWallEffectInterval;
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


	// ずらす量
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

	// 左
	if (m_pos.x - m_radius * scale < centerX - m_fieldSize)
	{
		// 位置の修正
		m_pos.x = centerX - m_fieldSize + m_radius * scale;

		if (isShift)
		{
			// エフェクトフレーム初期化
			m_wallHitFrame = kWallHitFrame;

			// エフェクト描画位置
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
	// 右
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
	// 上
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
	// 下
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

	// 法線ベクトルの2倍から現在のベクトルを引く
	m_vec = m_vec + norVec * norVec.Dot(-m_vec) * 2.0f;
}

void EnemyBase::ShiftReflection(const Vec2& shift)
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
	// 当たり判定の描画
	m_col.Draw(0xff0000, false);
#endif
}

void EnemyBase::DrawHitWallEffect()
{
	// 壁に当たったエフェクトの描画
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
