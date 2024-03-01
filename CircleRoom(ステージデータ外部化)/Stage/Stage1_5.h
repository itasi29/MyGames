#pragma once
#include "StageBase.h"
class Stage1_5 : public StageBase
{
public:
	Stage1_5(GameManager& mgr, Input& input);
	~Stage1_5();

	void ChangeStage(Input& input) override;

private:
	// 敵生成時間
	int m_createFrame;

	int m_createNum;

	// ステージに入った時点でのクリア情報を保持
	bool m_isDownClear;
};

