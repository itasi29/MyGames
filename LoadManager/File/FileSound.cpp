#include "FileSound.h"

#include <DxLib.h>

FileSound::FileSound()
{
}

FileSound::~FileSound()
{
}

void FileSound::Delete()
{
	DeleteSoundMem(m_handle);
}
