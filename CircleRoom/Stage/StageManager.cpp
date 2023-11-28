#include "StageManager.h"
#include "StageBase.h"

StageManager::StageManager()
{
}

StageManager::~StageManager()
{
}

void StageManager::Update(Input& input)
{
	m_stage->Update(input);
	m_stage->ChangeStage(input);
}

void StageManager::Draw()
{
	m_stage->Draw();
}

void StageManager::ChangeStage(std::shared_ptr<StageBase> nextStage)
{
	m_stage = nextStage;
	m_stage->Init();
}
