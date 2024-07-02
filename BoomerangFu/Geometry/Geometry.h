#pragma once
#include "Vec3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "Easing.h"

// Å‹ßÚ“_
Vec3 GetNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end);
// ü•ªã‚ÉÅ‹ßÚ“_‚ª‚ ‚é‚©‚Ì”»’è
bool IsNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end);
