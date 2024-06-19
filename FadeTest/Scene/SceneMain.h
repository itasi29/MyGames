#pragma once
#include "SceneBase.h"
class SceneMain :
    public SceneBase
{
public:
    SceneMain();
    ~SceneMain();

	void Init() override;
	void Final() override;

	void Update() override;
	void Draw() const override;

	std::shared_ptr<SceneBase> Change() override;

private:
	int m_handle;
	int m_frame;
};

