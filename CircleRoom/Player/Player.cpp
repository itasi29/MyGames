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
	constexpr float kDashSpeed = 4.0f;
	// ダッシュ可能時間
	constexpr int kDashFrame = 50;
	// ダッシュ待機時間
	constexpr int kDashWaitFrame = 25;

	// 回転の合成行列
	Matrix4x4 kMatRight;
	Matrix4x4 kMatLeft;
	// 線形補間を行うフレーム
	constexpr int kInterpolatedFrame = 4;
}

Player::Player(Application& app) :
	m_app(app),
	m_size(m_app.GetWindowSize()),
	m_interpolatedFrame(-1),
	m_dashFrame(-1),
	m_dashWaitFrame(-1),
	m_isDash(false),
	m_colRaidus(kColRadius),
	m_isExsit(false)
{
	m_pos = Vec2{ m_size.w / 2.0f, m_size.h - 100.0f };

	kMatRight.AngleAxisZ(DX_PI_F / 3.0f * 2.0f);
	kMatLeft.AngleAxisZ(DX_PI_F / 3.0f * 4.0f);

	m_nowFront = Vec2::Up();
	m_frontVec = m_nowFront * kDistanceVertex;
	m_rightVec = kMatRight * m_nowFront * kDistanceVertex;
	m_leftVec = kMatLeft * m_nowFront * kDistanceVertex;
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

	DrawFormatString(32, 32, 0xffffff, L"%.2f, %.2f", m_pos.x, m_pos.y);
	DrawFormatString(32, 32 + 16, 0xffffff, L"f:%.2f, %.2f", m_frontVec.x, m_frontVec.y);
	DrawFormatString(32, 32 + 32, 0xffffff, L"r:%.2f, %.2f", m_rightVec.x, m_rightVec.y);
	DrawFormatString(32, 32 + 48, 0xffffff, L"l:%.2f, %.2f", m_leftVec.x, m_leftVec.y);
}

void Player::Move(Input& input)
{
	// ゼロベクトルに戻す
	m_vec = Vec2::Zero();

	//m_vec = input.GetStickDate();

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

	Lerp();

	m_vec *= kSpeed;

	Dash(input);

	// 座標に移動ベクトルを足す
	m_pos += m_vec;
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

void Player::Lerp()
{
	// 動いていたら線形補間を更新する
	if (m_vec != m_nowFront)
	{
		m_interpolatedValue = m_vec - m_nowFront;
		m_interpolatedValue = m_interpolatedValue.GetNormalized() / kInterpolatedFrame;
		m_interpolatedFrame = kInterpolatedFrame;

		m_nowFront = m_vec;
	}

	// 線形補間をしない場合は補間処理しない
	if (m_interpolatedFrame < 0) return;

	// 線形補間時間の更新
	m_interpolatedFrame--;
	// 現在の正面方向の更新
	//m_nowFront += m_interpolatedValue;

	//m_frontVec = m_nowFront * kDistanceVertex;
	//m_rightVec = kMatRight * m_nowFront * kDistanceVertex;
	//m_leftVec = kMatLeft * m_nowFront * kDistanceVertex;

	m_frontVec *= m_interpolatedValue;
	m_rightVec *= m_interpolatedValue;
	m_leftVec *= m_interpolatedValue;

	m_frontVec *= kDistanceVertex;
	m_rightVec *= kDistanceVertex;
	m_leftVec *= kDistanceVertex;
}
