#pragma once
#include "FileBase.h"

/// <summary>
/// モデル用ファイルクラス
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

