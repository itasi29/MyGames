#pragma once
#include "Vec2.h"

/// <summary>
/// 4*4の行列
/// m'行''列'と数字が対応している
/// </summary>
class Matrix4x4
{
public:
	Matrix4x4();

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
	Matrix4x4 operator*(Matrix4x4 left) const;
	/// <summary>
	/// 行列の掛け算
	/// </summary>
	/// <param name="left">3*3行列</param>
	/// <returns>掛け算後の3*3行列</returns>
	Matrix4x4 operator*=(Matrix4x4 left);

	/// <summary>
	/// 行列とベクトルの掛け算
	/// </summary>
	/// <param name="left">2次元ベクトル</param>
	/// <returns>掛け算後の2次元ベクトル</returns>
	Vec2 operator*(Vec2 left) const;

	Matrix4x4 AngleAxisZ(float rad);
public:
	// 変数
	float m00 = 0.0f;
	float m01 = 0.0f;
	float m02 = 0.0f;
	float m03 = 0.0f;
	float m10 = 0.0f;
	float m11 = 0.0f;
	float m12 = 0.0f;
	float m13 = 0.0f;
	float m20 = 0.0f;
	float m21 = 0.0f;
	float m22 = 0.0f;
	float m23 = 0.0f;
	float m30 = 0.0f;
	float m31 = 0.0f;
	float m32 = 0.0f;
	float m33 = 0.0f;
};

