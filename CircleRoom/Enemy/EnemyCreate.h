#pragma once
#include "EnemyBase.h"

class StageBase;

/// <summary>
/// EnemyChildを生成していく敵
/// </summary>
class EnemyCreate : public EnemyBase
{
public:
	EnemyCreate(const Size& windowSize, float fieldSize, StageBase* stage);
	virtual ~EnemyCreate();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">初期位置</param>
	void Init(const Vec2& pos) override;

private:
	// 更新関数
	void StartUpdate() override;
	void NormalUpdate() override;

private:
	StageBase* m_stage;

	// 変換後の移動ベクトル
	Vec2 m_conversionVec;

	// 角度
	float m_radian;
};

