#include <DxLib.h>
#include "../Application.h"
#include "Player.h"
#include "../Common/Input.h"
#include "../Utility/Matrix3x3.h"

namespace
{
	// 当たり判定の半径の大きさ
	constexpr float kColRadius = 2.0f;

	// プレイヤーの各頂点までの長さ(プレイヤーの大きさ)
	constexpr float kDistanceVertex = 24.0f;
	// プレイヤーのスピード
	constexpr float kSpeed = 4.0f;
	// ダッシュ時のスピード倍率
	constexpr float kDashAdd = 2.0f;
	// ダッシュ可能時間
	constexpr int kDashFrame = 50;
	// ダッシュ待機時間
	constexpr int kDashWaitFrame = 25;

	// 回転の合成行列(60度)
	Matrix3x3 kMatAngle;
	// 線形補間を行うフレーム
	constexpr int kInterpolatedFrame = 4;
}

Player::Player(Application& app) :
	m_app(app),
	m_size(m_app.GetWindowSize()),
	m_interpolatedFrame(0),
	m_dashFrame(0),
	m_dashWaitFrame(0),
	m_colRaidus(kColRadius),
	m_isExsit(false)
{
	m_pos = Vec2{ m_size.w / 2.0f, m_size.h - 100.0f };

	kMatAngle.Angle(DX_PI_F / 3.0f * 2.0f);

	m_front = Vec2::Up() * kDistanceVertex;
	m_nowFront = m_front;
}

Player::~Player()
{
}

void Player::Update(Input& input)
{
	Move(input);
	Dash(input);

	// 線形補間の間は処理を行う
	if (m_interpolatedFrame > 0)
	{
		m_interpolatedFrame--;
		m_nowFront += m_interpolatedValue;
	}
}

void Player::Draw()
{
	// プレイヤーを三角形で描画
	// 上から順に正面、左、右
	Vec2 right = kMatAngle * m_nowFront;
	Vec2 left = kMatAngle * right;
	DrawTriangle(static_cast<int>(m_nowFront.x + m_pos.x), static_cast<int>(m_nowFront.y + m_pos.y),
		static_cast<int>(left.x + m_pos.x), static_cast<int>(left.y + m_pos.y),
		static_cast<int>(right.x + m_pos.x), static_cast<int>(right.y + m_pos.y),
		0xffffff, true);

	DrawFormatString(32, 32, 0xffffff, L"%.2f, %.2f", m_pos.x, m_pos.y);
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

	// ゼロベクトルなら以降の処理を行わない
	if (m_vec.SqLength() == 0)	return;

	// 移動ベクトルを正規化
	m_vec.Normalize();

	// 方向の更新
	m_front = m_vec * kDistanceVertex;
	// 変わった向きまでの線形補間を作成
	m_interpolatedValue = (m_front - m_nowFront) / kInterpolatedFrame;
	m_interpolatedFrame = kInterpolatedFrame;

	m_vec *= kSpeed;

	// 座標に移動ベクトルを足す
	m_pos += m_vec;
}

void Player::Dash(Input& input)
{
	// todo:ダッシュの連続使用不可とフレームの更新をやって

	if (m_dashFrame > 0 && input.IsPress("dash"))
	{
		m_dashFrame--;
		m_vec *= kDashAdd;
	}
	// 離されたときか時間経過したとき
	else
	{

	}
}
