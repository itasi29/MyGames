#include "Matrix3x3.h"
#include <cmath>

Matrix3x3::Matrix3x3()
{
	Identity();
}

void Matrix3x3::Identity()
{
	m00 = 1.0f;
	m11 = 1.0f;
	m22 = 1.0f;

	m01 = 0.0f;
	m02 = 0.0f;
	m12 = 0.0f;
	m21 = 0.0f;
}

Matrix3x3 Matrix3x3::operator*(Matrix3x3 left) const
{
	Matrix3x3 temp;

	temp.m00 = m00 * left.m00 + m01 * left.m10 + m02 * left.m20;
	temp.m01 = m00 * left.m01 + m01 * left.m11 + m02 * left.m21;
	temp.m02 = m00 * left.m02 + m01 * left.m12 + m02 * left.m22;
	temp.m10 = m10 * left.m00 + m11 * left.m10 + m12 * left.m20;
	temp.m11 = m10 * left.m01 + m11 * left.m11 + m12 * left.m21;
	temp.m12 = m10 * left.m02 + m11 * left.m12 + m12 * left.m22;
	temp.m20 = m20 * left.m00 + m21 * left.m10 + m22 * left.m20;
	temp.m21 = m20 * left.m01 + m21 * left.m11 + m22 * left.m21;
	temp.m22 = m20 * left.m02 + m21 * left.m12 + m22 * left.m22;

	return temp;
}

Matrix3x3 Matrix3x3::operator*=(Matrix3x3 left)
{
	Matrix3x3 temp = *this;

	m00 = temp.m00 * left.m00 + temp.m01 * left.m10 + temp.m02 * left.m20;
	m01 = temp.m00 * left.m01 + temp.m01 * left.m11 + temp.m02 * left.m21;
	m02 = temp.m00 * left.m02 + temp.m01 * left.m12 + temp.m02 * left.m22;
	m10 = temp.m10 * left.m00 + temp.m11 * left.m10 + temp.m12 * left.m20;
	m11 = temp.m10 * left.m01 + temp.m11 * left.m11 + temp.m12 * left.m21;
	m12 = temp.m10 * left.m02 + temp.m11 * left.m12 + temp.m12 * left.m22;
	m20 = temp.m20 * left.m00 + temp.m21 * left.m10 + temp.m22 * left.m20;
	m21 = temp.m20 * left.m01 + temp.m21 * left.m11 + temp.m22 * left.m21;
	m22 = temp.m20 * left.m02 + temp.m21 * left.m12 + temp.m22 * left.m22;

	return *this;
}

Vec2 Matrix3x3::operator*(Vec2 left) const
{
	Vec2 temp;

	temp.x = m00 * left.x + m01 * left.y + m02 * 1.0f;
	temp.y = m10 * left.x + m11 * left.y + m12 * 1.0f;

	return temp;
}

Matrix3x3 Matrix3x3::Angle(float rad)
{
	float cos = cosf(rad);
	float sin = sinf(rad);

	m00 = cos;
	m01 = -sin;
	m10 = sin;
	m11 = cos;

	return *this;
}
