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
/// 数字を画像として描画するクラス
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
	/// 整数のみを描画
	/// </summary>
	/// <param name="posX">X座標</param>
	/// <param name="posY">Y座標</param>
	/// <param name="integer">整数</param>
	/// <param name="scale">サイズ</param>
	/// <param name="drawPos">描画基準位置 def:左上</param>
	void Draw(int posX, int posY, int integer, double scale = 1.0f, const DrawPos& drawPos = DrawPos::LeftUp) const;
	/// <summary>
	/// 実数を描画
	/// </summary>
	/// <param name="posX">X座標</param>
	/// <param name="posY">Y座標</param>
	/// <param name="integer">整数</param>
	/// <param name="decimal">小数</param>
	/// <param name="scale">サイズ</param>
	/// <param name="drawPos">描画基準位置 def:左上</param>
	void Draw(int posX, int posY, int integer, int decimal, double scale = 1.0f, const DrawPos& drawPos = DrawPos::LeftUp) const;

	/// <summary>
	/// 明示的なファイルの消去
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

