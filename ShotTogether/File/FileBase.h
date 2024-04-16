#pragma once
#include <string>

class FileManager;

/// <summary>
/// �t�@�C���x�[�X
/// </summary>
class FileBase
{
friend FileManager;

public:
	FileBase(FileManager& mgr);
	virtual ~FileBase();

	virtual int GetHandle() const { return m_handle; }

private:
	virtual void Delete() abstract;
	void End() { m_isEnd = true; }

protected:
	FileManager& m_mgr;
	std::wstring m_name;
	int m_handle;
	int m_refCount;
	bool m_isEternal;
	bool m_isEnd;
};
