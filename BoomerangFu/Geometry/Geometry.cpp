#include "Geometry.h"
#include <cmath>

Vec3 GetNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end)
{
	Vec3 startToEnd = end - start;
	Vec3 startToPoint = point - start;

	// 線上のどの辺か
	float t = Dot(startToEnd, startToPoint) / startToEnd.SqLength();
	// 排他処理
	t = std::fmax(std::fmin(t, 1.0f), 0.0f);

	return start + startToEnd * t;
}

bool IsNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end)
{
	Vec3 startToEnd = end - start;
	Vec3 startToPoint = point - start;

	// 線上のどの辺か
	float t = Dot(startToEnd, startToPoint) / startToEnd.SqLength();

	// tが線分上ならtrue
	return 0.0f <= t && t <= 1.0f;
}
