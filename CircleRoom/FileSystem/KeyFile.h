#pragma once
#include <string>
#include <memory>
#include <unordered_map>

class FileManager;

struct SrcData
{
	// �J�b�g���鍶����W
	int x;
	int y;
	// �������ɂȂ��Ă��邩
	bool isLink = false;
};

/// <summary>
/// �}�pKey
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

	// �{�^�����@�؂蔲���͈�
	std::unordered_map<std::wstring, SrcData> m_key;
};

