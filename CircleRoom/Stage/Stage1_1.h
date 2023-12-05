#pragma once
#include "StageBase.h"

class Stage1_1 : public StageBase
{
public:
	Stage1_1(StageManager& mgr, const Size& windowSize, float fieldSize);
	~Stage1_1();

	void CheckStageConditions() override;
	void DrawStageConditions(bool isPlaying = false) override;
	void Init() override;
	void CreateEnemy() override;

	void ChangeStage(Input& input) override;

private:
	// “G¶¬ŠÔ
	int m_createFrame;
};

