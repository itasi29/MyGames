#pragma once
#include "FileBase.h"

/// <summary>
/// �T�E���h�p�t�@�C���N���X
/// </summary>
class FileSound : public FileBase
{
public:
	FileSound();
	virtual ~FileSound();

private:
	void Delete() override;
};

