#pragma once
#include "FileBase.h"

/// <summary>
/// 画像用ファイルクラス
/// </summary>
class FileImage :  public FileBase
{
public:
	FileImage();
	virtual ~FileImage();

private:
	void Delete() override;
};

