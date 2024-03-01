#pragma once
#include "StageBase.h"

class Stage1_1 : public StageBase
{
public:
	Stage1_1(GameManager& mgr, Input& input);
	~Stage1_1();

	void ChangeStage(Input& input) override;

private:
	// 生成数
	int m_createNum;

	// 敵生成時間
	int m_createFrame;

	// ステージに入った時点でのクリア情報を保持
	bool m_isLeftClear;
	bool m_isUpClear;
};
