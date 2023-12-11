#pragma once
#include "EnemyBase.h"
#include <memory>

class Player;

/// <summary>
/// ダッシュしてくる敵
/// </summary>
class EnemyDash : public EnemyBase
{
public:
	EnemyDash(const Size& windowSize, float fieldSize, std::shared_ptr<Player>& player);
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

	// ダッシュに関する処理
	void DashStartProcess();

private:
	std::shared_ptr<Player>& m_player;

	// 現状ダッシュしているか
	bool m_isDash;
	// ダッシュするまでの待機フレーム
	int m_waitDashFrame;
	// ダッシュを開始するまでのフレーム
	int m_startWaitDashFrame;
};

