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
	void UpTime() override;

private:
	void UniqueEndProcessing() override;

	void CheckStageConditions() override;
	int DrawStageConditions(int drawY) override;
	void DrawArrow() const override;
	void DrawKilledEnemyType() const override;

	void CreateEnemy() override;
	void CreateStrongBoss() override;
	void UpdateTime() override;

private:
	std::shared_ptr<FileBase> m_explanation;

	// �X�e�[�W�ɓ��������_�ł̃N���A����ێ�
	bool m_isDownClear;
	bool m_isRightClear;
};

