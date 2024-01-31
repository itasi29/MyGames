#pragma once
#include "EnemyBase.h"

class StageBase;

/// <summary>
/// 分裂して4方向に飛ばす敵
/// </summary>
class EnemyDivision : public EnemyBase
{
public:
	EnemyDivision(const size& windowSize, float fieldSize);
	EnemyDivision(const size& windowSize, float fieldSize, StageBase* stage);
	virtual ~EnemyDivision();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">初期位置</param>
	void Init(const Vec2& pos, bool isStart = false) override;

private:
	// 更新関数
	void StartUpdate() override;
	void NormalUpdate() override;

private:
	StageBase* m_stage;

	// 分裂待機前か
	bool m_isDivisionWait;
	// 分裂前待機フレーム
	int m_divisionWaitFrame;
};

