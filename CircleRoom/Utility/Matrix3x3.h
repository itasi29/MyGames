#pragma once
#include "Vec2.h"

/// <summary>
/// 3*3�̍s��
/// m'�s''��'�Ɛ������Ή����Ă���
/// </summary>
class Matrix3x3
{
public:
	Matrix3x3();

	/// <summary>
	/// ���M��P�ʍs��ɂ���
	/// </summary>
	/// <returns></returns>
	void Identity();

	/// <summary>
	/// �s��̊|���Z
	/// </summary>
	/// <param name="left">3*3�s��</param>
	/// <returns>�|���Z���3*3�s��</returns>
	Matrix3x3 operator*(Matrix3x3 left) const;
	/// <summary>
	/// �s��̊|���Z
	/// </summary>
	/// <param name="left">3*3�s��</param>
	/// <returns>�|���Z���3*3�s��</returns>
	Matrix3x3 operator*=(Matrix3x3 left);

	/// <summary>
	/// �s��ƃx�N�g���̊|���Z
	/// </summary>
	/// <param name="left">2�����x�N�g��</param>
	/// <returns>�|���Z���2�����x�N�g��</returns>
	Vec2 operator*(Vec2 left) const;

	Matrix3x3 Angle(float rad);
public:
	// �ϐ�
	float m00 = 0.0f;
	float m01 = 0.0f;
	float m02 = 0.0f;
	float m10 = 0.0f;
	float m11 = 0.0f;
	float m12 = 0.0f;
	float m20 = 0.0f;
	float m21 = 0.0f;
	float m22 = 0.0f;
};

