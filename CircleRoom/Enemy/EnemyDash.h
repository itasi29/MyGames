#pragma once
#include "EnemyBase.h"
#include <vector>
#include <memory>

class Player;

/// <summary>
/// ダッシュしてくる敵
/// </summary>
class EnemyDash : public EnemyBase
{
public:
	EnemyDash(const size& windowSize, float fieldSize, std::shared_ptr<Player>& player);
	virtual ~EnemyDash();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">初期位置</param>
	void Init(const Vec2& pos) override;

private:
	// 更新関数
	void StartUpdate() override;
	void NormalUpdate() override;

	void NormalDraw() override;

	// ダッシュに関する処理
	void Dash();

private:
	std::shared_ptr<Player>& m_player;

	// 位置ログ
	std::vector<Vec2> m_posLog;
	// ログフレーム
	int m_logFrame;
	// 現状ダッシュしているか
	bool m_isDash;
	// ダッシュするまでの待機フレーム
	int m_waitDashFrame;
	// ダッシュを開始するまでのフレーム
	int m_startWaitDashFrame;
};

