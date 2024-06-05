#pragma once
#include <DxLib.h>
#include "Vec3.h"

struct Quaternion;

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4
{
public:
	float m[4][4];
	// 行->列

	Matrix4x4();

	/* 演算 */
	Matrix4x4 operator+ (const Matrix4x4& mat) const;
	Matrix4x4 operator- (const Matrix4x4& mat) const;
	Matrix4x4 operator* (const Matrix4x4& mat) const;
	Matrix4x4 operator* (const Quaternion& q) const;
	Matrix4x4 operator/ (float div) const;

	// ゼロ
	void Zero();
	// 単位行列
	void Identity();
	// 転置行列
	Matrix4x4 Transpose();
	// 逆行列
	Matrix4x4 Inverse(bool isCorrect = false);

	// ポジションの設定
	void SetPos(Vec3 pos);
	void SetRot(const Quaternion& q);
	Vec3 GetPos() const;

	// DxLib変換用
	MATRIX GetMATRIX() const;

private:
	float Dot(const Matrix4x4& mat, int line, int row) const;
};

