#pragma once
#include "StageBase.h"
class Stage1_6 :  public StageBase
{
public:
	Stage1_6(GameManager& mgr, Input& input);
	~Stage1_6();

	void ChangeStage(Input& input) override;

private:
	// 生成数
	int m_createNum;

	// 敵生成時間
	int m_createNormalFrame1;
	int m_createNormalFrame2;
	int m_createEneCreateFrame;

	// ステージに入った時点でのクリア情報を保持
	bool m_isLeftClear;
	bool m_isUpClear;
	bool m_isDownClear;
};

