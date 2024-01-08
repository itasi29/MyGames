#pragma once
#include <string>
#include <unordered_map>

/// <summary>
/// 汎用Bottan
/// </summary>

class FileManager;

class BottansFile
{
public:
	BottansFile(FileManager& mgr);
	~BottansFile();

	/// <summary>
	/// ボタンの画像のみ描画
	/// </summary>
	/// <param name="name">Bottan名</param>
	/// <param name="drawX">左端</param>
	/// <param name="drawY">上端</param>
	void DrawBottan(std::wstring name, int drawX, int drawY) const;
	/// <summary>
	/// ボタンの画像＋文字描画
	/// </summary>
	/// <param name="name">Bottan名</param>
	/// <param name="drawGraphX">画像左端</param>
	/// <param name="drawGraphY">画像上端</param>
	/// <param name="drawStrX">文字左端</param>
	/// <param name="drawStrY">文字上端</param>
	void DrawBottanToString(std::wstring name, int drawGraphX, int drawGraphY, int drawStrX, int drawStrY) const;

private:
	int m_handle;

	// ボタン名　切り抜き範囲
	std::unordered_map<std::wstring, int> m_bottans;
};

