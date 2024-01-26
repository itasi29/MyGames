#pragma once
#include <string>

// マネージャを呼べるように
class FileManager;
class GameManager;

/// <summary>
/// ファイルの基底クラス
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
	int m_handle;	// ハンドル
	int m_count;	// 参照カウンタ(所有者数)
	bool m_isEternal;	// 常駐フラグ(最後まで消えない)
	std::wstring m_path;	// マップの指定のために持っておく

protected:
	virtual void Delete() = 0;
};

