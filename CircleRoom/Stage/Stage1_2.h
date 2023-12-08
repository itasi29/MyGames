#pragma once
#include "StageBase.h"

class Stage1_2 : public StageBase
{
public:
	Stage1_2(StageManager& mgr, const Size& windowSize, float fieldSize);
	~Stage1_2();

	void Init() override;
	void ChangeStage(Input& input) override;
	void CheckStageConditions() override;
	void DrawStageConditions(int drawY) override;
	void DrawArrow() const override;
	void CreateEnemy() override;

private:
	// 敵生成時間
	int m_createFrame;

	// ステージに入った時点でのクリア情報を保持
	bool m_isRightClear;
};
