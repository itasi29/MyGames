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
};

/// <summary>
/// ベクター構造体
/// </summary>
struct Vec3
{
	float x, y, z;

	Vec3();
	Vec3(float inX, float inY, float inZ);

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

using Pos3 = Vec3;

/// <summary>
/// 矩形の当たり判定(2D判定)
/// 足元中心
/// </summary>
struct Rect
{
private:
	// 位置
	Pos3 pos;
	// 幅高さ
	float h, w, d;

public:
	Rect();

	/// <summary>
	/// 幅高さを設定
	/// </summary>
	/// <param name="inH">高さ</param>
	/// <param name="inW">幅</param>
	/// <param name="inD">奥行き</param>
	void SetRange(float inH, float inW, float inD);
	/// <summary>
	/// 場所を設定
	/// </summary>
	/// <param name="inX">X座標</param>
	/// <param name="inY">Y座標</param>
	void SetPos(float inX, float inY);
	/// <summary>
	/// 場所を設定
	/// </summary>
	/// <param name="inPos">座標</param>
	void SetPos(const Pos3& inPos);
	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="rect">判定側</param>
	/// <returns>true:当たった / false:当たっていない</returns>
	bool IsHit(const Rect& rect);
	/// <summary>
	/// 幅を取得
	/// </summary>
	/// <returns>width</returns>
	float GetW() const { return w; }

	/// <summary>
	/// 判定場所描画
	/// </summary>
	/// <param name="color">カラー</param>
	void Draw(unsigned int color = 0xff0000) const;
};