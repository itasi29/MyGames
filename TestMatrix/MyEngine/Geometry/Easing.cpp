#include "Easing.h"
#include <cmath>

using namespace MyEngine;

Vec3 MyEngine::Lerp(const Vec3& start, const Vec3& end, float t)
{
    return start + (end - start) * t;
}

Vec3 MyEngine::EaseIn(const Vec3& start, const Vec3& end, float t, float effect)
{
    float rate = std::powf(t, effect);
    return start + (end - start) * rate;
}

Vec3 MyEngine::EaseOut(const Vec3& start, const Vec3& end, float t, float effect)
{
    float rate = 1.0f - std::powf(1.0f - t, effect);
    return (end - start) * rate;
}

Vec3 MyEngine::EaseInOut(const Vec3& start, const Vec3& end, float t, float effect)
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
