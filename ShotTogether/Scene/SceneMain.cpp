#include "SceneMain.h"

SceneMain::SceneMain(SceneManager& scnMgr, FileManager& fileMgr, SoundSystem& sndSys, Camera& camera) :
	SceneBase(scnMgr, fileMgr, sndSys, camera)
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
}

void SceneMain::Update(Input& input)
{
}

void SceneMain::Draw() const
{
}

void SceneMain::End()
{
}

std::shared_ptr<SceneBase> SceneMain::Change()
{
	return std::shared_ptr<SceneBase>();
}
