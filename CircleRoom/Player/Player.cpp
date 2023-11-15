#include "Player.h"
#include <DxLib.h>
#include "../Common/Input.h"
#include "../Application.h"

#include "../Utility/Matrix3x3.h"

namespace
{
	// 当たり判定の半径の大きさ
	constexpr float kColRadius = 2.0f;

	// プレイヤーの各頂点までの長さ(プレイヤーの大きさ)
	constexpr float kDistanceVertex = 24.0f;
	// プレイヤーのスピード
	constexpr float kSpeed = 4.0f;

	// 回転の合成行列(60度)
	Matrix3x3 kMatAngle;
	// 線形補間を行うフレーム
	constexpr int kLinearInterpolationFrame = 10;
}

Player::Player(Application& app) :
	m_app(app),
	m_size(m_app.GetWindowSize()),
	m_lineInterpolateFrame(0),
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

	// 線形補間の間は処理を行う
	if (m_lineInterpolateFrame > 0)
	{
		m_lineInterpolateFrame--;
		m_nowFront += m_lineInterpolate;
	}
}

void Player::Draw()
{
	// 現状デバッグ用で円の描画
//	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 8, 0xffffff, true);

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
	m_lineInterpolate = (m_front - m_nowFront) / kLinearInterpolationFrame;
	m_lineInterpolateFrame = kLinearInterpolationFrame;

	// スピードに変更
	m_vec *= kSpeed;

	// 座標に移動ベクトルを足す
	m_pos += m_vec;
}
