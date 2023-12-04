#pragma once
#include <string>

// �}�l�[�W�����Ăׂ�悤��
class FileManager;

/// <summary>
/// �t�@�C���̊��N���X
/// </summary>
class File
{
	friend FileManager;

public:
	File(FileManager& manager);
	virtual ~File();

	int GetHandle() const;

protected:
	FileManager& m_manager;
	int m_handle;	// �n���h��
	int m_count;	// �Q�ƃJ�E���^(���L�Ґ�)
	bool m_isEternal;	// �풓�t���O(�Ō�܂ŏ����Ȃ�)
	std::wstring m_path;	// �}�b�v�̎w��̂��߂Ɏ����Ă���

protected:
	virtual void Delete() = 0;
};

