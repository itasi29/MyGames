#pragma once

#include "EnemyBase.h"

class EnemyNormal : public EnemyBase
{
public:
	EnemyNormal(const Size& windowSize, float fieldSize);
	virtual ~EnemyNormal();

	void Init(Vec2 pos) override;

private:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;

	// �`��֐�
	void StartDraw() override;
	void NormalDraw() override;
};

