#include "Geometry.h"
#include <cmath>

using namespace MyEngine;

Vec3 MyEngine::GetNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end)
{
	Vec3 startToEnd = end - start;
	Vec3 startToPoint = point - start;

	// ����̂ǂ̕ӂ�
	float t = Dot(startToEnd, startToPoint) / startToEnd.SqLength();
	// �r������
	t = std::fmax(std::fmin(t, 1.0f), 0.0f);

	return start + startToEnd * t;
}

bool MyEngine::IsNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end)
{
	Vec3 startToEnd = end - start;
	Vec3 startToPoint = point - start;

	// ����̂ǂ̕ӂ�
	float t = Dot(startToEnd, startToPoint) / startToEnd.SqLength();

	// t��������Ȃ�true
	return 0.0f <= t && t <= 1.0f;
}
