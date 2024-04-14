#include "ImageNumber.h"
#include <DxLib.h>
#include <list>
#include "File/FileManager.h"
#include "File/FileBase.h"

namespace
{
	// ��
	constexpr int kLine = 4;

	// �`�b�v�T�C�Y
	constexpr int kChipSize = 32;

	// "."�̐؂���ʒu
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
		// ������
		posX = LeftBasic(posX, posY, inteager, scale);
		// .��
		DrawRectRotaGraph(posX, posY, kDotRectX, kDotRectY, kChipSize, kChipSize,
			scale, 0.0, m_file->GetHandle(), true);
		posX += static_cast<int>(kChipSize * scale);
		// ������
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
/// �����̈ʒu�𒆐S�����e�[�ɒ���
/// </summary>
/// <param name="posX">X���W</param>
/// <param name="posY">Y���W</param>
/// <param name="scale">�X�P�[��</param>
/// <param name="drawPos">�`���ʒu</param>
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
/// �n���ꂽ���l��������`�悵�Ă���
/// </summary>
/// <param name="posX">X���W</param>
/// <param name="posY">Y���W</param>
/// <param name="value">�l</param>
/// <param name="scale">�X�P�[��</param>
/// <returns>X���W�̈ʒu</returns>
int ImageNumber::LeftBasic(int posX, int posY, int value, double scale) const
{
	int chipSize = static_cast<int>(kChipSize * scale);
	std::list<int> nums;
	while (true) {
		nums.push_front(value % 10);
		// ���l�X�V
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
/// �n���ꂽ���l���E����`�悵�Ă���
/// </summary>
/// <param name="posX">X���W</param>
/// <param name="posY">Y���W</param>
/// <param name="value">�l</param>
/// <param name="scale">�X�P�[��</param>
/// <returns>X���W�̈ʒu</returns>
int ImageNumber::RightBasic(int posX, int posY, int value, double scale) const
{
	int chipSize = static_cast<int>(kChipSize * scale);
	std::list<int> nums;
	while (true) {
		nums.push_back(value % 10);
		// ���l�X�V
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
/// �l�ɍ��킹�ĕ`�悷�鐔����ύX
/// </summary>
/// <param name="x">X���W</param>
/// <param name="y">Y���W</param>
/// <param name="no">�l</param>
/// <param name="scale">�X�P�[��</param>
void ImageNumber::DrawNum(int x, int y, int no, double scale) const
{
	// �؂���ʒu
	int rcX, rcY;
	if (no == 0) {
		rcX = 0;
		rcY = 0;
	}
	else {
		rcX = no % kLine;
		rcY = no / kLine;
	}

	// �`��f
	DrawRectRotaGraph(x, y, rcX * kChipSize, rcY * kChipSize, kChipSize, kChipSize,
		scale, 0.0, m_file->GetHandle(), true);
}