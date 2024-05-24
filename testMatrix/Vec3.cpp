#include "Vec3.h"
#include <cmath>
#include <cassert>

Vec3::Vec3() :
    x(0.0f),
    y(0.0f),
    z(0.0f)
{
}

Vec3::Vec3(float inX, float inY, float inZ) :
    x(inX),
    y(inY),
    z(inZ)
{
}

Vec3 Vec3::operator+(const Vec3& val) const
{
	return Vec3(x + val.x, y + val.y, z + val.z);
}

void Vec3::operator+=(const Vec3& val)
{
	x += val.x;
	y += val.y;
	z += val.z;
}

Vec3 Vec3::operator-(const Vec3& val) const
{
	return Vec3(x - val.x, y - val.y, z - val.z);
}

void Vec3::operator-=(const Vec3& val)
{
	x -= val.x;
	y -= val.y;
	z -= val.z;
}

Vec3 Vec3::operator*(float scale) const
{
	return Vec3(x * scale, y * scale, z * scale);
}

void Vec3::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

Vec3 Vec3::operator/(float scale) const
{
	if (SqLength() == 0.0f) 
	{
		assert(false);
		return Vec3();
	}

	return Vec3(x / scale, y / scale, z / scale);
}

void Vec3::operator/=(float scale)
{
	if (SqLength() > 0.0f) 
	{
		x /= scale;
		y /= scale;
		z /= scale;
	}
	else
	{
		assert(false);
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
}

bool Vec3::operator==(const Vec3& val) const
{
	bool isX = x == val.x;
	bool isY = y == val.y;
	bool isZ = z == val.z;
	return isX && isY && isZ;
}

bool Vec3::operator!=(const Vec3& val) const
{
	bool isX = x != val.x;
	bool isY = y != val.y;
	bool isZ = z != val.z;
	return isX || isY || isZ;
}

float Vec3::Length() const
{
	return std::hypot(x, y, z);
}

float Vec3::SqLength() const
{
	return x * x + y * y + z * z;
}

Vec3 Vec3::GetNormalized() const
{
	Vec3 ref(0.0f, 0.0f, 0.0f);
	float len = Length();

	if (len > 0.0f)
	{
		ref = *this / len;
	}

    return ref;
}

void Vec3::Normalize()
{
	float len = Length();

	if (len > 0.0f)
	{
		*this /= len;
	}
}

VECTOR Vec3::GetVECTOR() const
{
    return VGet(x, y, z);
}
