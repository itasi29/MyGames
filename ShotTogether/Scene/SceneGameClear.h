#pragma once
#include "SceneBase.h"
class SceneGameClear : public SceneBase
{
public:
	SceneGameClear(SceneManager& scnMgr, FileManager& fileMgr, SoundSystem& sndSys, Camera& camera);
	virtual ~SceneGameClear();

	virtual void Init();
	virtual void Update(Input& input);
	virtual void Draw() const;
	virtual void End();
	virtual std::shared_ptr<SceneBase> Change();
};

