#pragma once
#include <string>
#include "DxLib.h"
#include "StageManager.h"

class StageManager;

class StageBase
{
public:
	StageBase(StageManager& mgr);
	virtual ~StageBase();

	virtual void Update();
	void Draw() const;

	std::string GetName() const { return m_name; }

protected:
	StageManager& m_mgr;
	std::string m_name;

	int handle;
};

