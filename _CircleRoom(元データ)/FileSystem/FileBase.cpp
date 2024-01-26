#include "FileBase.h"

FileBase::FileBase(FileManager& manager) :
	m_mgr(manager)
{
}

FileBase::~FileBase()
{
	// ‰½‚à‚µ‚È‚¢
}

int FileBase::GetHandle() const
{
	return m_handle;
}
