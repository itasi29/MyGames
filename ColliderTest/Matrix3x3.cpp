#include "Matrix3x3.h"
#include "Geometry.h"

void Matrix3x3::Identity()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (j == i)
			{
				m[i][j] = 1.0f;
			}
			else
			{
				m[i][j] = 0.0f;
			}
		}
	}
}

Matrix3x3 Matrix3x3::GetInverse()
{
	// WARNING: ŠÈˆÕŽÀ‘•
	Matrix3x3 mat;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mat.m[j][i] = m[i][j];
		}
	}

	return mat;
}

void Matrix3x3::SetLine(int lineNo, const Vec3& vec)
{
	m[0][lineNo] = vec.x;
	m[1][lineNo] = vec.y;
	m[2][lineNo] = vec.z;
}

void Matrix3x3::SetRow(int rowNo, const Vec3& vec)
{
	m[rowNo][0] = vec.x;
	m[rowNo][1] = vec.y;
	m[rowNo][2] = vec.z;
}

Vec3 Matrix3x3::GetLine(int lineNo)
{
	return Vec3(m[0][lineNo], m[1][lineNo], m[2][lineNo]);
}

Vec3 Matrix3x3::GetRow(int rowNo)
{
	return Vec3(m[rowNo][0], m[rowNo][1], m[rowNo][2]);
}
