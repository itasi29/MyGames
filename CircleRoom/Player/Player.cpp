#include "Player.h"
#include <DxLib.h>
#include "../Common/Input.h"
#include "../Application.h"

namespace
{
	// 当たり判定の半径の大きさ
	constexpr float kColRadius = 2.0f;

	// プレイヤーの各頂点までの長さ(プレイヤーの大きさ)
	constexpr float kDistanceVertex = 5.0f;
	// プレイヤーのスピード
	constexpr float kSpeed = 4.0f;
}

Player::Player(Application& app) :
	m_app(app),
	m_size(m_app.GetWindowSize()),
	m_colRaidus(kColRadius),
	m_isExsit(false)
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_pos = Vec2{ m_app.GetWindowSize().w / 2, m_app.GetWindowSize().h - 100.0f };
}

void Player::Update(Input& input)
{
	Move(input);
}

void Player::Draw()
{
	// 現状デバッグ用で円の描画
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 8, 0xffffff, true);
}

void Player::Move(Input& input)
{
	// ゼロベクトルに戻す
	m_vec = Vec2::zero();

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
	// スピードに変更
	m_vec *= kSpeed;

	// 座標に移動ベクトルを足す
	m_pos += m_vec;
}
