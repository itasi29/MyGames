#pragma once

struct Vec3;

struct Matrix3x3
{
	// MEMO:����͗񁨍s�Ŕz��͈���
	// �s�P��Q�̏ꍇ��m[2][1]
	float m[3][3];

	void Identity();
	Matrix3x3 GetInverse();

	void SetLine(int lineNo, const Vec3& vec);
	void SetRow(int rowNo, const Vec3& vec);

	Vec3 GetLine(int lineNo);
	Vec3 GetRow(int rowNo);
};

