#include "Geometry.h"
#include <cmath>

using namespace MyEngine;

float MyEngine::Dot(const Vec3& item1, const Vec3& item2)
{
	return item1.x * item2.x + item1.y * item2.y + item1.z * item2.z;
}

Vec3 MyEngine::Cross(const Vec3& item1, const Vec3& item2)
{
	Vec3 result;

	result.x = item1.y * item2.z - item1.z * item2.y;
	result.y = item1.z * item2.x - item1.x * item2.z;
	result.z = item1.x * item2.y - item1.y * item2.x;

	return result;
}

Vec3 MyEngine::Projection(const Vec3& projection, const Vec3& base)
{
	auto projectionN = projection.GetNormalized();
	return projectionN * Dot(base, projectionN);
}

Vec3 MyEngine::GetNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end)
{
	Vec3 startToEnd = end - start;
	Vec3 startToPoint = point - start;

	// üã‚Ì‚Ç‚Ì•Ó‚©
	float t = Dot(startToEnd, startToPoint) / startToEnd.SqLength();
	// ”r‘¼ˆ—
	t = std::fmax(std::fmin(t, 1.0f), 0.0f);

	return start + startToEnd * t;
}

bool MyEngine::IsNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end)
{
	Vec3 startToEnd = end - start;
	Vec3 startToPoint = point - start;

	// üã‚Ì‚Ç‚Ì•Ó‚©
	float t = Dot(startToEnd, startToPoint) / startToEnd.SqLength();

	// t‚ªü•ªã‚È‚çtrue
	return 0.0f <= t && t <= 1.0f;
}

Matrix4x4 MyEngine::Move(const Vec3& move)
{
	return Move(move.x, move.y, move.z);
}

Matrix4x4 MyEngine::Move(float x, float y, float z)
{
	Matrix4x4 result;
	result.Identity();
	
	result.m[3][0] = x;
	result.m[3][1] = y;
	result.m[3][2] = z;

	return result;
}

Matrix4x4 MyEngine::Scale(const Vec3& scale)
{
	return Scale(scale.x, scale.y, scale.z);
}

Matrix4x4 MyEngine::Scale(float x, float y, float z)
{
	Matrix4x4 result;
	result.Identity();

	result.m[0][0] = x;
	result.m[1][1] = y;
	result.m[2][2] = z;

	return result;
}

Quaternion MyEngine::AngleAxis(float angle, const Vec3& axis)
{
	Quaternion result;

	float halfRad = angle * Math::kDeg2Rad * 0.5f;
	float sin = std::sin(halfRad);
	float cos = std::cos(halfRad);
	auto normAxis = axis.GetNormalized();

	result = Quaternion(normAxis.x * sin, normAxis.y * sin, normAxis.z * sin, cos);

	return result;
}

Vec3 Easing::Linear(const Vec3& start, const Vec3& end, float t)
{
	return start + (end - start) * t;
}

Vec3 MyEngine::Easing::EaseIn(const Vec3& start, const Vec3& end, float t, float effect)
{
	float rate = std::powf(t, effect);
	return start + (end - start) * rate;
}

Vec3 MyEngine::Easing::EaseOut(const Vec3& start, const Vec3& end, float t, float effect)
{
	float rate = 1.0f - std::powf(1.0f - t, effect);
	return (end - start) * rate;
}

Vec3 Easing::EaseInOut(const Vec3& start, const Vec3& end, float t, float effect)
{
	float rate;
	if (t < 0.5f)
	{
		rate = 2 * (effect - 1.0f) * std::powf(t, effect);
	}
	else
	{
		rate = 1.0f - std::powf(-2 * t + 2, effect) * 0.5f;
	}
	return start + (end - start) * rate;
}
