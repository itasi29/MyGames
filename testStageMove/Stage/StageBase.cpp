#include <DxLib.h>
#include "StageBase.h"

StageBase::StageBase(StageManager& mgr) :
	m_mgr(mgr)
{
	handle = LoadGraph("field.png");
}

StageBase::~StageBase()
{
	DeleteGraph(handle);
}

void StageBase::Update()
{
}

void StageBase::Draw() const
{
	DrawRotaGraph(640, 360, 1.0, 0.0, handle, true);

	DrawString(640, 360, m_name.c_str(), 0xff00ff);
}
