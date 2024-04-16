#pragma once
#include "SceneBase.h"

class SceneTitle : public SceneBase
{
public:
	SceneTitle(SceneManager& scnMgr, FileManager& fileMgr, SoundSystem& sndSys, Camera& camera);
	virtual ~SceneTitle();

	virtual void Init();
	virtual void Update(Input& input);
	virtual void Draw() const;
	virtual void End();
	virtual std::shared_ptr<SceneBase> Change();
};

