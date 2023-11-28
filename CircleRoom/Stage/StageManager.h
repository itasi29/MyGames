#pragma once
#include <memory>

class Input;
class StageBase;

/// <summary>
/// �X�e�[�W�̑J�ڂ��R���g���[��
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

