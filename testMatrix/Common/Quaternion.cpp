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

Matrix4x4 Quaternion::operator*(const Matrix4x4& mat) const
{
    Matrix4x4 matQ;

    float r00 = x * x + y * y - z * z - w * w;
    float r01 = 2 * (y * z - x * w);
    float r02 = 2 * (y * w - x * z);
    float r10 = 2 * (y * z + x * w);
    float r11 = x * x - y * y + z * z - w * w;
    float r12 = 2 * (z * w - x * y);
    float r20 = 2 * (y * w - x * z);
    float r21 = 2 * (z * w + x * y);
    float r22 = x * x - y * y - z * z + w * w;

    matQ.m[0 + 0] = r00;
    matQ.m[0 + 1] = r01;
    matQ.m[0 + 2] = r02;
    matQ.m[4 + 0] = r10;
    matQ.m[4 + 1] = r11;
    matQ.m[4 + 2] = r12;
    matQ.m[8 + 0] = r20;
    matQ.m[8 + 1] = r21;
    matQ.m[8 + 2] = r22;
    matQ.m[12 + 3] = 1.0f;

    return matQ * mat;
}

Vec3 Quaternion::operator*(const Vec3& vec) const
{
    Quaternion posQ = Quaternion(vec.x, vec.y, vec.z, 1.0f);

    Quaternion newPos = *this * posQ * this->Conjugated();
    
    return Vec3(newPos.x, newPos.y, newPos.z);
}
