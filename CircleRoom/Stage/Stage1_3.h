#pragma once
#include "StageBase.h"
class Stage1_3 : public StageBase
{
public:
	Stage1_3(GameManager& mgr, float fieldSize);
	~Stage1_3();

	void Init() override;
	void StartCheck() override;
	void ChangeStage(Input& input) override;

private:
	void CheckStageConditions() override;
	void DrawStageConditions(int drawY) override;
	void DrawArrow() const override;
	void DrawKilledEnemyType() const override;
	void CreateEnemy() override;
	void UpdateTime() override;

private:
	// 敵生成時間
	int m_createFrame;

	// ステージに入った時点でのクリア情報を保持
	bool m_isDownClear;
	bool m_isLeftClear;
};

