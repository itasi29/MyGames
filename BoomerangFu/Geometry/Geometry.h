#pragma once
#include "Vec3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "Easing.h"

// 最近接点
Vec3 GetNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end);
// 線分上に最近接点があるかの判定
bool IsNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end);
