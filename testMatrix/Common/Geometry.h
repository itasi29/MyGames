#pragma once
#include "Vec3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

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
// 回転
// TODO:どう作ろう
Matrix4x4 Rotate(const Quaternion& item);

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