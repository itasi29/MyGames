#pragma once
#include "File.h"
/// <summary>
/// �T�E���h�n�̃t�@�C���̃n���h������
/// �폜������K�؂ɍs��
/// </summary>
class SoundFile : public File
{
public:
	SoundFile(FileManager& manager);
	~SoundFile();

private:
	void Delete() override;
};

