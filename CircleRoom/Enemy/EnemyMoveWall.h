#pragma once
#include "EnemyBase.h"
class EnemyMoveWall : public EnemyBase
{
public:
	EnemyMoveWall(const Size& windowSize, float fieldSize);
	virtual ~EnemyMoveWall();

	/// <summary>
	/// ‰Šú‰»ˆ—
	/// </summary>
	/// <param name="vec">Œ‚‚Â•ûŒü (ãor‰º)</param>
	void Init(Vec2& vec) override;

private:
	// XVŠÖ”
	void StartUpdate() override;
	void NormalUpdate() override;

	// •`‰æŠÖ”
	void StartDraw() override;
	void NormalDraw() override;
};

