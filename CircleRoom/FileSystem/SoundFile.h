#pragma once
#include "FileBase.h"
/// <summary>
/// �T�E���h�n�̃t�@�C���̃n���h������
/// �폜������K�؂ɍs��
/// </summary>
class SoundFile : public FileBase
{
public:
	SoundFile(FileManager& manager);
	~SoundFile();

private:
	void Delete() override;
};

