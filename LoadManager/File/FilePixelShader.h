#pragma once
#include "FileBase.h"

/// <summary>
/// �s�N�Z���V�F�[�_�[�p�t�@�C���N���X
/// </summary>
class FilePixelShader : public FileBase
{
public:
	FilePixelShader();
	virtual ~FilePixelShader();

private:
	void Delete() override;
};

