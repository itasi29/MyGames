#pragma once
#include "Vec3.h"

// 等速
Vec3 Lerp(const Vec3& start, const Vec3& end, float t);
// 加速
Vec3 EaseIn(const Vec3& start, const Vec3& end, float t, float effect = 2.0f);
// 減速
Vec3 EaseOut(const Vec3& start, const Vec3& end, float t, float effect = 2.0f);
// 加速して減速
Vec3 EaseInOut(const Vec3& start, const Vec3& end, float t, float effect = 2.0f);
