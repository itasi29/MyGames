#pragma once
#include <array>
#include <memory>

class FileBase;

enum Explanation
{
	kOperation,	// 操作説明
	kClear,		// クリア方法
	kPlay,		// 実プレイ
	kClearAnother,	// クリア後説明(ステージ移動とチュートリアル終了)
	kEnd,		// 説明終了
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

