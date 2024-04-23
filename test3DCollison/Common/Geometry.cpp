#include "Common/Geometry.h"
#include <cassert>
#include <cmath>

Vec3::Vec3() :
	x(0.0f),
	y(0.0f),
	z(0.0f)
{
}

Vec3::Vec3(float inX, float inY, float inZ) :
	x(inX),
	y(inY),
	z(inZ)
{
}

Vec3 Vec3::operator+(const Vec3& val) const
{
	return Vec3( x + val.x, y + val.y, z + val.z );
}

void Vec3::operator+=(const Vec3& val)
{
	x += val.x;
	y += val.y;
	z += val.z;
}

Vec3 Vec3::operator-(const Vec3& val) const
{
	return Vec3( x - val.x, y - val.y, z - val.z );
}

void Vec3::operator-=(const Vec3& val)
{
	x -= val.x;
	y -= val.y;
	z -= val.z;
}

Vec3 Vec3::operator*(float scale) const
{
	return Vec3( x * scale, y * scale, z * scale );
}

void Vec3::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

Vec3 Vec3::operator/(float scale) const
{
	if (SqLength() == 0.0f)	{
		assert(false);
		return Vec3();
	}

	return Vec3( x / scale, y / scale, z / scale );
}

void Vec3::operator/=(float scale)
{
	if (SqLength() == 0.0f) {
		assert(false);
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	x /= scale;
	y /= scale;
	z /= scale;
}

bool Vec3::operator==(const Vec3& val) const
{
	return x == val.x && y == val.y && z == val.z;
}

bool Vec3::operator!=(const Vec3& val) const
{
	return !(*this == val);
}

float Vec3::Length() const
{
	return std::hypot(x, y, z);
}

float Vec3::SqLength() const
{
	return x * x + y * y + z * z;
}

void Vec3::Normalize()
{
	float len = Length();
	if (len == 0.0f) {
		assert(false);
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		return;
	}

	*this /= len;
}

Vec3 Vec3::GetNormalized() const
{
	float len = Length();
	if (len == 0.0f) {
		return Vec3();
	}

	return Vec3( x / len, y / len, z / len );
}

Rect::Rect() :
	pos {},
	h(0.0f),
	w(0.0f)
{
}

void Rect::SetRange(float inH, float inW, float inD)
{
	h = inH;
	w = inW;
	d = inD;
}

void Rect::SetPos(float inX, float inY)
{
	pos.x = inX;
	pos.y = inY;
}

void Rect::SetPos(const Pos3& inPos)
{
	pos = inPos;
}

bool Rect::IsHit(const Rect& rect)
{
	// 自身の四隅の座標
	float oTop = pos.y + h;
	float oBottom = pos.y;
	float oRight = pos.x + w * 0.5f;
	float oLeft = pos.x - w * 0.5f;
	// 対象の四隅の座標
	float tTop = rect.pos.y + rect.h;
	float tBottom = rect.pos.y;
	float tRight = rect.pos.x + rect.w * 0.5f;
	float tLeft = rect.pos.x - rect.w * 0.5f;

	// 上下判定
	if (oTop < tBottom) return false;
	if (oBottom > tTop) return false;
	// 左右判定
	if (oRight < tLeft) return false;
	if (oLeft > tRight) return false;

	// 全ての条件を通ったため当たっている
	return true;
}

void Rect::Draw(unsigned int color) const
{
#ifdef _DEBUG
	float top = pos.y + h;
	float bottom = pos.y;
	float right = pos.x + w * 0.5f;
	float left = pos.x - w * 0.5f;
	VECTOR lefttop = VGet(left, top, 0.0f);
	VECTOR righttop = VGet(right, top, 0.0f);
	VECTOR leftbottom = VGet(left, bottom, 0.0f);
	VECTOR rightbottom = VGet(right, bottom, 0.0f);

	DrawLine3D(lefttop, righttop, color);
	DrawLine3D(righttop, rightbottom, color);
	DrawLine3D(rightbottom, leftbottom, color);
	DrawLine3D(leftbottom, lefttop, color);
#endif
}

Vec3 Lerp(const Vec3& start, const Vec3& end, float t)
{
	float x = start.x + t * (end.x - start.x);
	float y = start.y + t * (end.y - start.y);
	float z = start.z + t * (end.z - start.z);
	return Vec3(x, y, z);
}

void CircleCol::SetRadisu(float radius)
{
	this->radius = radius;
}

void CircleCol::SetPos(const Pos3& pos)
{
	this->pos = pos;
}

bool CircleCol::IsHit(const CircleCol& col)
{
	float len = (pos - col.pos).SqLength();
	float dis = (radius + col.radius) * (radius + col.radius);

	return dis > len;
}

void CircleCol::Draw(unsigned int color) const
{
	DrawSphere3D(pos.GetVECTOR(), radius, 5, color, 0, true);
}
