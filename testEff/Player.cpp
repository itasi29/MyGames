#include <DxLib.h>

#include "Input.h"
#include "Player.h"

namespace
{
	// 色
	constexpr unsigned int kExsitColor = 0xFFFAE7;
	constexpr unsigned int kDeathColor = 0xD2001A;

	// 当たり判定の半径の大きさ
	constexpr float kColRadius = 6.0f;
	// オブジェクトとの当たり判定半径
	constexpr float kObjColRadius = 12.0f;

	// プレイヤーの大きさ
	constexpr float kSize = 24.0f;
	// プレイヤーのスピード
	constexpr float kSpeed = 4.0f;
	// プレイヤーの中心から判定をどのくらい動かすか
#ifdef false
	constexpr float kColShift = -kSize * 0.12f;
#else
	constexpr float kColShift = kSize * 0.32f;
#endif

	// 移動エフェクトフレーム
	constexpr int kEffFrame = 20;

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

Player::Player() :
	m_angle(0),
	m_logFrame(kDashLogNum),
	m_dashFrame(0),
	m_dashWaitFrame(0),
	m_isDash(false)
{
	Init();
	m_posLog.resize(kDashLogNum);
	m_dirLog.resize(kDashLogNum);
	m_angleLog.resize(kDashLogNum);

	handle = LoadGraph("_PlayerEff.png");
}

Player::~Player()
{
	DeleteGraph(handle);
}

void Player::Init()
{
	// 初期化処理
	m_effs.clear();
	m_angle = 0;
	m_dashFrame = 0;
	m_dashWaitFrame = 0;
	m_isDash = false;

	// 位置の設定
	m_pos = {320, 240};

	// 方向の設定
	m_front = Vec2::Up();
	m_dir.front = m_front * kSize;
	m_dir.right = m_front.Right() * kSize * 0.5f;
	m_dir.left = m_front.Left() * kSize * 0.5f;
}

void Player::Update(Input& input)
{
	Move(input);
	Dash(input);
	// 移動エフェクトの更新
	for (auto& eff : m_effs)
	{
		eff.frame++;

		if (eff.frame > kEffFrame)
		{
			eff.isEnd = true;
		}

		eff.pos += eff.vec;

		eff.angle += DX_PI_F / 180.0f;
	}

	m_effs.remove_if(
		[](const auto& eff) {
			return eff.isEnd;
		});
	// ログの更新
	for (int i = kDashLogNum - 1; i > 0; i--)
	{
		m_posLog[i] = m_posLog[i - 1];
		m_dirLog[i] = m_dirLog[i - 1];
		m_angleLog[i] = m_angleLog[i - 1];
	}
	m_posLog[0] = m_pos;
	m_dirLog[0] = m_dir;
	m_angleLog[0] = m_angle;
	// 位置の更新
	m_pos += m_vec;
}

void Player::Draw()
{
	double rate;
	int alpha;

	for (const auto& eff : m_effs)
	{
		rate = 1.0f - eff.frame / static_cast<double>(kEffFrame);
		alpha = static_cast<int>(153 * rate);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraph(eff.pos.x, eff.pos.y, 1.0, eff.angle, handle, true);
	}

	// ダッシュした時のログを描画
	if (m_logFrame < kDashLogNum)
	{
		for (int i = 0; i < kDashLogNum; i++)
		{
			rate = 1.0f - (m_logFrame + i + 1) / static_cast<double>(m_logFrame + kDashLogNum);
			alpha = static_cast<int>(128 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawTriangle(static_cast<int>(m_dirLog[i].front.x + m_posLog[i].x), static_cast<int>(m_dirLog[i].front.y + m_posLog[i].y),
				static_cast<int>(m_dirLog[i].left.x + m_posLog[i].x), static_cast<int>(m_dirLog[i].left.y + m_posLog[i].y),
				static_cast<int>(m_dirLog[i].right.x + m_posLog[i].x), static_cast<int>(m_dirLog[i].right.y + m_posLog[i].y),
				kExsitColor, true);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 現在のプレイヤーを描画
	DrawTriangleAA(m_dir.front.x + m_pos.x, m_dir.front.y + m_pos.y,
		m_dir.left.x + m_pos.x, m_dir.left.y + m_pos.y,
		m_dir.right.x + m_pos.x, m_dir.right.y + m_pos.y,
		kExsitColor, true);
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
		m_front = m_vec;

		for (int i = 0; i < 1; i++)
		{
			float effX = -m_vec.x * 0.5f + (GetRand(30) * 0.1f - 1.5f);
			float effY = -m_vec.y * 0.5f + (GetRand(30) * 0.1f - 1.5f);

			int randSize = kSize;

			Vec2 pos = m_pos + m_pos.Right().GetNormalized() * (GetRand(randSize) - kSize * 0.5f) * 0.8f;

			m_effs.push_back({ {effX, effY}, pos });
		}

#ifdef false
		m_angle = atan2(m_front.x, -m_front.y);
#else
		m_front = m_vec;
		m_dir.front = m_front * kSize;
		m_dir.right = m_front.Right() * kSize * 0.5f;
		m_dir.left = m_front.Left() * kSize * 0.5f;
#endif
	}

	m_vec *= kSpeed;
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
		for (int i = 0; i < kDashLogNum; i++)
		{
			m_posLog[i] = m_pos;
			m_dirLog[i] = m_dir;
			m_angleLog[i] = m_angle;
		}
	}

	// 現在ダッシュ中でないなら処理終了
	if (!m_isDash) return;

	// ダッシュさせる
	m_vec = m_front * kDashSpeed;

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
