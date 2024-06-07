#pragma once
#include "FileBase.h"

/// <summary>
/// ピクセルシェーダー用ファイルクラス
/// </summary>
class FilePixelShader : public FileBase
{
public:
	FilePixelShader();
	virtual ~FilePixelShader();

private:
	void Delete() override;
};

