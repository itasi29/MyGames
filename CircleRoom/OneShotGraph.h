#pragma once
#include <string>

/// <summary>
/// 説明用等画像を出すためのクラス
/// </summary>
class OneShotGraph
{
public:
	OneShotGraph(const std::wstring& path);

private:
	int m_handle;
};

