#pragma once
#include <DxLib.h>

namespace MyEngine
{
	/// <summary>
	/// �x�N�^�[
	/// </summary>
	class Vec3
	{
	public:
		float x;
		float y;
		float z;

		Vec3();
		Vec3(float inX, float inY, float inZ);

		static Vec3 Up();

		/* ���Z */
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

		// �����擾
		float Length() const;
		// ������2����擾
		float SqLength() const;
		// ���K�������x�N�g�����擾
		Vec3 GetNormalized() const;
		// ���g�̐��K��
		void Normalize();

		// DxLib�p��VECTOR���擾
		DxLib::VECTOR VGet() const;
	};
}
