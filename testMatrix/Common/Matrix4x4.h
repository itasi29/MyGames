#pragma once
#include "Vec3.h"

/// <summary>
/// 4x4s—ñ
/// </summary>
struct Matrix4x4
{
public:
	float m[4 * 4];
	/* —v‘f‚Ì•À‚Ñ•û
	0  1  2  3
	4  5  6  7
	8  9  10 11
	12 13 14 15	*/

	Matrix4x4();

	/* ‰‰Z */
	Matrix4x4 operator+ (const Matrix4x4& mat) const;
	Matrix4x4 operator- (const Matrix4x4& mat) const;
	Matrix4x4 operator* (const Matrix4x4& mat) const;

	// ƒ[ƒ
	void Zero();
	// ’PˆÊs—ñ
	void Identity();
	// ‹ts—ñ
	Matrix4x4 Inverse(bool isCorrect = false);

private:
	float Dot(const Matrix4x4& mat, int line, int row) const;
};

