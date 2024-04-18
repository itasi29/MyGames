#include "Object.h"
#include <DxLib.h>
#include <cassert>
#include "ObjectManager.h"

Object::Object(int handle, ObjectManager& objMgr, FileManager& fileMgr, const wchar_t* const name) :
	m_isExist(true)
{
	m_handle = MV1DuplicateModel(handle);
	assert(m_handle > 0);

	const auto& size = objMgr.GetObjData(name);
	// ƒ‚ƒfƒ‹‚Ì‘å‚«‚³
	MV1SetScale(m_handle, VGet(size.model.w, size.model.h, size.model.d));
	// “–‚½‚è”»’è
	m_rect.SetRange(size.col.w, size.col.h, size.col.d);
}

Object::~Object()
{
}

void Object::Init(const Pos3& pos)
{
	m_pos = pos;
	m_rect.SetPos(m_pos);
}

void Object::Draw() const
{
	MV1DrawModel(m_handle);
}
