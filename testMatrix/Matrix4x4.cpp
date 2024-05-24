#include "Matrix4x4.h"

Matrix4x4::Matrix4x4() :
    m{}
{
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& mat) const
{
    Matrix4x4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            int idx = row * 4 + line;
            result.m[idx] = m[idx] + mat.m[idx];
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& mat) const
{
    Matrix4x4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            int idx = row * 4 + line;
            result.m[idx] = m[idx] - mat.m[idx];
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mat) const
{
    return Matrix4x4();
}

void Matrix4x4::Zero()
{
    for (auto& itme : m)
    {
        itme = 0.0f;
    }
}

void Matrix4x4::Identity()
{
    Zero();
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
}

Matrix4x4 Matrix4x4::Inverse(bool isCorrect)
{
    Matrix4x4 result;

    // ŠÈ—ª‚È‚â‚è•û
    if (!isCorrect)
    {
        for (int row = 0; row < 4; ++row)
        {
            for (int line = 0; line < 4; ++line)
            {
                result.m[row * 4 + line] = m[line * 4 + row];
            }
        }
    }
    // ³Šm‚È‚â‚è•û
    else
    {
        float det;
        det = m[0] * m[5] * m[10] * m[15] +
              m[0] * m[6] * m[11] * m[13] +
              m[0] * m[7] * m[9]  * m[14] +
              m[1] * m[4] * m[11] * m[14] +
              m[1] * m[6] * m[8]  * m[15] +
              m[1] * m[7] * m[10] * m[12] +
              m[2] * m[4] * m[9]  * m[15] +
              m[2] * m[5] * m[11] * m[12] +
              m[2] * m[7] * m[8]  * m[13] +
              m[3] * m[4] * m[10] * m[13] +
              m[3] * m[5] * m[8]  * m[14] +
              m[3] * m[6] * m[9]  * m[12] -
              m[0] * m[5] * m[11] * m[14] -
              m[0] * m[6] * m[9]  * m[15] -
              m[0] * m[7] * m[10] * m[13] -
              m[1] * m[4] * m[10] * m[15] -
              m[1] * m[6] * m[11] * m[12] -
              m[1] * m[7] * m[8]  * m[14] -
              m[2] * m[4] * m[11] * m[13] -
              m[2] * m[5] * m[8]  * m[15] -
              m[2] * m[7] * m[9]  * m[12] -
              m[3] * m[4] * m[9]  * m[14] -
              m[3] * m[5] * m[10] * m[12] -
              m[3] * m[6] * m[8]  * m[13];

        if (det != 0.0f)
        {

        }
    }
    

    return result;
}

void Matrix4x4::SetLine(int lineNo, const Vec3 val)
{
}

void Matrix4x4::SetRow(int rowNo, const Vec3 val)
{
}

Vec3 Matrix4x4::GetLine(int lineNo) const
{
    return Vec3();
}

Vec3 Matrix4x4::GetRow(int rowNo) const
{
    return Vec3();
}
