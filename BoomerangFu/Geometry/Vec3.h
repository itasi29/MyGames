#pragma once
#include <DxLib.h>

/// <summary>
/// ベクター
/// </summary>
class Vec3
{
public:
	float x;
	float y;
	float z;

	Vec3();
	Vec3(float inX, float inY, float inZ);

	static Vec3 Up();

	/* 演算 */
	Vec3 operator+(const Vec3& val) const;
	void operator+=(const Vec3& val);
	Vec3 operator-(const Vec3& val) const;
	void operator-=(const Vec3& val);
	Vec3 operator*(float scale) const;
	void operator*=(float scale);
	Vec3 operator/(float scale) const;
	void operator/=(float scale);
	bool operator==(const Vec3& val) const;
	bool operator!=(const Vec3& val) const;

	// 長さ取得
	float Length() const;
	// 長さの2乗を取得
	float SqLength() const;
	// 正規化したベクトルを取得
	Vec3 GetNormalized() const;
	// 自身の正規化
	void Normalize();

	// DxLib用のVECTORを取得
	DxLib::VECTOR VGet() const;
};

// 内積
float Dot(const Vec3& item1, const Vec3& item2);
// 外積
Vec3 Cross(const Vec3& item1, const Vec3& item2);
// 射影
Vec3 Projection(const Vec3& projection, const Vec3& base);
