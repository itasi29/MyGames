#pragma once
#include <memory>

class Input;
class StageBase;

/// <summary>
/// ステージの遷移をコントロール
/// </summary>
class StageManager
{
public:
	StageManager();
	~StageManager();

	void Update(Input& input);
	void Draw();

	void ChangeStage(std::shared_ptr<StageBase> nextStage);

private:
	std::shared_ptr<StageBase> m_stage;
};

