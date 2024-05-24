#pragma once
#include "Vec3.h"

/// <summary>
/// 4x4�s��
/// </summary>
struct Matrix4x4
{
public:
	float m[4 * 4];
	/* �v�f�̕��ѕ�
	0  1  2  3
	4  5  6  7
	8  9  10 11
	12 13 14 15	*/

	Matrix4x4();

	/* ���Z */
	Matrix4x4 operator+ (const Matrix4x4& mat) const;
	Matrix4x4 operator- (const Matrix4x4& mat) const;
	Matrix4x4 operator* (const Matrix4x4& mat) const;

	// �[��
	void Zero();
	// �P�ʍs��
	void Identity();
	// �t�s��
	Matrix4x4 Inverse(bool isCorrect = false);

	// Setter
	void SetLine(int lineNo, const Vec3 val);
	void SetRow(int rowNo, const Vec3 val);
	// Getter
	Vec3 GetLine(int lineNo) const;
	Vec3 GetRow(int rowNo) const;
};

