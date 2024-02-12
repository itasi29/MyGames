#pragma once
#include "StageBase.h"
class Stage1_6 :  public StageBase
{
public:
	Stage1_6(GameManager& mgr, Input& input);
	~Stage1_6();

	void Init() override;
	void StartCheck() override;
	void ChangeStage(Input& input) override;

private:
	void CheckStageConditions(int timeFrame) override;
	int DrawStageConditions(int drawY) const override;
	void DrawArrow() const override;
	void DrawKilledEnemyType(int x, int y) const override;

	void CreateEnemy() override;
	void UpdateTime() override;

private:
	// ������
	int m_createNum;

	// �G��������
	int m_createNormalFrame1;
	int m_createNormalFrame2;
	int m_createEneCreateFrame;

	// �X�e�[�W�ɓ��������_�ł̃N���A����ێ�
	bool m_isLeftClear;
	bool m_isUpClear;
	bool m_isDownClear;
};

