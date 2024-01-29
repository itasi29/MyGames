#pragma once
#include <string>
#include <memory>
#include "../Vec2.h"

class StageBase;

class StageManager
{
public:
	StageManager();
	~StageManager();

	void Update();
	void Draw() const;

	void ChangeScene(const std::shared_ptr<StageBase> nextStage);
	void DeathMoveScreen(const Vec2& vec);

private:
	// 通常の描画
	void NormalUpdate();
	// 移動の描画
	void StageMoveUpdate();
	void DeathMoveUpdate();

	// 通常の描画
	void NormalDraw() const;
	// 移動時の描画
	void StageMoveDraw() const;
	void DeathMoveDraw() const;

	Vec2 GetPos(const std::string& stage);

private:
	using UpdateFunc_t = void(StageManager::*)();
	using DrawFunc_t = void(StageManager::*)() const;

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	// 現在のステージ
	std::shared_ptr<StageBase> m_stage;
	// 次のステージ
	std::shared_ptr<StageBase> m_nextStage;

	// ステージの場所
	Vec2 m_pos;
	// 次行く場所
	Vec2 m_targetPos;
	// 移動用
	Vec2 m_vec;

	// 画面描画用ハンドル
	int m_drawScreen;
	// 移動時の画面ハンドル
	int m_screen;
	// キープ用画面ハンドル
	int m_keepScreen;

	// デバッグ用
	Vec2 m_motoPos;

	// 死亡時の演出用
	Vec2 m_deathVec;
	// 動かすのがX軸基準か
	bool m_isBaseX;
	int m_deathMoveFrame;
	double m_deathMoveAngle;
};

