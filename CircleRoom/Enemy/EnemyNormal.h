#pragma once

#include "EnemyBase.h"

class EnemyNormal : public EnemyBase
{
public:
	EnemyNormal(const Size& windowSize, float fieldSize);
	virtual ~EnemyNormal();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">初期位置</param>
	void Init(Vec2& pos) override;

private:
	// 更新関数
	void StartUpdate() override;
	void NormalUpdate() override;

	// 描画関数
	void StartDraw() override;
	void NormalDraw() override;
};

