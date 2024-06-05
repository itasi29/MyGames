#pragma once
#include <DxLib.h>
#include "Vec3.h"

struct Quaternion;

/// <summary>
/// 4x4sρ
/// </summary>
struct Matrix4x4
{
public:
	float m[4][4];
	// s->ρ

	Matrix4x4();

	/* Z */
	Matrix4x4 operator+ (const Matrix4x4& mat) const;
	Matrix4x4 operator- (const Matrix4x4& mat) const;
	Matrix4x4 operator* (const Matrix4x4& mat) const;
	Matrix4x4 operator* (const Quaternion& q) const;
	Matrix4x4 operator/ (float div) const;

	// [
	void Zero();
	// PΚsρ
	void Identity();
	// ]usρ
	Matrix4x4 Transpose();
	// tsρ
	Matrix4x4 Inverse(bool isCorrect = false);

	// |WVΜέθ
	void SetPos(Vec3 pos);
	void SetRot(const Quaternion& q);
	Vec3 GetPos() const;

	// DxLibΟ·p
	MATRIX GetMATRIX() const;

private:
	float Dot(const Matrix4x4& mat, int line, int row) const;
};

