#pragma once
#include "Vec3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

namespace Math
{
	constexpr double kPi = 3.14159265358979323846;
	constexpr float kPiF = 3.14159265358979323846f;

	constexpr float kDeg2Rad = kPiF / 180.0f;
	constexpr float kRad2Deg = 180.0f / kPiF;
}

// 内積
float Dot(const Vec3& item1, const Vec3& item2);
// 外積
Vec3 Cross(const Vec3& item1, const Vec3& item2);

// 平行移動
Matrix4x4 Move(const Vec3& move);
Matrix4x4 Move(float x, float y, float z);
// 拡大
Matrix4x4 Scale(const Vec3& scale);
Matrix4x4 Scale(float x, float y, float z);
// 回転(度数法)
Quaternion AngleAxis(float angle, const Vec3 axis);

// イージング
class Easing
{
private:
	// クラスの作成はさせない
	Easing();
public:
	// 等速
	static Vec3 Linear(const Vec3& start, const Vec3& end, float t);
	// 加速
	static Vec3 EaseIn(const Vec3& start, const Vec3& end, float t);
	// 減速
	static Vec3 EaseOut(const Vec3& start, const Vec3& end, float t);
	// 加速して減速
	static Vec3 EaseInOut(const Vec3& start, const Vec3& end, float t);
};