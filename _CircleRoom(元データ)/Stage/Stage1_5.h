#pragma once
#include "StageBase.h"
class Stage1_5 : public StageBase
{
public:
	Stage1_5(GameManager& mgr, Input& input);
	~Stage1_5();

	void Init() override;
	void StartCheck() override;
	void ChangeStage(Input& input) override;

private:
	void CheckStageConditions() override;
	int DrawStageConditions(int drawY) override;
	void DrawArrow() const override;
	void DrawKilledEnemyType() const override;
	void CreateEnemy() override;
	void UpdateTime() override;
	void CreateStrongBoss() override;

private:
	// 敵生成時間
	int m_createFrame;

	// ステージに入った時点でのクリア情報を保持
	bool m_isDownClear;
};

