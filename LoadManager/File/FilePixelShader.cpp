#include "FilePixelShader.h"
#include <DxLib.h>

FilePixelShader::FilePixelShader()
{
}

FilePixelShader::~FilePixelShader()
{
}

void FilePixelShader::Delete()
{
	DeleteShader(m_handle);
}
