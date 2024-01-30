#include <DxLib.h>
#include "StageBase.h"
#include "Stage1.h"
#include "StageManager.h"

namespace
{
	// 移動スピード
	constexpr float kSpeed = 100.0f;

	// ゆっくりになる距離
	constexpr float kSlowDis = 200.0f;

	// 下限速度
	constexpr float kLimitSlowSpeed = 10.0f;

	// ステージの縦横数
	constexpr int kLine = 3;
	constexpr int kRow = 3;

	// ステージ間の間隔
	constexpr int kStageMargin = 80;

	// 名前の場所
	const std::string kStName[kLine][kRow] =
	{
		{"1", "2", "3"},
		{"4", "5", "6"},
		{"7", "8", "9"}
	};

	// 横に伸びる大きさ
	constexpr int kWidth = 200;
	// 縦に伸びる大きさ
	constexpr int kHeight = 100;
	// 動かすフレーム
	constexpr int kDeathMoveFrame = 60;
}

StageManager::StageManager()
{
	m_drawScreen = MakeScreen(1280, 720, true);
	m_screen = MakeScreen(1280 * kRow + kStageMargin * kRow, 720 * kLine + kStageMargin * kLine, true);
	m_keepScreen = MakeScreen(1280 * kRow + kStageMargin * kRow, 720 * kLine + kStageMargin * kLine, true);

	m_updateFunc = &StageManager::NormalUpdate;
	m_drawFunc = &StageManager::NormalDraw;

	m_stage = std::make_shared<Stage1>(*this);
	m_nextStage = nullptr;

	m_pos = {};
	m_targetPos = {};
	m_vec = {};

	m_motoPos = {};
}

StageManager::~StageManager()
{
}

void StageManager::Update()
{
	(this->*m_updateFunc)();
}

void StageManager::Draw() const
{
	(this->*m_drawFunc)();

	/*デバッグ用の描画　本体には持って行かない*/ 
	// 大枠描画
	DrawBox(0, 0, 128 * kRow + 8 + kRow, 72 * kLine + 8 * kLine, 0xffffff, false);

	// 元の位置
	DrawBox(m_motoPos.x / 10, m_motoPos.y / 10, m_motoPos.x / 10+ 128, m_motoPos.y / 10 + 72, 0x00ff00, false);
	// 向かう位置
	DrawBox(m_targetPos.x / 10, m_targetPos.y / 10, m_targetPos.x / 10 + 128, m_targetPos.y / 10 + 72, 0xff0000, false);

	// 現在位置
	DrawBox(m_pos.x / 10, m_pos.y / 10, m_pos.x / 10 + 128, m_pos.y / 10 + 72, 0xffff00, false);

	clsDx();
	printfDx("pos : %.2f, %.2f\n", m_pos.x, m_pos.y);
	printfDx("vec : %.2f, %.2f\n", m_deathVec.x, m_deathVec.y);
}

void StageManager::ChangeScene(const std::shared_ptr<StageBase> nextStage)
{
	Vec2 pos;

	if (m_vec.Length() < kSlowDis)
	{
		// 描画用スクリーンに現在いるステージを描画
		SetDrawScreen(m_drawScreen);
		ClearDrawScreen();
		m_stage->Draw();

		// キープ用のスクリーンに場所を指定してさっきのを描画
		SetDrawScreen(m_keepScreen);
		ClearDrawScreen();
		pos = GetPos(m_stage->GetName());
		m_motoPos = pos;

		DrawGraph(pos.x, pos.y, m_drawScreen, true);
	}

	// 描画用スクリーンに次のステージの描画
	SetDrawScreen(m_drawScreen);
	ClearDrawScreen();
	nextStage->Draw();

	// 移動中描画スクリーンに場所を指定してさっきのを描画
	SetDrawScreen(m_screen);
	ClearDrawScreen();
	pos = GetPos(nextStage->GetName());

	DrawGraph(pos.x, pos.y, m_drawScreen, true);

	// キープにあるものを描画
	DrawGraph(0, 0, m_keepScreen, true);
		
	// 移動ベクトルの計算
	m_vec = (pos - m_pos).GetNormalized() * kSpeed;

	// 向かう先の場所の保存
	m_targetPos = pos;

	// メンバ関数ポインタの更新
	m_updateFunc = &StageManager::StageMoveUpdate;
	m_drawFunc = &StageManager::StageMoveDraw;

	// 次のステージの保存
	m_nextStage = nextStage;

	// 現在の描画先へと戻す(本来は)
	SetDrawScreen(DX_SCREEN_BACK);
}

