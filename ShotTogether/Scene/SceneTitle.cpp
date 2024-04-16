#include "SceneTitle.h"

SceneTitle::SceneTitle(SceneManager& scnMgr, FileManager& fileMgr, SoundSystem& sndSys, Camera& camera) :
	SceneBase(scnMgr, fileMgr, sndSys, camera)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init() 
{
}

void SceneTitle::Update(Input& input)
{
}

void SceneTitle::Draw() const
{
}

void SceneTitle::End()
{
}

std::shared_ptr<SceneBase> SceneTitle::Change()
{
	return std::shared_ptr<SceneBase>();
}
