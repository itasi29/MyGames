#pragma once
#include "FileBase.h"

class FileModel : public FileBase
{
public:
	FileModel(FileManager& mgr);
	virtual ~FileModel();

	int GetHandle() const override;

private:
	void Delete() override;
};

