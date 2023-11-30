#pragma once
#include "EnemyBase.h"
class EnemyMoveWall : public EnemyBase
{
public:
	EnemyMoveWall(const Size& windowSize, float fieldSize);
	virtual ~EnemyMoveWall();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="vec">撃つ方向 (上or下)</param>
	void Init(Vec2& vec) override;

private:
	// 更新関数
	void StartUpdate() override;
	void NormalUpdate() override;

	// 描画関数
	void StartDraw() override;
	void NormalDraw() override;
};

