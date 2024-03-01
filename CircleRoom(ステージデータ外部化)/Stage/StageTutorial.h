#pragma once
#include <array>
#include <memory>

class FileBase;

enum class Explanation
{
	kOperation,	// �������
	kClear,		// �N���A���@
	kPlay,		// ���v���C
	kEnd,		// �����I��
};

#include "StageBase.h"
class StageTutorial : public StageBase
{
public:
	StageTutorial(GameManager& mgr, Input& input);
	~StageTutorial();

	void ChangeStage(Input& input) override;

private:
	UpdateFunc_t m_updateFunc;

	int  m_index;
	std::array<std::shared_ptr<FileBase>, 5> m_handle;
	std::array<std::shared_ptr<FileBase>, 2> m_arrowImg;

	Explanation m_explanation;
	int m_createFrame;
	bool m_isStart;
	int m_emphasisFrame;
};

