#pragma once
#include "FileBase.h"

/// <summary>
/// サウンド用ファイルクラス
/// </summary>
class FileSound : public FileBase
{
public:
	FileSound();
	virtual ~FileSound();

private:
	void Delete() override;
};

