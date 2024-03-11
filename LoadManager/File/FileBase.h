#pragma once
#include <string>

class FileManager;

/// <summary>
/// ファイルベース
/// </summary>
class FileBase
{
friend FileManager;

public:
	FileBase(FileManager& mgr);
	virtual ~FileBase();

	virtual int GetHandle() const { return m_handle; }
	void End() { m_isEnd = true; }

private:
	virtual void Delete() abstract;

protected:
	FileManager& m_mgr;
	int m_handle;

private:
	std::wstring m_name;
	int m_refCount;
	bool m_isEternal;
	bool m_isEnd;
};

