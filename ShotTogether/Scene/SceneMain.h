#pragma once
#include "SceneBase.h"
class SceneMain : public SceneBase
{
public:
	SceneMain(SceneManager& scnMgr, FileManager& fileMgr, SoundSystem& sndSys, Camera& camera);
	virtual ~SceneMain();

	virtual void Init();
	virtual void Update(Input& input);
	virtual void Draw() const;
	virtual void End();
	virtual std::shared_ptr<SceneBase> Change();
};

