#pragma once
#include <memory>

class FileBase;
class FileManager;

enum class DrawPos
{
	LeftUp,
	LeftDown,
	RightUp,
	RightDown
};

/// <summary>
/// �������摜�Ƃ��ĕ`�悷��N���X
/// </summary>
class ImageNumber
{
private:
	ImageNumber(FileManager& mgr);

	ImageNumber(const ImageNumber& app) = delete;
	void operator= (const ImageNumber& app) = delete;

public:
	static ImageNumber& GetInstance(FileManager& mgr);

	/// <summary>
	/// �����݂̂�`��
	/// </summary>
	/// <param name="posX">X���W</param>
	/// <param name="posY">Y���W</param>
	/// <param name="integer">����</param>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="drawPos">�`���ʒu def:����</param>
	void Draw(int posX, int posY, int integer, double scale = 1.0f, const DrawPos& drawPos = DrawPos::LeftUp) const;
	/// <summary>
	/// ������`��
	/// </summary>
	/// <param name="posX">X���W</param>
	/// <param name="posY">Y���W</param>
	/// <param name="integer">����</param>
	/// <param name="decimal">����</param>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="drawPos">�`���ʒu def:����</param>
	void Draw(int posX, int posY, int integer, int decimal, double scale = 1.0f, const DrawPos& drawPos = DrawPos::LeftUp) const;

	/// <summary>
	/// �����I�ȃt�@�C���̏���
	/// </summary>
	void End();

private:
	void InitPos(int& posX, int& posY, double scale, const  DrawPos& drawPos) const;

	int LeftBasic(int x, int y, int num, double scale) const;
	int RightBasic(int x, int y, int num, double scale) const;

	void DrawNum(int x, int y, int num, double scale) const;

private:
	std::shared_ptr<FileBase> m_file;
};

