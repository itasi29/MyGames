#include <DxLib.h>
#include "Application.h"

#include "EnemyDash.h"

#include "Player/Player.h"

namespace
{
	// 動くスピード
	constexpr float kSpeed = 4.0f;
	// 半径
	constexpr float kRadius = 24.0f;

	// カラー
	constexpr int kColor = 0x0808ff;

	// ログ数
	constexpr int kDashLogNum = 8;
	// ダッシュ時のスピード
	constexpr float kDashSpeed = 15.0f;
	// ダッシュ開始までのスピード
	constexpr float kStartWaitSpeed = 2.5f;

	// ダッシュ待機時間
	constexpr int kWaitDashFrame = 60 * 5;
	// ダッシュを開始するまでのフレーム
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
}

EnemyDash::~EnemyDash()
{
}

void EnemyDash::Init(const Vec2& pos)
{
	// 引数で渡された位置を初期位置に
	m_pos = pos;
	m_radius = kRadius;

	// フレームの初期化
	m_frame = 0;

	// 撃つ方向をランダムで決める
	float moveX = (GetRand(16) - 8) * 0.125f;
	float moveY = (GetRand(16) - 8) * 0.125f;
	m_vec = Vec2{ moveX, moveY };

	// ゼロベクトルでないなら正規化
	if (m_vec.SqLength() > 0)
	{
		m_vec.Normalize();
	}
	// ゼロベクトルなら方向を真横にする
	else
	{
		m_vec = Vec2{ 1.0f, 0.0f };
	}

	// スピードを調整
	m_vec *= kSpeed;
}

void EnemyDash::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// 変わるときに当たり判定も入れる
		m_col.SetCenter(m_pos, m_radius);

		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyDash::NormalUpdate()
{
	Dash();
	m_pos += m_vec;

	// 反射したかつ、ダッシュ状態なら通常状態に戻す
	if (Reflection() && m_isDash)
	{
		// ダッシュ終了
		m_isDash = false;

		// ダッシュ状態から通常速度に戻る
		m_vec = m_vec.GetNormalized() * kSpeed;

		// 待機時間を戻す
		m_waitDashFrame = kWaitDashFrame;
		// スタート待機時間を戻す
		m_startWaitDashFrame = kStartWaitDashFrame;
	}

	m_col.SetCenter(m_pos, m_radius);
}

void EnemyDash::NormalDraw()
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), m_color, true);

	if (m_logFrame < kDashLogNum)
	{
		for (int i = 0; i < kDashLogNum; i++)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (255 - (m_logFrame + i) * (255 / kDashLogNum)));
			DrawCircle(static_cast<int>(m_posLog[i].x), static_cast<int>(m_posLog[i].y),
				static_cast<int>(m_radius), m_color, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

#ifdef _DEBUG
	// 当たり判定の描画
	m_col.Draw(0xff0000, false);
#endif
}

void EnemyDash::Dash()
{
	m_logFrame++;
	// ダッシュ中ならログのみ更新
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

	// 待機時間待ち処理
	if (m_waitDashFrame > 0)
	{
		m_waitDashFrame--;

		// 待機時間が終わったら
		if (m_waitDashFrame <= 0)
		{
			// 正規化
			m_vec.Normalize();
			// 速度の変更
			m_vec *= kStartWaitSpeed;
		}
		return;
	}
	
	// スタートまでのフレームを減らす
	m_startWaitDashFrame--;

	// スタート待機が終わったら
	if (m_startWaitDashFrame <= 0)
	{
		m_isDash = true;

		// 現在の位置からプレイヤーまでのベクトルの生成
		m_vec = m_player->GetPos() - m_pos;
		// 正規化
		m_vec.Normalize();
		// スピードに変換
		m_vec *= kDashSpeed;

		// ログの更新
		for (auto& log : m_posLog)
		{
			log = m_pos;
		}
		m_logFrame = 0;
	}
}
