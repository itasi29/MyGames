#include "FileImage.h"
#include <DxLib.h>

FileImage::FileImage()
{
}

FileImage::~FileImage()
{
}

void FileImage::Delete()
{
	DeleteGraph(m_handle);
}
