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
    return GetMat() * mat;
}

Vec3 Quaternion::operator*(const Vec3& vec) const
{
    Quaternion posQ = Quaternion(vec.x, vec.y, vec.z, 1.0f);

    Quaternion newPos = *this * posQ * this->Conjugated();
    
    return Vec3(newPos.x, newPos.y, newPos.z);
}

Matrix4x4 Quaternion::GetMat() const
{
    Matrix4x4 matQ;

    float x2 = x * x;
    float y2 = y * y;
    float z2 = z * z;
    float w2 = w * w;

    float r00 = x2 - y2 - z2 + w2;
    float r01 = 2.0f * (x * y + z * w);
    float r02 = 2.0f * (x * z - y * w);

    float r10 = 2.0f * (x * y - z * w);
    float r11 = -x2 + y2 - z2 + w2;
    float r12 = 2.0f * (y * z + x * w);

    float r20 = 2.0f * (x * z + y * w);
    float r21 = 2.0f * (y * z - x * w);
    float r22 = -x2 - y2 + z2 + w2;

    matQ.m[0][0] = r00;
    matQ.m[0][1] = r01;
    matQ.m[0][2] = r02;
    matQ.m[1][0] = r10;
    matQ.m[1][1] = r11;
    matQ.m[1][2] = r12;
    matQ.m[2][0] = r20;
    matQ.m[2][1] = r21;
    matQ.m[2][2] = r22;

    matQ.m[3][3] = 1.0f;

    return matQ;
}
