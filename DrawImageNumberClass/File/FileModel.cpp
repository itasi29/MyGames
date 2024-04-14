#include "FileModel.h"

#include <DxLib.h>

FileModel::FileModel(FileManager& mgr) :
	FileBase(mgr)
{
}

FileModel::~FileModel()
{
}

int FileModel::GetHandle() const
{
	return MV1DuplicateModel(m_handle);
}

void FileModel::Delete()
{
	MV1DeleteModel(m_handle);
}
