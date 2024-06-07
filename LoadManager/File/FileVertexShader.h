#pragma once
#include "FileBase.h"

/// <summary>
/// 頂点シェーダー用ファイルクラス
/// </summary>
class FileVertexShader : public FileBase
{
public:
	FileVertexShader();
	virtual ~FileVertexShader();

private:
	void Delete() override;
};

