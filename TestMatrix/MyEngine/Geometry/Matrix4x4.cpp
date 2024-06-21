#include "Matrix4x4.h"
#include "Quaternion.h"

using namespace MyEngine;

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
            result.m[row][line] = m[row][line] + mat.m[row][line];
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
            result.m[row][line] = m[row][line] - mat.m[row][line];
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mat) const
{
    Matrix4x4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            result.m[row][line] = Dot(mat, line, row);
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::operator*(const Quaternion& q) const
{
    return *this * q.GetMat();
}

Matrix4x4 Matrix4x4::operator/(float div) const
{
    Matrix4x4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            result.m[row][line] = m[row][line] / div;
        }
    }

    return result;
}

void Matrix4x4::Zero()
{
    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            m[row][line] = 0.0f;
        }
    }
}

void Matrix4x4::Identity()
{
    Zero();
    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
}

Matrix4x4 Matrix4x4::Transpose()
{
    Matrix4x4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            result.m[row][line] = m[line][row];
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::Inverse(bool isCorrect)
{
    Matrix4x4 result;

    // ŠÈ—ª‚È‚â‚è•û
    if (!isCorrect)
    {
        // ‰ñ“]•”•ª‚Ì“]’u
        for (int row = 0; row < 3; ++row)
        {
            for (int line = 0; line < 3; ++line)
            {
                result.m[row][line] = m[line][row];
            }
        }
        // •½sˆÚ“®•”•ª‚Ì’l”½“]
        for (int i = 0; i < 3; ++i)
        {
            result.m[3][i] = -m[3][i];
        }
    }
    // ³Šm‚È‚â‚è•û
    else
    {
        float det;
        // REVIEW: ŠÔˆá‚Á‚Ä‚½‚çŒöŽ®Œ©’¼‚µ‚Ä
        det = m[0][0] * m[1][1] * m[2][2] * m[3][3] +
              m[0][0] * m[1][2] * m[2][3] * m[3][1] +
              m[0][0] * m[1][3] * m[2][1] * m[3][2] +
              m[0][1] * m[1][0] * m[2][3] * m[3][2] +
              m[0][1] * m[1][2] * m[2][0] * m[3][3] +
              m[0][1] * m[1][3] * m[2][2] * m[3][0] +
              m[0][2] * m[1][0] * m[2][1] * m[3][3] +
              m[0][2] * m[1][1] * m[2][3] * m[3][0] +
              m[0][2] * m[1][3] * m[2][0] * m[3][1] +
              m[0][3] * m[1][0] * m[2][2] * m[3][1] +
              m[0][3] * m[1][1] * m[2][0] * m[3][2] +
              m[0][3] * m[1][2] * m[2][1] * m[3][0] -
              m[0][0] * m[1][1] * m[2][3] * m[3][2] -
              m[0][0] * m[1][2] * m[2][1] * m[3][3] -
              m[0][0] * m[1][3] * m[2][2] * m[3][1] -
              m[0][1] * m[1][0] * m[2][2] * m[3][3] -
              m[0][1] * m[1][1] * m[2][3] * m[3][0] -
              m[0][1] * m[1][3] * m[2][0] * m[3][2] -
              m[0][2] * m[1][0] * m[2][3] * m[3][1] -
              m[0][2] * m[1][1] * m[2][0] * m[3][3] -
              m[0][2] * m[1][3] * m[2][1] * m[3][0] -
              m[0][3] * m[1][0] * m[2][1] * m[3][2] -
              m[0][3] * m[1][1] * m[2][2] * m[3][0] -
              m[0][3] * m[1][2] * m[2][0] * m[3][1];

        if (det != 0.0f)
        {
            // REVIEW: ŠÔˆá‚Á‚Ä‚½‚çŒöŽ®Œ©’¼‚µ‚Ä
            result.m[0][0]  = m[1][1] * m[2][2] * m[3][3] + m[1][2] * m[2][3] * m[3][1] + m[1][3] * m[2][1] * m[3][2] - m[1][1] * m[2][3] * m[3][2] - m[1][2] * m[2][1] * m[3][3] - m[1][3] * m[2][2] * m[3][1];
            result.m[0][1]  = m[0][1] * m[2][3] * m[3][2] + m[0][2] * m[2][1] * m[3][3] + m[0][3] * m[2][2] * m[3][1] - m[0][1] * m[2][2] * m[3][3] - m[0][2] * m[2][3] * m[3][1] - m[0][3] * m[2][1] * m[3][2];
            result.m[0][2]  = m[0][1] * m[1][2] * m[3][3] + m[0][2] * m[1][3] * m[3][1] + m[0][3] * m[1][1] * m[3][2] - m[0][1] * m[1][3] * m[3][2] - m[0][2] * m[1][1] * m[3][3] - m[0][3] * m[1][2] * m[3][1];
            result.m[0][3]  = m[0][1] * m[1][3] * m[2][2] + m[0][2] * m[1][1] * m[2][3] + m[0][3] * m[1][2] * m[2][1] - m[0][1] * m[1][2] * m[2][3] - m[0][2] * m[1][3] * m[2][1] - m[0][3] * m[1][1] * m[2][2];

            result.m[1][0]  = m[1][0] * m[2][3] * m[3][2] + m[1][2] * m[2][0] * m[3][3] + m[1][3] * m[2][2] * m[3][0] - m[1][0] * m[2][2] * m[3][3] - m[1][2] * m[2][3] * m[3][0] - m[1][3] * m[2][0] * m[3][2];
            result.m[1][1]  = m[0][0] * m[2][2] * m[3][3] + m[0][2] * m[2][3] * m[3][0] + m[0][3] * m[2][0] * m[3][2] - m[0][0] * m[2][3] * m[3][2] - m[0][2] * m[2][0] * m[3][3] - m[0][3] * m[2][2] * m[3][0];
            result.m[1][2]  = m[0][0] * m[1][3] * m[3][2] + m[0][2] * m[1][0] * m[3][3] + m[0][3] * m[1][2] * m[3][0] - m[0][0] * m[1][2] * m[3][3] - m[0][2] * m[1][3] * m[3][0] - m[0][3] * m[1][0] * m[3][2];
            result.m[1][3]  = m[0][0] * m[1][2] * m[2][3] + m[0][2] * m[1][3] * m[2][0] + m[0][3] * m[1][0] * m[2][2] - m[0][0] * m[1][3] * m[2][2] - m[0][2] * m[1][0] * m[2][3] - m[0][3] * m[1][2] * m[2][0];

            result.m[2][0]  = m[1][0] * m[2][1] * m[3][3] + m[1][1] * m[2][3] * m[3][0] + m[1][3] * m[2][0] * m[3][1] - m[1][0] * m[2][3] * m[3][1] - m[1][1] * m[2][0] * m[3][3] - m[1][3] * m[2][1] * m[3][0];
            result.m[2][1]  = m[0][0] * m[2][3] * m[3][1] + m[0][1] * m[2][0] * m[3][3] + m[0][3] * m[2][1] * m[3][0] - m[0][0] * m[2][1] * m[3][3] - m[0][1] * m[2][3] * m[3][0] - m[0][3] * m[2][0] * m[3][1];
            result.m[2][2]  = m[0][0] * m[1][1] * m[3][3] + m[0][1] * m[1][3] * m[3][0] + m[0][3] * m[1][0] * m[3][1] - m[0][0] * m[1][3] * m[3][1] - m[0][1] * m[1][0] * m[3][3] - m[0][3] * m[1][1] * m[3][0];
            result.m[2][3]  = m[0][0] * m[1][3] * m[2][1] + m[0][1] * m[1][0] * m[2][3] + m[0][3] * m[1][1] * m[2][0] - m[0][0] * m[1][1] * m[2][3] - m[0][1] * m[1][3] * m[2][0] - m[0][3] * m[1][0] * m[2][1];

            result.m[3][0] = m[1][0] * m[2][2] * m[3][1] + m[1][1] * m[2][0] * m[3][2] + m[1][2] * m[2][1] * m[3][0] - m[1][0] * m[2][1] * m[3][2] - m[1][1] * m[2][2] * m[3][0] - m[1][2] * m[2][0] * m[3][1];
            result.m[3][1] = m[0][0] * m[2][1] * m[3][2] + m[0][1] * m[2][2] * m[3][0] + m[0][2] * m[2][0] * m[3][1] - m[0][0] * m[2][2] * m[3][1] - m[0][1] * m[2][0] * m[3][2] - m[0][2] * m[2][1] * m[3][0];
            result.m[3][2] = m[0][0] * m[1][2] * m[3][1] + m[0][1] * m[1][0] * m[3][2] + m[0][2] * m[1][1] * m[3][0] - m[0][0] * m[1][1] * m[3][2] - m[0][1] * m[1][2] * m[3][0] - m[0][2] * m[1][0] * m[3][1];
            result.m[3][3] = m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1] - m[0][0] * m[1][2] * m[2][1] - m[0][1] * m[1][0] * m[2][2] - m[0][2] * m[1][1] * m[2][0];

            result = result / det;
        }
    }
    

    return result;
}

void Matrix4x4::SetPos(Vec3 pos)
{
    Identity();

    m[3][0] = pos.x;
    m[3][1] = pos.y;
    m[3][2] = pos.z;
}

void Matrix4x4::SetRot(const Quaternion& q)
{
    Matrix4x4 matQ = q.GetMat();
    matQ.m[3][3] = 0.0f;

    *this = *this + matQ;
}

Vec3 Matrix4x4::GetPos() const
{
    return Vec3(m[3][0], m[3][1], m[3][2]);
}

MATRIX Matrix4x4::GetMATRIX() const
{
    MATRIX result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            result.m[row][line] = m[row][line];
        }
    }

    return result;
}

float Matrix4x4::Dot(const Matrix4x4& mat, int line, int row) const
{
    float result = 0.0f;

    for (int i = 0; i < 4; ++i)
    {
        float val = m[row][i] * mat.m[i][line];
        result += val;
    }

    return result;
}
