#include "ImageFile.h"
#include <DxLib.h>
#include "GameManager.h"

ImageFile::ImageFile(FileManager& manager) :
	FileBase(manager)
{
}

ImageFile::~ImageFile()
{
	if (m_isEternal) return;

	m_mgr.Delete(m_path);
}

void ImageFile::Delete()
{
	DeleteGraph(m_handle);
}
