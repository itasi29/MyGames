#pragma once
#include "StageBase.h"

class Stage1_2 : public StageBase
{
public:
	Stage1_2(GameManager& mgr, float fieldSize);
	~Stage1_2();

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
	void CreateLage();
	void UpdateTime() override;

private:
	// �G(Lage)�����t���[��
	int m_createLageFrame;
	// �G(Normal)�����t���[��
	int m_createNormalFrame;

	// �X�e�[�W�ɓ��������_�ł̃N���A����ێ�
	bool m_isRightClear;
};
