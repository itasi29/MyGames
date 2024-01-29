#pragma once
#include "StageBase.h"

class Stage1_9 : public StageBase
{
public:
	Stage1_9(GameManager& mgr, Input& input);
	~Stage1_9();

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

private:
	// 生成数
	int m_createNum;

	// 敵生成時間
	int m_createFrame;

	// ステージに入った時点でのクリア情報を保持
	bool m_isUpClear;
};
