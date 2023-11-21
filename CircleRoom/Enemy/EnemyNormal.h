#pragma once

#include "EnemyBase.h"

class EnemyNormal : public EnemyBase
{
public:
	EnemyNormal(const Size& windowSize, float fieldSize);
	virtual ~EnemyNormal();

	void Init(Vec2 pos) override;

private:
	// XVŠÖ”
	void StartUpdate() override;
	void NormalUpdate() override;

	// •`‰æŠÖ”
	void StartDraw() override;
	void NormalDraw() override;
};

