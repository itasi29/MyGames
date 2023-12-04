#pragma once
#include "StageBase.h"

class Stage1_1 : public StageBase
{
public:
	Stage1_1(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize);
	~Stage1_1();

	void CheckStageConditions() override;
	void DrawStageConditions(bool isPlaying = false) override;
	void Init() override;
	void CreateEnemy() override;

	void ChangeStage(Input& input) override;

private:
	// �G��������
	int m_createFrame;

	// �N���A����
	std::vector<bool> m_isClear;
};

