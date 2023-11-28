#pragma once
#include "StageBase.h"

class Stage1_2 : public StageBase
{
public:
	Stage1_2(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize);
	~Stage1_2();

	void Init() override;
	void CreateEnemy() override;

	void ChangeStage(Input& input) override;

private:
	// “G¶¬ŠÔ
	int m_createFrame;
};
