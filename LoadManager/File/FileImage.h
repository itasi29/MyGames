#pragma once
#include "FileBase.h"

/// <summary>
/// �摜�p�t�@�C���N���X
/// </summary>
class FileImage :  public FileBase
{
public:
	FileImage();
	virtual ~FileImage();

private:
	void Delete() override;
};

