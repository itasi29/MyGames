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
	void Init(const Vec2& pos, bool isStart = false) override;

private:
	// 更新関数
	void StartUpdate() override;
	void NormalUpdate() override;

	void NormalDraw() override;

	// ダッシュに関する処理
	void Dash();

	void DrawDashEff() const;

private:
	std::shared_ptr<Player>& m_player;

	// ダッシュSe
	std::shared_ptr<FileBase> m_dashSe;
	// ダッシュ状態で壁に当たった時のエフェクト
	std::shared_ptr<FileBase> m_dashEff;

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

	// ダッシュの位置
	Vec2 m_dashEffPos;
	// ダッシュエフェクトのフレーム
	int m_dashEffFrame;
};

