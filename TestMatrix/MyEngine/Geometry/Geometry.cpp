#include "Geometry.h"
#include <cmath>

using namespace MyEngine;

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
