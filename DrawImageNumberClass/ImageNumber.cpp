#include "ImageNumber.h"
#include <DxLib.h>
#include <list>
#include "File/FileManager.h"
#include "File/FileBase.h"

namespace
{
	// 列数
	constexpr int kLine = 4;

	// チップサイズ
	constexpr int kChipSize = 32;

	// "."の切り取り位置
	constexpr int kDotRectX = kChipSize * 2;
	constexpr int kDotRectY = kChipSize * 2;
}

ImageNumber::ImageNumber(FileManager& mgr)
{
	m_file = mgr.Load(L"Number", true);
}

ImageNumber& ImageNumber::GetInstance(FileManager& mgr)
{
	static ImageNumber instance(mgr);
	return instance;
}

void ImageNumber::Draw(int posX, int posY, int inteager, double scale, const DrawPos& drawPos) const
{
	InitPos(posX, posY, scale, drawPos);

	if (drawPos == DrawPos::LeftDown || drawPos == DrawPos::LeftUp) {
		LeftBasic(posX, posY, inteager, scale);
	}
	else {
		RightBasic(posX, posY, inteager, scale);
	}
}

void ImageNumber::Draw(int posX, int posY, int inteager, int decimal, double scale, const DrawPos& drawPos) const
{
	InitPos(posX, posY, scale, drawPos);

	if (drawPos == DrawPos::LeftDown || drawPos == DrawPos::LeftUp) {
		// 実数部
		posX = LeftBasic(posX, posY, inteager, scale);
		// .部
		DrawRectRotaGraph(posX, posY, kDotRectX, kDotRectY, kChipSize, kChipSize,
			scale, 0.0, m_file->GetHandle(), true);
		posX += static_cast<int>(kChipSize * scale);
		// 小数部
		LeftBasic(posX, posY, decimal, scale);
	}
	else {
		posX = RightBasic(posX, posY, inteager, scale);
		DrawRectRotaGraph(posX, posY, kDotRectX, kDotRectY, kChipSize, kChipSize,
			scale, 0.0, m_file->GetHandle(), true);
		posX -= static_cast<int>(kChipSize * scale);
		RightBasic(posX, posY, decimal, scale);
	}
}

void ImageNumber::End()
{
	m_file.reset();
}

/// <summary>
/// 初期の位置を中心基準から各端に調整
/// </summary>
/// <param name="posX">X座標</param>
/// <param name="posY">Y座標</param>
/// <param name="scale">スケール</param>
/// <param name="drawPos">描画基準位置</param>
void ImageNumber::InitPos(int& posX, int& posY, double scale, const  DrawPos& drawPos) const
{
	if (drawPos == DrawPos::LeftDown || drawPos == DrawPos::RightDown) {
		posY -= static_cast<int>(kChipSize * scale * 0.5);
	}
	else {
		posY += static_cast<int>(kChipSize * scale * 0.5);
	}

	if (drawPos == DrawPos::LeftDown || drawPos == DrawPos::LeftUp) {
		posX += static_cast<int>(kChipSize * scale * 0.5);
	}
	else {
		posX -= static_cast<int>(kChipSize * scale * 0.5);
	}
}

/// <summary>
/// 渡された数値を左から描画していく
/// </summary>
/// <param name="posX">X座標</param>
/// <param name="posY">Y座標</param>
/// <param name="value">値</param>
/// <param name="scale">スケール</param>
/// <returns>X座標の位置</returns>
int ImageNumber::LeftBasic(int posX, int posY, int value, double scale) const
{
	int chipSize = static_cast<int>(kChipSize * scale);
	std::list<int> nums;
	while (true) {
		nums.push_front(value % 10);
		// 数値更新
		value /= 10;
		if (value == 0) {
			break;
		}
	}

	for (const auto& no : nums) {
		DrawNum(posX, posY, no, scale);
		posX += chipSize;
	}

	return posX;
}

/// <summary>
/// 渡された数値を右から描画していく
/// </summary>
/// <param name="posX">X座標</param>
/// <param name="posY">Y座標</param>
/// <param name="value">値</param>
/// <param name="scale">スケール</param>
/// <returns>X座標の位置</returns>
int ImageNumber::RightBasic(int posX, int posY, int value, double scale) const
{
	int chipSize = static_cast<int>(kChipSize * scale);
	std::list<int> nums;
	while (true) {
		nums.push_back(value % 10);
		// 数値更新
		value /= 10;
		if (value == 0) {
			break;
		}
	}

	for (const auto& no : nums) {
		DrawNum(posX, posY, no, scale);
		posX -= chipSize;
	}

	return posX;
}

/// <summary>
/// 値に合わせて描画する数字を変更
/// </summary>
/// <param name="x">X座標</param>
/// <param name="y">Y座標</param>
/// <param name="no">値</param>
/// <param name="scale">スケール</param>
void ImageNumber::DrawNum(int x, int y, int no, double scale) const
{
	// 切り取り位置
	int rcX, rcY;
	if (no == 0) {
		rcX = 0;
		rcY = 0;
	}
	else {
		rcX = no % kLine;
		rcY = no / kLine;
	}

	// 描画f
	DrawRectRotaGraph(x, y, rcX * kChipSize, rcY * kChipSize, kChipSize, kChipSize,
		scale, 0.0, m_file->GetHandle(), true);
}