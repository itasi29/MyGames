#pragma once
#include "SceneBase.h"
class SceneDebug : public SceneBase
{
public:
	enum Kind : int
	{
		Title,
		Main,
		Max
	};

public:
	SceneDebug();
	~SceneDebug();

	void Init() override;
	void Final() override;

	void Update() override;
	void Draw() const override;

	std::shared_ptr<SceneBase> Change() override;

private:
	Kind m_kind;
};

