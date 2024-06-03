#pragma once
#include "Vec3.h"
#include "Matrix4x4.h"

/// <summary>
/// クオータニオン
/// </summary>
struct Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion();
	Quaternion(float x, float y, float z, float w);

	Quaternion Conjugated() const;

	Quaternion operator* (const Quaternion& val) const;
	Vec3 operator* (const Vec3& vec) const;
};

