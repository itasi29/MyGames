#pragma once
#include "FileBase.h"


class FileGraph :  public FileBase
{
public:
	FileGraph(FileManager& mgr);
	virtual ~FileGraph();

private:
	void Delete() override;
};

