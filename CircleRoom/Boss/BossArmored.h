#pragma once
#include <list>
#include <memory>
#include "BossBase.h"

class StageBase;
class BossDamageObject;

/// <summary>
/// 一面ボス
/// </summary>
class BossArmored : public BossBase
{
public:
	BossArmored(const Size& windowSize, float fieldSize, int maxHp, StageBase* stage);
	virtual ~BossArmored();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">初期位置</param>
	void Init(Vec2& pos) override;

	void OnAttack(bool isDash, const Collision& col) override;

protected:
	// 更新関数
	void StartUpdate() override;
	void NormalUpdate() override;

	// 描画関数
	void NormalDraw() const override;

	void CreateEnemy();

private:
	StageBase* m_stage;

	// 実際の移動は別の動きとなるので変数としてもっておく
	Vec2 m_conversionVec;

	// ラジアン
	float m_radian;

	// ダメージを与える物体のリスト
	std::list<std::shared_ptr<BossDamageObject>> m_objects;

	// 生成間隔フレーム
	int m_createFrame;
	// 生成前待機フレーム
	int m_waitCreateFrame;
};

