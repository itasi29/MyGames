#pragma once
#include "FileBase.h"

/// <summary>
/// ���f���p�t�@�C���N���X
/// </summary>
class FileModel : public FileBase
{
public:
	FileModel();
	virtual ~FileModel();

	int GetHandle() const override;

private:
	void Delete() override;
};

