#pragma once
#include "Vec3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

namespace MyEngine
{
	namespace Math
	{
		constexpr double kPi = 3.14159265358979323846;
		constexpr float kPiF = 3.14159265358979323846f;

		constexpr float kDeg2Rad = kPiF / 180.0f;
		constexpr float kRad2Deg = 180.0f / kPiF;
	}

	// ����
	float Dot(const Vec3& item1, const Vec3& item2);
	// �O��
	Vec3 Cross(const Vec3& item1, const Vec3& item2);
	// �ˉe
	Vec3 Projection(const Vec3& projection, const Vec3& base);
	// �ŋߐړ_
	Vec3 GetNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end);
	// ������ɍŋߐړ_�����邩�̔���
	bool IsNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end);

	// ���s�ړ�
	Matrix4x4 Move(const Vec3& move);
	Matrix4x4 Move(float x, float y, float z);
	// �g��
	Matrix4x4 Scale(const Vec3& scale);
	Matrix4x4 Scale(float x, float y, float z);
	// ��](�x���@)
	Quaternion AngleAxis(float angle, const Vec3& axis);

	// �C�[�W���O
	class Easing
	{
	private:
		// �N���X�̍쐬�͂����Ȃ�
		Easing();
	public:
		// ����
		static Vec3 Linear(const Vec3& start, const Vec3& end, float t);
		// ����
		static Vec3 EaseIn(const Vec3& start, const Vec3& end, float t, float effect = 2.0f);
		// ����
		static Vec3 EaseOut(const Vec3& start, const Vec3& end, float t, float effect = 2.0f);
		// �������Č���
		static Vec3 EaseInOut(const Vec3& start, const Vec3& end, float t, float effect = 2.0f);
	};
}