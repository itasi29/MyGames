#pragma once
#include "SceneBase.h"

class SceneGameOver : public SceneBase
{
public:
	SceneGameOver(SceneManager& scnMgr, FileManager& fileMgr, SoundSystem& sndSys, Camera& camera);
	virtual ~SceneGameOver();

	virtual void Init();
	virtual void Update(Input& input);
	virtual void Draw() const;
	virtual void End();
	virtual std::shared_ptr<SceneBase> Change();
};

