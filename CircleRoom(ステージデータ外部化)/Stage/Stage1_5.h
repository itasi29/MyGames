#pragma once
#include "StageBase.h"
class Stage1_5 : public StageBase
{
public:
	Stage1_5(GameManager& mgr, Input& input);
	~Stage1_5();

	void ChangeStage(Input& input) override;

private:
	// �G��������
	int m_createFrame;

	int m_createNum;

	// �X�e�[�W�ɓ��������_�ł̃N���A����ێ�
	bool m_isDownClear;
};

