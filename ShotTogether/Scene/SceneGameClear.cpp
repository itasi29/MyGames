#include "SceneGameClear.h"

SceneGameClear::SceneGameClear(SceneManager& scnMgr, FileManager& fileMgr, SoundSystem& sndSys, Camera& camera) :
	SceneBase(scnMgr, fileMgr, sndSys, camera)
{
}

SceneGameClear::~SceneGameClear()
{
}

void SceneGameClear::Init()
{
}

void SceneGameClear::Update(Input& input)
{
}

void SceneGameClear::Draw() const
{
}

void SceneGameClear::End()
{
}

std::shared_ptr<SceneBase> SceneGameClear::Change()
{
	return std::shared_ptr<SceneBase>();
}
