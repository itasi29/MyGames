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
	void DrawStageConditions(int drawY) override;
	void DrawArrow() const override;
	void CreateEnemy() override;


private:
	// �G��������
	int m_createFrame;

	// �X�e�[�W�ɓ��������_�ł̃N���A����ێ�
	bool m_isLeftClear;
	bool m_isUpClear;
};

