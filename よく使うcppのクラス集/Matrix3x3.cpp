#include "Matrix3x3.h"
#include <cmath>

Matrix4x4::Matrix4x4()
{
	Identity();
}

void Matrix4x4::Identity()
{
	m00 = 1.0f;
	m11 = 1.0f;
	m22 = 1.0f;
	m33 = 1.0f;

	m01 = 0.0f;
	m02 = 0.0f;
	m03 = 0.0f;
	m10 = 0.0f;
	m12 = 0.0f;
	m13 = 0.0f;
	m20 = 0.0f;
	m21 = 0.0f;
	m23 = 0.0f;
	m30 = 0.0f;
	m31 = 0.0f;
	m32 = 0.0f;
}

Matrix4x4 Matrix4x4::operator*(Matrix4x4 left) const
{
	Matrix4x4 temp;

	temp.m00 = m00 * left.m00 + m01 * left.m10 + m02 * left.m20 + m03 * left.m30;
	temp.m01 = m00 * left.m01 + m01 * left.m11 + m02 * left.m21 + m03 * left.m31;
	temp.m02 = m00 * left.m02 + m01 * left.m12 + m02 * left.m22 + m03 * left.m32;
	temp.m03 = m00 * left.m03 + m01 * left.m13 + m02 * left.m23 + m03 * left.m33;
	temp.m10 = m10 * left.m00 + m11 * left.m10 + m12 * left.m20 + m13 * left.m30;
	temp.m11 = m10 * left.m01 + m11 * left.m11 + m12 * left.m21 + m13 * left.m31;
	temp.m12 = m10 * left.m02 + m11 * left.m12 + m12 * left.m22 + m13 * left.m32;
	temp.m13 = m10 * left.m03 + m11 * left.m13 + m12 * left.m23 + m13 * left.m33;
	temp.m20 = m20 * left.m00 + m21 * left.m10 + m22 * left.m20 + m23 * left.m30;
	temp.m21 = m20 * left.m01 + m21 * left.m11 + m22 * left.m21 + m23 * left.m31;
	temp.m22 = m20 * left.m02 + m21 * left.m12 + m22 * left.m22 + m23 * left.m32;
	temp.m23 = m20 * left.m03 + m21 * left.m13 + m22 * left.m23 + m23 * left.m33;
	temp.m30 = m30 * left.m00 + m31 * left.m10 + m32 * left.m20 + m33 * left.m30;
	temp.m31 = m30 * left.m01 + m31 * left.m11 + m32 * left.m21 + m33 * left.m31;
	temp.m32 = m30 * left.m02 + m31 * left.m12 + m32 * left.m22 + m33 * left.m32;
	temp.m33 = m30 * left.m03 + m31 * left.m13 + m32 * left.m23 + m33 * left.m33;

	return temp;
}

Matrix4x4 Matrix4x4::operator*=(Matrix4x4 left)
{
	Matrix4x4 temp = *this;

	m00 = temp.m00 * left.m00 + temp.m01 * left.m10 + temp.m02 * left.m20 + temp.m03 * left.m30;
	m01 = temp.m00 * left.m01 + temp.m01 * left.m11 + temp.m02 * left.m21 + temp.m03 * left.m31;
	m02 = temp.m00 * left.m02 + temp.m01 * left.m12 + temp.m02 * left.m22 + temp.m03 * left.m32;
	m03 = temp.m00 * left.m03 + temp.m01 * left.m13 + temp.m02 * left.m23 + temp.m03 * left.m33;
	m10 = temp.m10 * left.m00 + temp.m11 * left.m10 + temp.m12 * left.m20 + temp.m13 * left.m30;
	m11 = temp.m10 * left.m01 + temp.m11 * left.m11 + temp.m12 * left.m21 + temp.m13 * left.m31;
	m12 = temp.m10 * left.m02 + temp.m11 * left.m12 + temp.m12 * left.m22 + temp.m13 * left.m32;
	m13 = temp.m10 * left.m03 + temp.m11 * left.m13 + temp.m12 * left.m23 + temp.m13 * left.m33;
	m20 = temp.m20 * left.m00 + temp.m21 * left.m10 + temp.m22 * left.m20 + temp.m23 * left.m30;
	m21 = temp.m20 * left.m01 + temp.m21 * left.m11 + temp.m22 * left.m21 + temp.m23 * left.m31;
	m22 = temp.m20 * left.m02 + temp.m21 * left.m12 + temp.m22 * left.m22 + temp.m23 * left.m32;
	m23 = temp.m20 * left.m03 + temp.m21 * left.m13 + temp.m22 * left.m23 + temp.m23 * left.m33;
	m30 = temp.m30 * left.m00 + temp.m31 * left.m10 + temp.m32 * left.m20 + temp.m33 * left.m30;
	m31 = temp.m30 * left.m01 + temp.m31 * left.m11 + temp.m32 * left.m21 + temp.m33 * left.m31;
	m32 = temp.m30 * left.m02 + temp.m31 * left.m12 + temp.m32 * left.m22 + temp.m33 * left.m32;
	m33 = temp.m30 * left.m03 + temp.m31 * left.m13 + temp.m32 * left.m23 + temp.m33 * left.m33;

	return *this;
}

Vec2 Matrix4x4::operator*(Vec2 left) const
{
	Vec2 temp;

	temp.x = m00 * left.x + m01 * left.y + m02 * 1.0f;
	temp.y = m10 * left.x + m11 * left.y + m12 * 1.0f;

	return temp;
}

Matrix4x4 Matrix4x4::AngleAxisZ(float rad)
{
	float cos = cosf(rad);
	float sin = sinf(rad);

	m00 = cos;
	m01 = -sin;
	m10 = sin;
	m11 = cos;

	return *this;
}
