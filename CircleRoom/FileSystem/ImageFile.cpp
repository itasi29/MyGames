#include "ImageFile.h"
#include <DxLib.h>
#include "FileManager.h"

ImageFile::ImageFile(FileManager& manager) :
	File(manager)
{
}

ImageFile::~ImageFile()
{
	if (m_isEternal) return;

	m_manager.Delete(m_path);
}

void ImageFile::Delete()
{
	DeleteGraph(m_handle);
}
