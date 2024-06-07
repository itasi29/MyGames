#pragma once
#include "SceneBase.h"


class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	~SceneTitle();

	void Init() override;
	void Final() override;

	void Update() override;
	void Draw() const override;

	std::shared_ptr<SceneBase> Change() override;
};

