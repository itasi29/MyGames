#include "SoundFile.h"
#include "FileManager.h"
#include <DxLib.h>

SoundFile::SoundFile(FileManager& manager) :
	File(manager)
{
}

SoundFile::~SoundFile()
{
	if (m_isEternal) return;

	m_manager.Delete(m_path);
}

void SoundFile::Delete()
{
	DeleteSoundMem(m_handle);
}
