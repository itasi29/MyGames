#pragma once
#include "StageBase.h"

class Stage1_2 : public StageBase
{
public:
	Stage1_2(StageManager& mgr, const Size& windowSize, float fieldSize);
	~Stage1_2();

	void CheckStageConditions() override;
	void DrawStageConditions(bool isPlaying = false) override;
	void Init() override;
	void CreateEnemy() override;

	void ChangeStage(Input& input) override;
	void SaveInf() const override;

private:
	// ìGê∂ê¨éûä‘
	int m_createFrame;
};
