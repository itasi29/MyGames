#pragma once
#include <array>
#include <memory>

class FileBase;

enum Explanation
{
	kOperation,	// �������
	kClear,		// �N���A���@
	kPlay,		// ���v���C
	kClearAnother,	// �N���A�����(�X�e�[�W�ړ��ƃ`���[�g���A���I��)
	kEnd,		// �����I��
};

#include "StageBase.h"
class StageTutorial : public StageBase
{
public:
	StageTutorial(GameManager& mgr);
	~StageTutorial();

	void Init() override;
	void StartCheck() override {}
	void ChangeStage(Input& input) override;

private:
	void UpdateSelect(Input& input);
	void UpdatePlaying(Input& input);


	void CheckStageConditions() override;
	int DrawStageConditions(int drawY) override;
	void DrawArrow() const override;
	void DrawKilledEnemyType() const override {}
	void CreateEnemy() override {}

	void UpdateTime() override;

private:
	UpdateFunc_t m_updateFunc;

	std::array<std::shared_ptr<FileBase>, kEnd> m_handle;

	Explanation m_explanation;
};

