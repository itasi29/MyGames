#include <DxLib.h>
#include "../Application.h"
#include "Player.h"
#include "../Common/Input.h"

namespace
{
	// 当たり判定の半径の大きさ
	constexpr float kColRadius = 10.0f;

	// プレイヤーの大きさ
	constexpr float kSize = 24.0f;
	// プレイヤーのスピード
	constexpr float kSpeed = 4.0f;
	// プレイヤーの中心から判定をどのくらい動かすか
	constexpr float kColShift = kSize * 0.32f;

	// ダッシュ時のスピード倍率
	constexpr float kDashSpeed = 4.0f;
	// ダッシュ可能時間
	constexpr int kDashFrame = 50;
	// ダッシュ待機時間
	constexpr int kDashWaitFrame = 25;

	// 線形補間を行うフレーム
	constexpr int kInterpolatedFrame = 50;
}

Player::Player(const Size& windowSize, float fieldSize) :
	m_windowSize(windowSize),
	m_fieldSize(fieldSize),
	m_dashFrame(-1),
	m_dashWaitFrame(-1),
	m_isDash(false),
	m_colRaidus(kColRadius),
	m_isExsit(false)
{
	m_pos = Vec2{ m_windowSize.w / 2.0f, m_windowSize.h - fieldSize };

	m_nowFront = Vec2::Up();
	m_frontVec = m_nowFront * kSize;
	m_rightVec = m_nowFront.Right() * kSize * 0.5f;
	m_leftVec = m_nowFront.Left() * kSize * 0.5f;

	m_colPos.SetCenter(m_pos, kColRadius, m_nowFront.x * kColShift, m_nowFront.y * kColShift);
}

Player::~Player()
{
}

void Player::Update(Input& input)
{
	Move(input);
	InRange();

	// 当たり判定の更新
	m_colPos.SetCenter(m_pos, kColRadius, m_nowFront.x * kColShift, m_nowFront.y * kColShift);
}

void Player::Draw()
{
	// プレイヤーを三角形で描画
	// 上から順に正面、左、右
	DrawTriangle(static_cast<int>(m_frontVec.x + m_pos.x), static_cast<int>(m_frontVec.y + m_pos.y),
		static_cast<int>(m_leftVec.x + m_pos.x), static_cast<int>(m_leftVec.y + m_pos.y),
		static_cast<int>(m_rightVec.x + m_pos.x), static_cast<int>(m_rightVec.y + m_pos.y),
		0xffffff, true);

	/*以下デバッグ用*/
#ifdef _DEBUG
	// 当たり判定描画
	//m_colPos.Draw(0xff0000, false);

	DrawFormatString(32, 32, 0xffffff, L"pos;%.2f, %.2f", m_pos.x, m_pos.y);
#endif
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

	if (m_vec.SqLength() > 0)
	{
		m_nowFront = m_vec;
		m_frontVec = m_nowFront * kSize;
		m_rightVec = m_nowFront.Right() * kSize * 0.5f;
		m_leftVec = m_nowFront.Left() * kSize * 0.5f;
	}

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

	// 現在ダッシュ中でないなら処理終了
	if (!m_isDash) return;
	
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

void Player::InRange()
{
	float centerX = m_windowSize.w *0.5f;
	float centerY = m_windowSize.h * 0.5f;

	// 左処理
	// 現在の位置から正面方向側の向きに中心をずらし(当たり判定の位置に持っていく)
	// 当たり判定の半径分ずらした位置が場外で位置をずらす
	if (m_pos.x + m_nowFront.x * kColShift - kColRadius < centerX - m_fieldSize)
	{
		// 場外の端から当たり判定の位置分ずらし
		// 半径分右にずらす
		m_pos.x = centerX - m_fieldSize - m_nowFront.x * kColShift + kColRadius;
	}
	// 右処理
	if (m_pos.x + m_nowFront.x * kColShift + kColRadius > centerX + m_fieldSize)
	{
		m_pos.x = centerX + m_fieldSize - m_nowFront.x * kColShift - kColRadius;
	}
	// 上処理
	if (m_pos.y + m_nowFront.y * kColShift - kColRadius < centerY - m_fieldSize)
	{
		m_pos.y = centerY - m_fieldSize - m_nowFront.y * kColShift + kColRadius;
	}
	// 下処理
	if (m_pos.y + m_nowFront.y * kColShift + kColRadius> centerY + m_fieldSize)
	{
		m_pos.y = centerY + m_fieldSize - m_nowFront.y * kColShift - kColRadius;
	}
}
