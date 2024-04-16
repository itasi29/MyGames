#include "SceneGameOver.h"

SceneGameOver::SceneGameOver(SceneManager& scnMgr, FileManager& fileMgr, SoundSystem& sndSys, Camera& camera) :
    SceneBase(scnMgr, fileMgr, sndSys, camera)
{
}

SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::Init()
{
}

void SceneGameOver::Update(Input& input)
{
}

void SceneGameOver::Draw() const
{
}

void SceneGameOver::End()
{
}

std::shared_ptr<SceneBase> SceneGameOver::Change()
{
    return std::shared_ptr<SceneBase>();
}
