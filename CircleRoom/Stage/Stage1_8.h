#pragma once
#include "StageBase.h"
class Stage1_8 : public StageBase
{
public:
	Stage1_8(GameManager& mgr, Input& input);
	~Stage1_8();

	void Init() override;
	void StartCheck() override;
	void ChangeStage(Input& input) override;

private:
	void CheckStageConditions() override;
	int DrawStageConditions(int drawY) override;
	void DrawArrow() const override;
	void DrawKilledEnemyType() const override;

	void CreateEnemy() override;
	void UpdateTime() override;

private:
	// ������
	int m_createNum;

	// �G��������
	int m_createEneCreateFrame1;
	int m_createEneCreateFrame2;
	int m_createDashFrame;

	// �X�e�[�W�ɓ��������_�ł̃N���A����ێ�
	bool m_isLeftClear;
	bool m_isUpClear;
};

