#pragma once
#include "StageBase.h"

class Stage1_1 : public StageBase
{
public:
	Stage1_1(GameManager& mgr, Input& input);
	~Stage1_1();

	void ChangeStage(Input& input) override;

private:

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
