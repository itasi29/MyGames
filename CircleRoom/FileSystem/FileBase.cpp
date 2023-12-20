#include "FileBase.h"

FileBase::FileBase(FileManager& manager) :
	m_mgr(manager)
{
}

FileBase::~FileBase()
{
	// 何もしない
}

int FileBase::GetHandle() const
{
	return m_handle;
}
