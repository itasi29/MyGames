#pragma once
#include "StageBase.h"

class Stage1_1 : public StageBase
{
public:
	Stage1_1(StageManager& mgr, const Size& windowSize, float fieldSize);
	~Stage1_1();

	void Init() override;
	void StartCheck() override;
	void ChangeStage(Input& input) override;

private:
	void CheckStageConditions() override;
	void DrawStageConditions(int drawY) override;
	void DrawArrow() const override;
	void DrawKilledEnemyType() const override;
	void CreateEnemy() override;

	void CreateNormal();
	void UpdateTime() override;

private:
	// ������
	int m_createNum;

	// �G��������
	int m_createFrame;

	// �X�e�[�W�ɓ��������_�ł̃N���A����ێ�
	bool m_isLeftClear;
	bool m_isUpClear;
};

