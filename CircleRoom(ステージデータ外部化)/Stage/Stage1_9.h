#pragma once
#include "StageBase.h"

class Stage1_9 : public StageBase
{
public:
	Stage1_9(GameManager& mgr, Input& input);
	~Stage1_9();

	void ChangeStage(Input& input) override;

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
