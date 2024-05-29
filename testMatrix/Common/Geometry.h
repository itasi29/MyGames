#pragma once
#include "Vec3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

// ����
float Dot(const Vec3& item1, const Vec3& item2);
// �O��
Vec3 Cross(const Vec3& item1, const Vec3& item2);

// ���s�ړ�
Matrix4x4 Move(const Vec3& move);
Matrix4x4 Move(float x, float y, float z);
// �g��
Matrix4x4 Scale(const Vec3& scale);
Matrix4x4 Scale(float x, float y, float z);
// ��]
// TODO:�ǂ���낤
Matrix4x4 Rotate(const Quaternion& item);

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
	static Vec3 EaseIn(const Vec3& start, const Vec3& end, float t);
	// ����
	static Vec3 EaseOut(const Vec3& start, const Vec3& end, float t);
	// �������Č���
	static Vec3 EaseInOut(const Vec3& start, const Vec3& end, float t);
};