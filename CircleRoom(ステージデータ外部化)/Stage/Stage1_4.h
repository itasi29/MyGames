#pragma once
#include "StageBase.h"
class Stage1_4 : public StageBase
{
public:
	Stage1_4(GameManager& mgr, Input& input);
	~Stage1_4();

	void ChangeStage(Input& input) override;

private:
	// �G��������
	int m_createNormalFrame;
	int m_createDashFrame;
	int m_createLargeFrame;

	// �X�e�[�W�ɓ��������_�ł̃N���A����ێ�
	bool m_isLeftClear;
	bool m_isRightClear;
	bool m_isUpClear;
	bool m_isDownClear;
};

