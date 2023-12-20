#pragma once
#include "StageBase.h"

class Stage1_2 : public StageBase
{
public:
	Stage1_2(GameManager& mgr, float fieldSize);
	~Stage1_2();

	void Init() override;
	void StartCheck() override;
	void ChangeStage(Input& input) override;

private:
	void CheckStageConditions() override;
	void DrawStageConditions(int drawY) override;
	void DrawArrow() const override;
	void DrawKilledEnemyType() const override;
	void CreateEnemy() override;
	void CreateNormal();
	void CreateLage();
	void UpdateTime() override;

private:
	// 敵(Lage)生成フレーム
	int m_createLageFrame;
	// 敵(Normal)生成フレーム
	int m_createNormalFrame;

	// ステージに入った時点でのクリア情報を保持
	bool m_isRightClear;
};
