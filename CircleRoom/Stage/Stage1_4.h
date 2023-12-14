#pragma once
#include "StageBase.h"
class Stage1_4 : public StageBase
{
public:
	Stage1_4(StageManager& mgr, const Size& windowSize, float fieldSize);
	~Stage1_4();

	void Init() override;
	void StartCheck() override;
	void ChangeStage(Input& input) override;

private:
	void CheckStageConditions() override;
	void DrawStageConditions(int drawY) override;
	void DrawArrow() const override;
	void CreateEnemy() override;

	void UpdateTime() override;

private:
	// �G��������
	int m_createNormalFrame;
	int m_createCreateFrame;

	// �X�e�[�W�ɓ��������_�ł̃N���A����ێ�
	bool m_isRightClear;
	bool m_isUpClear;
};

