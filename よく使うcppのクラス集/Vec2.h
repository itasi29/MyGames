#pragma once
#include <cmath>

/// ベクトルを便利に使うためのクラス(2次元ベクトル)
class Vec2
{
public:
	float x;
	float y;

public:
	Vec2() :
		x(0.0f),
		y(0.0f)
	{
	}

	Vec2(float posX, float posY) :
		x(posX),
		y(posY)
	{
	}

	/// 単項演算子+
	Vec2 operator +() const
	{
		// thisポインタ : 自分自身のアドレスを指す
		return *this;
	}

	/// 単項演算子- 逆ベクトル
	Vec2 operator -() const
	{
		return Vec2{ -x, -y };
	}

	/// 足し算
	Vec2 operator +(Vec2 vec) const
	{
		return Vec2{ x + vec.x, y + vec.y };
	}
	Vec2 operator +=(Vec2 vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	/// 引き算
	Vec2 operator -(Vec2 vec) const
	{
		return Vec2{ x - vec.x, y - vec.y };
	}
	Vec2 operator -=(Vec2 vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	/// 掛け算 
	Vec2 operator *(float scale) const
	{
		return Vec2{ x * scale, y * scale };
	}
	Vec2 operator *=(float scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}

	/// 割り算
	Vec2 operator /(float scale) const
	{
		return Vec2{ x / scale, y / scale };
	}
	Vec2 operator /=(float scale)
	{
		x /= scale;
		y /= scale;
		return *this;
	}

	/// ベクトルの長さの2乗を求める
	float SqLength() const
	{
		return x * x + y * y;
	}

	/// ベクトルの長さを求める
	float Length() const
	{
		return sqrtf(SqLength());
	}

	/// 自身の正規化を行う
	void Normalize()
	{
		float len = Length();

		// 長さ0以下なら処理を行わない
		if (len <= 0.0f) return;

		x /= len;
		y /= len;
	}

	/// 自身を正規化したベクトルを取得する
	Vec2 GetNormalized() const
	{
		float len = Length();

		// 長さ0以下なら(0, 0)を返す
		/* 引数ありにしている理由 */
		// 引数なしコンストラクタの修正に強くするため
		if (len <= 0.0f) return Vec2{ 0.0f, 0.0f };

		return Vec2{ x / len, y / len };
	}
};