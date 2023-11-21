#pragma once

#include "EnemyBase.h"

class EnemyNormal : public EnemyBase
{
public:
	EnemyNormal(const Size& windowSize, float fieldSize);
	virtual ~EnemyNormal();

	void Init(Vec2 pos) override;

private:
	// 更新関数
	void StartUpdate() override;
	void NormalUpdate() override;

	// 描画関数
	void StartDraw() override;
	void NormalDraw() override;
};

