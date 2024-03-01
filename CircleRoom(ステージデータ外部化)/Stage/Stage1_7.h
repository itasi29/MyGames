#pragma once
#include "StageBase.h"
class Stage1_7 : public StageBase
{
public:
	Stage1_7(GameManager& mgr, Input& input);
	~Stage1_7();

	void ChangeStage(Input& input) override;

private:
	// ������
	int m_createNum;

	// �G��������
	int m_createFrame;

	// �X�e�[�W�ɓ��������_�ł̃N���A����ێ�
	bool m_isLeftClear;
	bool m_isDownClear;
};

