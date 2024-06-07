#include "FileVertexShader.h"
#include <DxLib.h>
#include "FileManager.h"

FileVertexShader::FileVertexShader()
{
}

FileVertexShader::~FileVertexShader()
{
}

void FileVertexShader::Delete()
{
	DeleteShader(m_handle);
}