void StageManager::DeathMoveScreen(const Vec2& vec)
{
	m_deathVec = vec;
	// 正規化
	m_deathVec.Normalize();

	// どちらが大きいか
	m_isBaseX = (abs(m_deathVec.x) > abs(m_deathVec.y));

	m_deathMoveFrame = 0;
	m_deathMoveAngle = 0.0;

	// メンバ関数ポインタの更新
	m_updateFunc = &StageManager::DeathMoveUpdate;
	m_drawFunc = &StageManager::DeathMoveDraw;
}

void StageManager::NormalUpdate()
{
	m_stage->Update();
}

void StageManager::StageMoveUpdate()
{
	//m_nextStage->Update();

	// 場所の更新
	m_pos += m_vec;

	// 距離の計算
	Vec2 vel = (m_targetPos - m_pos);
	float dis = vel.Length();

	// おそくする距離になったら
	if (dis < kSlowDis)
	{
		// 速度が0以下になったら移動終了
		if (dis < kLimitSlowSpeed)
		{
			m_updateFunc = &StageManager::NormalUpdate;
			m_drawFunc = &StageManager::NormalDraw;

			m_pos = m_targetPos;

			m_stage = m_nextStage;

			return;
		}

		// スピードの計算
		float speed = (dis / kSlowDis) * kSpeed;
		// スピードの調整
		m_vec = vel.GetNormalized() * speed;
	}
}

void StageManager::DeathMoveUpdate()
{
	m_deathMoveFrame++;
	m_deathMoveAngle += (DX_PI / 15) * 0.5;

	//if (m_deathMoveFrame >= kDeathMoveFrame)
	if (m_deathMoveAngle >= DX_PI)
	{
		m_updateFunc = &StageManager::NormalUpdate;
		m_drawFunc = &StageManager::NormalDraw;
	}
}

void StageManager::NormalDraw() const
{
	m_stage->Draw();
}

void StageManager::StageMoveDraw() const
{
	// MEMO:なぜかマイナスにすると描画が普通になった
	DrawGraph(-m_pos.x, -m_pos.y, m_screen, true);
}

void StageManager::DeathMoveDraw() const
{
	int x, y;

	int frame = (static_cast<int>(m_deathMoveFrame % kDeathMoveFrame) - (kDeathMoveFrame * 0.5f));
	float rate = (1.0f - abs(frame) / static_cast<float>(kDeathMoveFrame * 0.5f));

#if false
	if (m_isBaseX)
	{
		x = m_deathVec.x * rate * kWidth;
		y = m_deathVec.y * rate * kHeight;
	}
	else
	{
		y = m_deathVec.x * rate * kWidth;
		x = m_deathVec.y * rate * kHeight;
	}
#else 
	rate = sinf(m_deathMoveAngle);
	if (m_isBaseX)
	{
		x = m_deathVec.x * rate * kWidth;
		y = m_deathVec.y * rate * kHeight;
	}
	else
	{
		y = m_deathVec.x * rate * kWidth;
		x = m_deathVec.y * rate * kHeight;
	}
#endif

	DrawGraph(x, y, m_drawScreen, true);
}

Vec2 StageManager::GetPos(const std::string& stage)
{
	Vec2 pos;

	for (int x = 0; x < kRow; x++)
	{
		for (int y = 0; y < kLine; y++)
		{
			// ステージ名が一致したら
			if (kStName[y][x] == stage)
			{
				// 場所の保存
				pos.x = 1280 * x + kStageMargin * x;
				pos.y = 720 * y + kStageMargin * y;

				// 探すの終了
				break;
			}
		}
	}

	return pos;
}
