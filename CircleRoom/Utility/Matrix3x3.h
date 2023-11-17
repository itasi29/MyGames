#pragma once
#include "Vec2.h"

/// <summary>
/// 4*4�̍s��
/// m'�s''��'�Ɛ������Ή����Ă���
/// </summary>
class Matrix4x4
{
public:
	Matrix4x4();

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
	Matrix4x4 operator*(Matrix4x4 left) const;
	/// <summary>
	/// �s��̊|���Z
	/// </summary>
	/// <param name="left">3*3�s��</param>
	/// <returns>�|���Z���3*3�s��</returns>
	Matrix4x4 operator*=(Matrix4x4 left);

	/// <summary>
	/// �s��ƃx�N�g���̊|���Z
	/// </summary>
	/// <param name="left">2�����x�N�g��</param>
	/// <returns>�|���Z���2�����x�N�g��</returns>
	Vec2 operator*(Vec2 left) const;

	Matrix4x4 AngleAxisZ(float rad);
public:
	// �ϐ�
	float m00 = 0.0f;
	float m01 = 0.0f;
	float m02 = 0.0f;
	float m03 = 0.0f;
	float m10 = 0.0f;
	float m11 = 0.0f;
	float m12 = 0.0f;
	float m13 = 0.0f;
	float m20 = 0.0f;
	float m21 = 0.0f;
	float m22 = 0.0f;
	float m23 = 0.0f;
	float m30 = 0.0f;
	float m31 = 0.0f;
	float m32 = 0.0f;
	float m33 = 0.0f;
};

