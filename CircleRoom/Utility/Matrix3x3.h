#pragma once
#include "Vec2.h"

/// <summary>
/// 3*3の行列
/// m'行''列'と数字が対応している
/// </summary>
class Matrix3x3
{
public:
	Matrix3x3();

	/// <summary>
	/// 自信を単位行列にする
	/// </summary>
	/// <returns></returns>
	void Identity();

	/// <summary>
	/// 行列の掛け算
	/// </summary>
	/// <param name="left">3*3行列</param>
	/// <returns>掛け算後の3*3行列</returns>
	Matrix3x3 operator*(Matrix3x3 left) const;
	/// <summary>
	/// 行列の掛け算
	/// </summary>
	/// <param name="left">3*3行列</param>
	/// <returns>掛け算後の3*3行列</returns>
	Matrix3x3 operator*=(Matrix3x3 left);

	/// <summary>
	/// 行列とベクトルの掛け算
	/// </summary>
	/// <param name="left">2次元ベクトル</param>
	/// <returns>掛け算後の2次元ベクトル</returns>
	Vec2 operator*(Vec2 left) const;

	Matrix3x3 Angle(float rad);
public:
	// 変数
	float m00 = 0.0f;
	float m01 = 0.0f;
	float m02 = 0.0f;
	float m10 = 0.0f;
	float m11 = 0.0f;
	float m12 = 0.0f;
	float m20 = 0.0f;
	float m21 = 0.0f;
	float m22 = 0.0f;
};

