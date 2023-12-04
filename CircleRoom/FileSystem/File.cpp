#include "File.h"

File::File(FileManager& manager) :
	m_manager(manager)
{
}

File::~File()
{
	// ‰½‚à‚µ‚È‚¢
}

int File::GetHandle() const
{
	return m_handle;
}
