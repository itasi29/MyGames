#include "Geometry.h"
#include <cassert>
#include <cmath>

Size::Size() :
	w(0.0f),
	h(0.0f),
	d(0.0f)
{
}

Size::Size(float inW, float inH, float inD) :
	w(inW),
	h(inH),
	d(inD)
{
}

Size Size::operator+(const Size& val) const
{
	return Size( w + val.w, h + val.h, d + val.d );
}

Size Size::operator*(float scale) const
{
	return Size( w * scale, h * scale, d * scale);
}

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

Vec3 Vec3::Reverse()
{
	return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator+(const Vec3& val) const
{
	return Vec3( x + val.x, y + val.y, z + val.z );
}

void Vec3::operator+=(const Vec3& val)
{
	x += val.x;
	y += val.y;
	z += val.z;
}

Vec3 Vec3::operator-(const Vec3& val) const
{
	return Vec3( x - val.x, y - val.y, z - val.z );
}

void Vec3::operator-=(const Vec3& val)
{
	x -= val.x;
	y -= val.y;
	z -= val.z;
}

Vec3 Vec3::operator*(float scale) const
{
	return Vec3( x * scale, y * scale, z * scale );
}

void Vec3::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

Vec3 Vec3::operator/(float scale) const
{
	if (SqLength() == 0.0f)	{
		assert(false);
		return Vec3();
	}

	return Vec3( x / scale, y / scale, z / scale );
}

void Vec3::operator/=(float scale)
{
	if (SqLength() == 0.0f) {
		assert(false);
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	x /= scale;
	y /= scale;
	z /= scale;
}

bool Vec3::operator==(const Vec3& val) const
{
	return x == val.x && y == val.y && z == val.z;
}

bool Vec3::operator!=(const Vec3& val) const
{
	return !(*this == val);
}

float Vec3::Length() const
{
	return std::hypot(x, y, z);
}

float Vec3::SqLength() const
{
	return x * x + y * y + z * z;
}

void Vec3::Normalize()
{
	float len = Length();
	if (len == 0.0f) {
		assert(false);
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		return;
	}

	*this /= len;
}

Vec3 Vec3::GetNormalized() const
{
	float len = Length();
	if (len == 0.0f) {
		return Vec3();
	}

	return Vec3( x / len, y / len, z / len );
}

Vec3 Lerp(const Vec3& start, const Vec3& end, float t)
{
	float x = start.x + t * (end.x - start.x);
	float y = start.y + t * (end.y - start.y);
	float z = start.z + t * (end.z - start.z);
	return Vec3(x, y, z);
}

float Dot(const Vec3& vec1, const Vec3& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

Vec3 Cross(const Vec3& vec1, const Vec3& vec2)
{
	Vec3 val;

	val.x = vec1.y * vec2.z - vec1.z * vec2.y;
	val.y = vec1.z * vec2.x - vec1.x * vec2.z;
	val.z = vec1.x * vec2.y - vec1.y * vec2.x;

	return val;
}
