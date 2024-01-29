#pragma once
#include "StageBase.h"
class Stage1_8 : public StageBase
{
public:
	Stage1_8(GameManager& mgr, Input& input);
	~Stage1_8();

	void Init() override;
	void StartCheck() override;
	void ChangeStage(Input& input) override;

private:
	void CheckStageConditions() override;
	int DrawStageConditions(int drawY) override;
	void DrawArrow() const override;
	void DrawKilledEnemyType() const override;

	void CreateEnemy() override;
	void CreateStrongBoss() override;
	void UpdateTime() override;

private:
	// ステージに入った時点でのクリア情報を保持
	bool m_isDownClear;
	bool m_isRightClear;
};

