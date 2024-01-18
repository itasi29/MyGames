#include <DxLib.h>
#include "Application.h"
#include "Common/Input.h"
#include "Stage/StageManager.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"

#include "Player.h"


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

	// ダッシュログ数
	constexpr int kDashLogNum = 8;
	// ダッシュ時のスピード
	constexpr float kDashSpeed = 12.0f;
	// ダッシュ可能時間
	constexpr int kDashFrame = 25;
	// ダッシュ待機時間
	constexpr int kDashWaitFrame = 25;

	// 線形補間を行うフレーム
	constexpr int kInterpolatedFrame = 50;

	// 死亡エフェクト1つの出ているフレーム数
	constexpr int kDeathEffectFrame = 3;
	// 死亡エフェクトのフレーム
	constexpr int kDeathFrame = 24 * kDeathEffectFrame;
	// 画像サイズ
	constexpr int kDeathGraphSize = 100;
	// 画像の行数
	constexpr int kRow = 6;
	// 画像の列数
	constexpr int kLine = 6;
}

Player::Player(const size& windowSize, float fieldSize) :
	m_size(windowSize),
	m_fieldSize(fieldSize),
	m_logFrame(kDashLogNum),
	m_dashFrame(0),
	m_dashWaitFrame(0),
	m_isDash(false),
	m_isExsit(false),
	m_deathFrame(0),
	m_isDeathEffect(false)
{
	Init();
	m_isExsit = false;
	m_posLog.resize(kDashLogNum);

	auto& mgr = GameManager::GetInstance();
	m_bloodImg = mgr.GetFile()->LoadGraphic(L"Player/blood.png");
}

Player::~Player()
{
}

void Player::Init()
{
	// 初期化処理
	m_dashFrame = 0;
	m_dashWaitFrame = 0;
	m_isDash = false;
	m_isExsit = true;
	m_isDeathEffect = false;

	// 位置の設定
	m_pos = Vec2{ m_size.w / 2.0f, m_size.h - m_fieldSize };

	// 方向の設定
	m_nowFront = Vec2::Up();
	m_frontVec = m_nowFront * kSize;
	m_rightVec = m_nowFront.Right() * kSize * 0.5f;
	m_leftVec = m_nowFront.Left() * kSize * 0.5f;

	// 当たり判定の更新
	m_col.SetCenter(m_pos, kColRadius, m_nowFront.x * kColShift, m_nowFront.y * kColShift);
}

void Player::Update(Input& input, Ability ability)
{
	// 死亡エフェクト
	if (m_isDeathEffect)
	{
		m_deathFrame++;

		if (m_deathFrame > kDeathFrame)
		{
			m_isDeathEffect = false;
		}
	}

	// 死んでいれば処理をしない
	if (!m_isExsit) return;

	Move(input);
	// アビリティ処理
	switch (ability)
	{
		// アビリティなし
	case kNone:
	default:
		break;

		// ダッシュ
	case kDash:
		Dash(input);
		break;
	}
	// ログの更新
	for (int i = kDashLogNum - 1; i > 0; i--)
	{
		m_posLog[i] = m_posLog[i - 1];
	}
	m_posLog[0] = m_pos;
	// 位置の更新
	m_pos += m_vec;

	InRange();

	// 当たり判定の更新
	m_col.SetCenter(m_pos, kColRadius, m_nowFront.x * kColShift, m_nowFront.y * kColShift);
}

void Player::Draw()
{
	// プレイヤーを三角形で描画
	// 上から順に正面、左、右
	// 生きていたら白
	if (m_isExsit)
	{
		DrawTriangle(static_cast<int>(m_frontVec.x + m_pos.x), static_cast<int>(m_frontVec.y + m_pos.y),
			static_cast<int>(m_leftVec.x + m_pos.x), static_cast<int>(m_leftVec.y + m_pos.y),
			static_cast<int>(m_rightVec.x + m_pos.x), static_cast<int>(m_rightVec.y + m_pos.y),
			0xffffff, true);

		// ダッシュした時のログを描画
		if (m_logFrame < kDashLogNum)
		{
			for (int i = 0; i < kDashLogNum; i++)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, (255 - (m_logFrame + i) * (255 / kDashLogNum)));
				DrawTriangle(static_cast<int>(m_frontVec.x + m_posLog[i].x), static_cast<int>(m_frontVec.y + m_posLog[i].y),
					static_cast<int>(m_leftVec.x + m_posLog[i].x), static_cast<int>(m_leftVec.y + m_posLog[i].y),
					static_cast<int>(m_rightVec.x + m_posLog[i].x), static_cast<int>(m_rightVec.y + m_posLog[i].y),
					0xffffff, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
	// 死んでいたら赤
	else
	{
		DrawTriangle(static_cast<int>(m_frontVec.x + m_pos.x), static_cast<int>(m_frontVec.y + m_pos.y),
			static_cast<int>(m_leftVec.x + m_pos.x), static_cast<int>(m_leftVec.y + m_pos.y),
			static_cast<int>(m_rightVec.x + m_pos.x), static_cast<int>(m_rightVec.y + m_pos.y),
			0xff0000, true);
	}

	// 死亡時のエフェクト
	if (m_isDeathEffect)
	{
		int x = m_pos.x - kDeathGraphSize * 0.5f;
		int y = m_pos.y - kDeathGraphSize * 0.5f;
		int index = m_deathFrame / kDeathEffectFrame;
		int srcX = kDeathGraphSize * (index % kRow);
		int srcY = kDeathGraphSize * (index / kLine);

		DrawRectGraph(x, y, srcX, srcY, kDeathGraphSize, kDeathGraphSize, m_bloodImg->GetHandle(), true);
	}

#ifdef _DEBUG
	// 走っていない場合当たり判定の描画
	if (!m_isDash)
	{
		m_col.Draw(0xff0000, false);
	}
#endif
}

void Player::Death()
{
	m_isExsit = false;

	m_deathFrame = 0;
	m_isDeathEffect = true;
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

	// 座標に移動ベクトルを足す
//	m_pos += m_vec;
}

void Player::Dash(Input& input)
{
	m_logFrame++;

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

		// ログを現在の位置にする
		for (auto& log : m_posLog)
		{
			log = m_pos;
		}
	}

	// 現在ダッシュ中でないなら処理終了
	if (!m_isDash) return;
	//
	//// その前に現在の移動ベクトル分マイナスする(移動中と移動してない時で変わらないように)
	//m_pos -= m_vec;
	//// 移動ベクトルに現在向いている方向の単位ベクトル*スピードしたものを足す
	//m_pos += m_nowFront * kDashSpeed;
	m_vec = m_nowFront * kDashSpeed;

	// ログフレームの更新
	m_logFrame = 0;

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
	float centerX = m_size.w *0.5f;
	float centerY = m_size.h * 0.5f;

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
