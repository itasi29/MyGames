#pragma once
#include <string>
#include <memory>
#include <unordered_map>

class FileManager;

struct SrcData
{
	// カットする左上座標
	int x;
	int y;
	// 横続きになっているか
	bool isLink = false;
};

/// <summary>
/// 凡用Key
/// </summary>
class KeyFile
{
public:
	KeyFile(std::shared_ptr<FileManager>& mgr);
	~KeyFile();

	void DrawKey(std::wstring name, int drawX, int drawY, double rate = 1.0) const;

private:
private:
	int m_handle;

	// ボタン名　切り抜き範囲
	std::unordered_map<std::wstring, SrcData> m_key;
};

