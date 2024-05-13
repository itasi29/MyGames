#pragma once
#include <DxLib.h>

/// <summary>
/// �T�C�Y�\����
/// </summary>
struct Size
{
	// ��
	float w = 0.0f;
	// ����
	float h = 0.0f;
	// ���s��
	float d = 0.0f;

	Size();
	Size(float inW, float inH, float inD);

	Size operator+(const Size& val) const;
	Size operator*(float scale) const;
};

/// <summary>
/// �x�N�^�[�\����
/// </summary>
struct Vec3
{
	float x, y, z;

	Vec3();
	Vec3(float inX, float inY, float inZ);

	Vec3 operator+(const Vec3& val) const;
	void operator+=(const Vec3& val);
	Vec3 operator-(const Vec3& val) const;
	void operator-=(const Vec3& val);
	Vec3 operator*(float scale) const;
	void operator*=(float scale);
	Vec3 operator/(float scale) const;
	void operator/=(float scale);
	bool operator==(const Vec3& val) const;
	bool operator!=(const Vec3& val) const;

	/// <summary>
	/// DxLib�̊֐����g���ہAVECTOR�^�ɕϊ�����֐�
	/// </summary>
	/// <returns>VECTOR�^�̌��݂̒l</returns>
	VECTOR GetVECTOR() const { return VGet(x, y, z); }

	/// <summary>
	/// �������擾
	/// </summary>
	/// <returns>����</returns>
	float Length() const;
	/// <summary>
	/// �����̓����擾
	/// </summary>
	/// <returns>����</returns>
	float SqLength() const;
	/// <summary>
	/// ���g�̐��K��
	/// </summary>
	void Normalize();
	/// <summary>
	/// ���K���������ʂ��擾
	/// </summary>
	Vec3 GetNormalized() const;
};

/// <summary>
/// ���`���
/// </summary>
/// <param name="start">�X�^�[�g�n�_</param>
/// <param name="end">�I���n�_</param>
/// <param name="t">����</param>
/// <returns>��Ԍ�</returns>
Vec3 Lerp(const Vec3& start, const Vec3& end, float t);

/// <summary>
/// ����
/// </summary>
/// <param name="vec1">�x�N�g���P</param>
/// <param name="vec2">�x�N�g���Q</param>
/// <returns>���ϒl</returns>
float Dot(const Vec3& vec1, const Vec3& vec2);

using Pos3 = Vec3;