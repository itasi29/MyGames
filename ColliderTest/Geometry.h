#pragma once
#include <DxLib.h>

/// <summary>
/// サイズ構造体
/// </summary>
struct Size
{
	// 幅
	float w = 0.0f;
	// 高さ
	float h = 0.0f;
	// 奥行き
	float d = 0.0f;

	Size();
	Size(float inW, float inH, float inD);

	Size operator+(const Size& val) const;
	Size operator*(float scale) const;
};

/// <summary>
/// ベクター構造体
/// </summary>
struct Vec3
{
	float x, y, z;

	Vec3();
	Vec3(float inX, float inY, float inZ);

	Vec3 Reverse();

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

	/// <summary>
	/// DxLibの関数を使う際、VECTOR型に変換する関数
	/// </summary>
	/// <returns>VECTOR型の現在の値</returns>
	VECTOR GetVECTOR() const { return VGet(x, y, z); }

	/// <summary>
	/// 長さを取得
	/// </summary>
	/// <returns>長さ</returns>
	float Length() const;
	/// <summary>
	/// 長さの二乗を取得
	/// </summary>
	/// <returns>長さ</returns>
	float SqLength() const;
	/// <summary>
	/// 自身の正規化
	/// </summary>
	void Normalize();
	/// <summary>
	/// 正規化した結果を取得
	/// </summary>
	Vec3 GetNormalized() const;
};

/// <summary>
/// 線形補間
/// </summary>
/// <param name="start">スタート地点</param>
/// <param name="end">終了地点</param>
/// <param name="t">割合</param>
/// <returns>補間後</returns>
Vec3 Lerp(const Vec3& start, const Vec3& end, float t);

/// <summary>
/// 内積
/// </summary>
/// <param name="vec1">ベクトル１</param>
/// <param name="vec2">ベクトル２</param>
/// <returns>内積値</returns>
float Dot(const Vec3& vec1, const Vec3& vec2);

/// <summary>
/// 外積
/// </summary>
/// <param name="vec1">ベクトル１</param>
/// <param name="vec2">ベクトル２</param>
/// <returns>外積</returns>
Vec3 Cross(const Vec3& vec1, const Vec3& vec2);

using Pos3 = Vec3;