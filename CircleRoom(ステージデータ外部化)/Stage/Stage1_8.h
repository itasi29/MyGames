#pragma once
#include "StageBase.h"

class Stage1_8 : public StageBase
{
public:
	Stage1_8(GameManager& mgr, Input& input);
	~Stage1_8();

	void ChangeStage(Input& input) override;

private:
	std::shared_ptr<FileBase> m_explanation;

	// ステージに入った時点でのクリア情報を保持
	bool m_isDownClear;
	bool m_isRightClear;
};

