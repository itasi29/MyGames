#pragma once
#include "StageBase.h"
class Stage1_7 : public StageBase
{
public:
	Stage1_7(GameManager& mgr, Input& input);
	~Stage1_7();

	void ChangeStage(Input& input) override;

private:
	// 生成数
	int m_createNum;

	// 敵生成時間
	int m_createFrame;

	// ステージに入った時点でのクリア情報を保持
	bool m_isLeftClear;
	bool m_isDownClear;
};

