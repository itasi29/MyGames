#include <DxLib.h>
#include "../Application.h"
#include "Player.h"
#include "../Common/Input.h"

namespace
{
	// 当たり判定の半径の大きさ
	constexpr float kColRadius = 2.0f;

	// プレイヤーの大きさ
	constexpr float kSize = 24.0f;
	// プレイヤーのスピード
	constexpr float kSpeed = 4.0f;
	// ダッシュ時のスピード倍率
	constexpr float kDashSpeed = 4.0f;
	// ダッシュ可能時間
	constexpr int kDashFrame = 50;
	// ダッシュ待機時間
	constexpr int kDashWaitFrame = 25;

	// 線形補間を行うフレーム
	constexpr int kInterpolatedFrame = 50;
}

Player::Player(Application& app) :
	m_app(app),
	m_size(m_app.GetWindowSize()),
	m_interpolatedFrame(-1),
	m_interpolatedFrameNum(0),
	m_dashFrame(-1),
	m_dashWaitFrame(-1),
	m_isDash(false),
	m_colRaidus(kColRadius),
	m_isExsit(false)
{
	m_pos = Vec2{ m_size.w / 2.0f, m_size.h - 100.0f };

	m_nowFront = Vec2::Up();
	m_frontVec = m_nowFront * kSize;
	m_rightVec = m_nowFront.Right() * kSize * 0.5f;
	m_leftVec = m_nowFront.Left() * kSize * 0.5f;

	m_lastChangeVec = m_nowFront;
}

Player::~Player()
{
}

void Player::Update(Input& input)
{
	Move(input);
}

void Player::Draw()
{
	// プレイヤーを三角形で描画
	// 上から順に正面、左、右
	DrawTriangle(static_cast<int>(m_frontVec.x + m_pos.x), static_cast<int>(m_frontVec.y + m_pos.y),
		static_cast<int>(m_leftVec.x + m_pos.x), static_cast<int>(m_leftVec.y + m_pos.y),
		static_cast<int>(m_rightVec.x + m_pos.x), static_cast<int>(m_rightVec.y + m_pos.y),
		0xffffff, true);

	// プレイヤーの中心を表示
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 3, 0xff00ff, true);

	DrawFormatString(32, 32, 0xffffff, L"pos;%.2f, %.2f", m_pos.x, m_pos.y);
	DrawFormatString(32, 32+16, 0xff0000, L"vec:%.2f, %.2f", m_vec.x, m_vec.y);
	DrawFormatString(32, 32+32, 0xff0000, L"now:%.2f, %.2f", m_nowFront.x, m_nowFront.y);

	int centerX = m_size.w * 0.5f;
	int centerY = m_size.h * 0.5f;
	int lineLong = 30;
	DrawLine(centerX, centerY,
		centerX + m_lastChangeVec.x * lineLong, centerY + m_lastChangeVec.y * lineLong,
		0x0000ff, 2);
	DrawLine(centerX, centerY,
		centerX + m_firstChangeVec.x * lineLong, centerY + m_firstChangeVec.y * lineLong,
		0xff0000, 2);
	DrawLine(centerX, centerY,
		centerX + m_nowFront.x * lineLong, centerY + m_nowFront.y * lineLong,
		0x00ff00, 2);
}

void Player::Move(Input& input)
{
	// ゼロベクトルに戻す
	m_vec = Vec2::Zero();

	m_vec = input.GetStickDate();

	if (input.IsPress("up"))
	{
		m_vec.y--;
	}
	if (input.IsPress("down"))
	{
		m_vec.y++;
	}
	if (input.IsPress("left"))
	{
		m_vec.x--;
	}
	if (input.IsPress("right"))
	{
		m_vec.x++;
	}

	// 移動ベクトルを正規化
	m_vec.Normalize();

#if true
	Lerp();
#else
	if (m_vec.SqLength() > 0)
	{
		m_nowFront = m_vec;
		m_frontVec = m_nowFront * kSize;
		m_rightVec = m_nowFront.Right() * kSize * 0.5f;
		m_leftVec = m_nowFront.Left() * kSize * 0.5f;
	}
#endif

	m_vec *= kSpeed;

	Dash(input);

	// 座標に移動ベクトルを足す
	m_pos += m_vec;
}

void Player::Lerp()
{
	// 動いていたら線形補間を更新する
	if (m_vec != m_lastChangeVec && m_vec.SqLength() > 0)
	{
#if false
		// 二点間の差＋2を補間にかかる時間とする
		int num = abs(m_vec.x - m_nowFront.x + m_vec.y - m_nowFront.y)*5 + 2;
		// 補間の時間＋元からあった補間の時間を入れる
		m_interpolatedFrameNum = num + m_interpolatedFrame;
		// 補間の時間を入れる
		m_interpolatedFrame = num;
#else
		m_interpolatedFrameNum = kInterpolatedFrame;
		m_interpolatedFrame = kInterpolatedFrame;
#endif

		m_firstChangeVec = m_nowFront;
		m_lastChangeVec = m_vec;
	}

	// 線形補間をしない場合は補間処理しない
	if (m_interpolatedFrame < 0) return;
	// 現在の正面方向の更新
	float rate = static_cast<float>(m_interpolatedFrame) / static_cast<float>(m_interpolatedFrameNum);
	m_nowFront = m_lastChangeVec * (1.0f - rate) + m_firstChangeVec * (rate);

	m_nowFront.Normalize();

	m_frontVec = m_nowFront * kSize;
	m_rightVec = m_nowFront.Right() * kSize * 0.5f;
	m_leftVec = m_nowFront.Left() * kSize * 0.5f;

	// 線形補間時間の更新
	m_interpolatedFrame--;
}

void Player::Dash(Input& input)
{
	// ダッシュ待機時間中なら待機時間を減らして処理終了
	if (m_dashWaitFrame > 0)
	{
		m_dashWaitFrame--;
		return;
	}

	// ダッシュコマンドが押されたら
	if (input.IsTriggered("dash"))
	{
		// ダッシュするようにする
		m_isDash = true;
		// 使用時間の初期化
		m_dashFrame = kDashFrame;
	}

	// 現在ダッシュ中なら
	if (m_isDash)
	{
		// 移動ベクトルに現在向いている方向の単位ベクトル*スピードしたものを足す
		m_vec += m_nowFront * kDashSpeed;
		// 使用時間を減らす
		m_dashFrame--;
		// ダッシュを一定時間押し続けるか離したら終了
		if (m_dashFrame <= 0 || input.IsReleased("dash"))
		{
			m_isDash = false;
			// 待機時間の初期化
			m_dashWaitFrame = kDashWaitFrame;
		}
	}
}