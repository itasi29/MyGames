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
	void CheckStageConditions(int timeFrame) override;

	int DrawStageConditions(int drawY) const override;
	void DrawArrow() const override;
	void DrawEnemyKilledInfo(int x, int y) const override;

	void CreateEnemy() override;
	void UpdateTime() override;

private:
	// ������
	int m_createDashNum;
	int m_createEneCreateNum;
	int m_createLargeNum;

	// �G��������
	int m_createDashFrame;
	int m_createEneCreateFrame;
	int m_createDivisionFrame;
	int m_createLargeFrame;

	// �X�e�[�W�ɓ��������_�ł̃N���A����ێ�
	bool m_isUpClear;
};
