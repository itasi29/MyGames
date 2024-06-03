#include "Quaternion.h"

// HACK: ‰Ê‚½‚µ‚Ä‰Šú‰»Žq‚±‚ê‚Å‚¢‚¢‚Ì‚©...
Quaternion::Quaternion() :
    x(0.0f),
    y(0.0f),
    z(0.0f),
    w(0.0f)
{
}

Quaternion::Quaternion(float inX, float inY, float inZ, float inW) :
    x(inX),
    y(inY),
    z(inZ),
    w(inW)
{
}

Quaternion Quaternion::Conjugated() const
{
    return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
    Quaternion res;

    res.x = x * q.w + w * q.x - z * q.y + y * q.z;
    res.y = y * q.w + z * q.x + w * q.y - x * q.z;
    res.z = z * q.w - y * q.x + x * q.y + w * q.z;
    res.w = w * q.w - x * q.x - y * q.y - z * q.z;

    return res;
}

Vec3 Quaternion::operator*(const Vec3& vec) const
{
    Quaternion posQuaternion = Quaternion(vec.x, vec.y, vec.z, 1.0f);

    Quaternion newPos = *this * posQuaternion * this->Conjugated();
    
    return Vec3(newPos.x, newPos.y, newPos.z);
}
