#pragma once

/// <summary>
/// 4x4�s��
/// </summary>
struct Matrix4x4
{
public:
	float m[4][4];

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
	void Inverse(bool isCorrect = false);

	void Scale(float x, float y, float z);
};

