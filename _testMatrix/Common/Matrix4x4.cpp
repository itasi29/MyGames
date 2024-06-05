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
    Matrix4x4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            result.m[row * 4 + line] = Dot(mat, line, row);
        }
    }

    return result;
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
    m[0 + 0] = 1.0f;
    m[4 + 1] = 1.0f;
    m[8 + 2] = 1.0f;
    m[12 + 3] = 1.0f;
}

Matrix4x4 Matrix4x4::Transpose()
{
    Matrix4x4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            result.m[row * 4 + line] = m[line * 4 + row];
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
                result.m[row * 4 + line] = m[line * 4 + row];
            }
        }
        // •½sˆÚ“®•”•ª‚Ì’l”½“]
        for (int i = 0; i < 3; ++i)
        {
            int idx = 4 * i + 3;
            result.m[idx] = m[idx] * -1;
        }
    }
    // ³Šm‚È‚â‚è•û
    else
    {
        // MEMO:ŒöŽ®’·‚­‚Ä—Ís‚«‚½‚Ì‚Å•K—v‚È‚Æ‚«‚Éì‚Á‚Ä‰º‚³‚¢
#if false
        float det;
        det = m[0 + 0] * m[4 + 1] * m[8 + 2] * m[12 + 3] +
              m[0 + 0] * m[4 + 2] * m[8 + 3] * m[12 + 1] +
              m[0 + 0] * m[4 + 3] * m[8 + 1] * m[12 + 2] +
              m[0 + 1] * m[4 + 0] * m[8 + 3] * m[12 + 2] +
              m[0 + 1] * m[4 + 2] * m[8 + 0] * m[12 + 3] +
              m[0 + 1] * m[4 + 3] * m[8 + 2] * m[12 + 0] +
              m[0 + 2] * m[4 + 0] * m[8 + 1] * m[12 + 3] +
              m[0 + 2] * m[4 + 1] * m[8 + 3] * m[12 + 0] +
              m[0 + 2] * m[4 + 3] * m[8 + 0] * m[12 + 1] +
              m[0 + 3] * m[4 + 0] * m[8 + 2] * m[12 + 1] +
              m[0 + 3] * m[4 + 1] * m[8 + 0] * m[12 + 2] +
              m[0 + 3] * m[4 + 2] * m[8 + 1] * m[12 + 0] -
              m[0 + 0] * m[4 + 1] * m[8 + 3] * m[12 + 2] -
              m[0 + 0] * m[4 + 2] * m[8 + 1] * m[12 + 3] -
              m[0 + 0] * m[4 + 3] * m[8 + 2] * m[12 + 1] -
              m[0 + 1] * m[4 + 0] * m[8 + 2] * m[12 + 3] -
              m[0 + 1] * m[4 + 1] * m[8 + 3] * m[12 + 0] -
              m[0 + 1] * m[4 + 3] * m[8 + 0] * m[12 + 2] -
              m[0 + 2] * m[4 + 0] * m[8 + 3] * m[12 + 1] -
              m[0 + 2] * m[4 + 1] * m[8 + 0] * m[12 + 3] -
              m[0 + 2] * m[4 + 3] * m[8 + 1] * m[12 + 0] -
              m[0 + 3] * m[4 + 0] * m[8 + 1] * m[12 + 2] -
              m[0 + 3] * m[4 + 1] * m[8 + 2] * m[12 + 0] -
              m[0 + 3] * m[4 + 2] * m[8 + 0] * m[12 + 1];
        if (det != 0.0f)
        {


            float b11 = m[4 + 2] * m[8 + 3] * m[12 + 4] + 
                        m[4 + 3] * m[8 + 4] * m[12 + 2] +
                        m[4 + 4] * m[8 + 2] * m[12 + 3] -
                        m[4 + 2] * m[8 + 4] * m[12 + 3] -
                        m[4 + 3] * m[8 + 2] * m[12 + 4] -
                        m[4 + 4] * m[8 + 3] * m[12 + 2];
            float b12 = m[4 + ] * m[8 + ] * m[12 + ] +
                        m[4 + ] * m[8 + ] * m[12 + ] +
                        m[4 + ] * m[8 + ] * m[12 + ] -
                        m[4 + ] * m[8 + ] * m[12 + ] -
                        m[4 + ] * m[8 + ] * m[12 + ] -
                        m[4 + ] * m[8 + ] * m[12 + ];
            float b13 = m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ];
            float b14 = m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ];
            float b21 = m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ];
            float b22 = m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ];
            float b23 = m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ];
            float b24 = m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ];
            float b31 = m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ];
            float b32 = m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ];
            float b33 = m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ];
            float b34 = m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ];
            float b41 = m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ];
            float b42 = m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] +
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ] -
                m[4 + ] * m[8 + ] * m[12 + ];
            float b43 = ;
            float b44 = ;
        }
#endif
    }
    

    return result;
}

float Matrix4x4::Dot(const Matrix4x4& mat, int line, int row) const
{
    float result = 0.0f;

    for (int i = 0; i < 4; ++i)
    {
        result += m[row * 4 + i] * mat.m[i * 4 + line];
    }

    return result;
}
