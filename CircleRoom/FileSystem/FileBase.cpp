#include "FileBase.h"

FileBase::FileBase(FileManager& manager) :
	m_mgr(manager)
{
}

FileBase::~FileBase()
{
	// �������Ȃ�
}

int FileBase::GetHandle() const
{
	return m_handle;
}
