#pragma once
#include "FileBase.h"

/// <summary>
/// ���_�V�F�[�_�[�p�t�@�C���N���X
/// </summary>
class FileVertexShader : public FileBase
{
public:
	FileVertexShader();
	virtual ~FileVertexShader();

private:
	void Delete() override;
};

