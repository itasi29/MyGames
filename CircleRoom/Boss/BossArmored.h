#pragma once
#include <list>
#include "BossBase.h"

class BossArmored : public BossBase
{
public:
	BossArmored(const Size& windowSize, float fieldSize, int maxHp);
	virtual ~BossArmored();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">初期位置</param>
	void Init(Vec2& pos) override;

	void OnAttack(const Collision& rect) override;

protected:
	// 更新関数
	void StartUpdate() override;
	void NormalUpdate() override;

private:
	// 実際の移動は別の動きとなるので変数としてもっておく
	Vec2 m_conversionVec;

	// ラジアン
	float m_radian;

	// ダメージを与える弾のリスト
};

