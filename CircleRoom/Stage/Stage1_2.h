#pragma once
#include "StageBase.h"

class Stage1_2 : public StageBase
{
public:
	Stage1_2(GameManager& mgr, Input& input);
	~Stage1_2();

	void Init() override;
	void StartCheck() override;
	void ChangeStage(Input& input) override;

private:
	void CheckStageConditions(int timeFrame) override;
	int DrawStageConditions(int drawY) override;
	void DrawArrow() const override;
	void DrawKilledEnemyType() const override;

	void CreateEnemy() override;
	void UpdateTime() override;

private:
	// �G(Lage)�����t���[��
	int m_createLageFrame;
	// �G(Normal)�����t���[��1
	int m_createNormalFrame1;
	// �G(Normal)�����t���[��2
	int m_createNormalFrame2;

	// ���񐶐���(Normal)
	int m_createNum;

	// �X�e�[�W�ɓ��������_�ł̃N���A����ێ�
	bool m_isRightClear;
	bool m_isUpClear;
};
