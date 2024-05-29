#include "Geometry.h"
#include <cmath>

float Dot(const Vec3& item1, const Vec3& item2)
{
	return item1.x * item2.x + item1.y * item2.y + item1.z * item2.z;
}

Vec3 Cross(const Vec3& item1, const Vec3& item2)
{
	Vec3 result;

	result.x = item1.y * item2.z - item1.z * item2.y;
	result.y = item1.z * item2.x - item1.x * item2.z;
	result.z = item1.x * item2.y - item1.y * item2.x;

	return result;
}

Matrix4x4 Move(const Vec3& move)
{
	return Move(move.x, move.y, move.z);
}

Matrix4x4 Move(float x, float y, float z)
{
	Matrix4x4 result;
	result.Identity();

	result.m[0 + 3] = x;
	result.m[4 + 3] = y;
	result.m[8 + 3] = z;

	return result;
}

Matrix4x4 Scale(const Vec3& scale)
{
	return Scale(scale.x, scale.y, scale.z);
}

Matrix4x4 Scale(float x, float y, float z)
{
	Matrix4x4 result;
	result.Identity();

	result.m[0 + 0] = x;
	result.m[4 + 1] = y;
	result.m[8 + 2] = z;

	return result;
}

Vec3 Easing::Linear(const Vec3& start, const Vec3& end, float t)
{
	return (end - start) * t;
}

Vec3 Easing::EaseIn(const Vec3& start, const Vec3& end, float t)
{
	float rate = 1 - std::cosf(t * DX_PI_F * 0.5f);
	return (end - start) * rate;
}

Vec3 Easing::EaseOut(const Vec3& start, const Vec3& end, float t)
{
	return Vec3();
}

Vec3 Easing::EaseInOut(const Vec3& start, const Vec3& end, float t)
{
	return Vec3();
}
