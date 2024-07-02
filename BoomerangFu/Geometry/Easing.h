#pragma once
#include "Vec3.h"

// ����
Vec3 Lerp(const Vec3& start, const Vec3& end, float t);
// ����
Vec3 EaseIn(const Vec3& start, const Vec3& end, float t, float effect = 2.0f);
// ����
Vec3 EaseOut(const Vec3& start, const Vec3& end, float t, float effect = 2.0f);
// �������Č���
Vec3 EaseInOut(const Vec3& start, const Vec3& end, float t, float effect = 2.0f);
