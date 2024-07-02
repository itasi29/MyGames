#pragma once
#include "Vec3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "Easing.h"

// �ŋߐړ_
Vec3 GetNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end);
// ������ɍŋߐړ_�����邩�̔���
bool IsNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end);
