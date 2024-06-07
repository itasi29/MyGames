#pragma once
#include <memory>

class SceneManager;

class SceneBase abstract
{
public:
	SceneBase();
	virtual ~SceneBase();

	virtual void Init() abstract;
	virtual void Final() abstract;

	virtual void Update() abstract;
	virtual void Draw() const abstract;

	virtual std::shared_ptr<SceneBase> Change() abstract;

protected:
	SceneManager& m_scnMgr;
};

