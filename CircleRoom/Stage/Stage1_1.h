#pragma once
#include "StageBase.h"

class Stage1_1 : public StageBase
{
public:
	Stage1_1(StageManager& mgr, const Size& windowSize, float fieldSize);
	~Stage1_1();

	void Init() override;
	void ChangeStage(Input& input) override;
	void CheckStageConditions() override;
	void DrawStageConditions(bool isPlaying = false) override;
	void DrawArrow() const override;
	void CreateEnemy() override;


private:
	// ìGê∂ê¨éûä‘
	int m_createFrame;
};

