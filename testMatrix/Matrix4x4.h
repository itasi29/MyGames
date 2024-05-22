#pragma once

/// <summary>
/// 4x4s—ñ
/// </summary>
struct Matrix4x4
{
public:
	float m[4][4];

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
	void Inverse(bool isCorrect = false);

	void Scale(float x, float y, float z);
};

