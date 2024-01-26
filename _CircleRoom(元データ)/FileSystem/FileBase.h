#pragma once
#include <string>

// �}�l�[�W�����Ăׂ�悤��
class FileManager;
class GameManager;

/// <summary>
/// �t�@�C���̊��N���X
/// </summary>
class FileBase
{
	friend FileManager;

public:
	FileBase(FileManager& manager);
	virtual ~FileBase();

	int GetHandle() const;

protected:
	FileManager& m_mgr;
	int m_handle;	// �n���h��
	int m_count;	// �Q�ƃJ�E���^(���L�Ґ�)
	bool m_isEternal;	// �풓�t���O(�Ō�܂ŏ����Ȃ�)
	std::wstring m_path;	// �}�b�v�̎w��̂��߂Ɏ����Ă���

protected:
	virtual void Delete() = 0;
};

